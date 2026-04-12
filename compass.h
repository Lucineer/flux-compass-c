#ifndef COMPASS_H
#define COMPASS_H
#include <stdint.h>

typedef enum { DIR_N=0, DIR_NE, DIR_E, DIR_SE, DIR_S, DIR_SW, DIR_W, DIR_NW, DIR_COUNT=8 } Direction;
typedef struct { int x, y; } Vec2;

typedef struct {
    float heading;      // degrees 0-360 (0=North, 90=East)
    float target_heading;
    float angular_velocity;
    float turn_rate;    // max degrees per tick
    Vec2 position;
    Vec2 forward;       // unit vector from heading
} Compass;

void compass_init(Compass *c, float heading_deg);
void compass_set_heading(Compass *c, float deg);
void compass_set_target(Compass *c, float deg);
int compass_tick(Compass *c, float dt);
float compass_diff(float from, float to);
Direction compass_direction(const Compass *c);
Vec2 compass_forward(float heading);
int compass_facing(const Compass *c, float target_heading, float tolerance);
Vec2 compass_offset(const Compass *c, float distance);
float compass_distance(Vec2 a, Vec2 b);
float compass_angle_between(Vec2 from, Vec2 to);

#endif
