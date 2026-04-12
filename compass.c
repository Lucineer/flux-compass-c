#define _GNU_SOURCE
#include "compass.h"
#include <math.h>

static float norm(float deg) {
    deg = fmodf(deg, 360.0f);
    if (deg < 0.0f) deg += 360.0f;
    return deg;
}

void compass_init(Compass *c, float heading_deg) {
    c->heading = norm(heading_deg);
    c->target_heading = c->heading;
    c->angular_velocity = 0.0f;
    c->turn_rate = 180.0f;
    c->position = (Vec2){0, 0};
    c->forward = compass_forward(c->heading);
}

void compass_set_heading(Compass *c, float deg) {
    c->heading = norm(deg);
    c->forward = compass_forward(c->heading);
}

void compass_set_target(Compass *c, float deg) {
    c->target_heading = norm(deg);
}

int compass_tick(Compass *c, float dt) {
    float diff = compass_diff(c->heading, c->target_heading);
    if (fabsf(diff) < 0.01f) {
        c->heading = c->target_heading;
        c->angular_velocity = 0.0f;
        c->forward = compass_forward(c->heading);
        return 1;
    }
    float step = c->turn_rate * dt;
    if (fabsf(diff) <= step) {
        c->heading = c->target_heading;
        c->angular_velocity = diff / dt;
        c->forward = compass_forward(c->heading);
        return 1;
    } else {
        float dir = diff > 0.0f ? 1.0f : -1.0f;
        c->angular_velocity = dir * c->turn_rate;
        c->heading = norm(c->heading + dir * step);
    }
    c->forward = compass_forward(c->heading);
    return 0;
}

float compass_diff(float from, float to) {
    float d = to - from;
    while (d > 180.0f) d -= 360.0f;
    while (d < -180.0f) d += 360.0f;
    return d;
}

Direction compass_direction(const Compass *c) {
    // Each sector is 45 degrees wide, centered on the cardinal.
    // N=337.5..22.5, NE=22.5..67.5, etc.
    float h = norm(c->heading);
    h += 22.5f; // offset so sectors align
    if (h >= 360.0f) h -= 360.0f;
    return (Direction)(int)(h / 45.0f);
}

Vec2 compass_forward(float heading) {
    float rad = heading * (float)M_PI / 180.0f;
    // 0=North → (0,1), 90=East → (1,0)
    // atan2: x=sin, y=cos for compass bearings
    float x = sinf(rad);
    float y = cosf(rad);
    // Approximate to int: multiply by 10000 and round
    return (Vec2){ (int)(x * 10000.0f), (int)(y * 10000.0f) };
}

int compass_facing(const Compass *c, float target_heading, float tolerance) {
    return fabsf(compass_diff(c->heading, norm(target_heading))) <= tolerance;
}

Vec2 compass_offset(const Compass *c, float distance) {
    float rad = c->heading * (float)M_PI / 180.0f;
    float dx = sinf(rad) * distance;
    float dy = cosf(rad) * distance;
    return (Vec2){ c->position.x + (int)dx, c->position.y + (int)dy };
}

float compass_distance(Vec2 a, Vec2 b) {
    float dx = (float)(b.x - a.x);
    float dy = (float)(b.y - a.y);
    return sqrtf(dx * dx + dy * dy);
}

float compass_angle_between(Vec2 from, Vec2 to) {
    float dx = (float)(to.x - from.x);
    float dy = (float)(to.y - from.y);
    float deg = atan2f(dx, dy) * 180.0f / (float)M_PI;
    return norm(deg);
}
