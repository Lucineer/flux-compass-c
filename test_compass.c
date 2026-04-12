#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "compass.h"

static int tests_run = 0;
static int tests_passed = 0;

#define T(name) do { tests_run++; printf("  %-40s ", name);
#define END(f) if (f) { tests_passed++; printf("PASS\n"); } else { printf("FAIL\n"); } } while(0)
#define CLOSE(a,b,e) (fabsf((a)-(b)) < (e))

int main(void) {
    printf("Running compass tests...\n\n");

    // 1. init sets heading and forward
    Compass c;
    compass_init(&c, 90.0f);
    T("init: heading = 90");
        END(c.heading == 90.0f);

    // 2. init normalizes heading > 360
    compass_init(&c, 450.0f);
    T("init: 450 -> 90");
        END(c.heading == 90.0f);

    // 3. init normalizes negative heading
    compass_init(&c, -90.0f);
    T("init: -90 -> 270");
        END(c.heading == 270.0f);

    // 4. set_heading updates forward
    compass_init(&c, 0.0f);
    compass_set_heading(&c, 0.0f);
    T("forward North (0,10000)");
        END(c.forward.x == 0 && c.forward.y == 10000);

    // 5. forward East
    compass_set_heading(&c, 90.0f);
    T("forward East (10000,0)");
        END(c.forward.x == 10000 && c.forward.y == 0);

    // 6. forward South
    compass_set_heading(&c, 180.0f);
    T("forward South (0,-10000)");
        END(c.forward.x == 0 && c.forward.y == -10000);

    // 7. forward West
    compass_set_heading(&c, 270.0f);
    T("forward West (-10000,0)");
        END(c.forward.x == -10000 && c.forward.y == 0);

    // 8. diff same angle
    T("diff: 0 to 0 = 0");
        END(compass_diff(0, 0) == 0.0f);

    // 9. diff short way clockwise
    T("diff: 0 to 90 = 90");
        END(compass_diff(0, 90) == 90.0f);

    // 10. diff short way counterclockwise
    T("diff: 0 to 270 = -90");
        END(compass_diff(0, 270) == -90.0f);

    // 11. diff wraps past 360
    T("diff: 350 to 10 = 20");
        END(CLOSE(compass_diff(350, 10), 20.0f, 0.001f));

    // 12. direction bins North
    compass_init(&c, 0.0f);
    T("direction: 0 = DIR_N");
        END(compass_direction(&c) == DIR_N);

    // 13. direction bins North at 359
    compass_init(&c, 359.0f);
    T("direction: 359 = DIR_N");
        END(compass_direction(&c) == DIR_N);

    // 14. direction bins South
    compass_init(&c, 180.0f);
    T("direction: 180 = DIR_S");
        END(compass_direction(&c) == DIR_S);

    // 15. direction bins NE
    compass_init(&c, 45.0f);
    T("direction: 45 = DIR_NE");
        END(compass_direction(&c) == DIR_NE);

    // 16. tick arrives when target = current
    compass_init(&c, 90.0f);
    compass_set_target(&c, 90.0f);
    T("tick: already at target");
        END(compass_tick(&c, 1.0f) == 1);

    // 17. tick rotates toward target
    compass_init(&c, 0.0f);
    c.turn_rate = 90.0f;
    compass_set_target(&c, 180.0f);
    int arrived = compass_tick(&c, 1.0f);
    T("tick: rotates 90 in 1s");
        END(arrived == 0 && CLOSE(c.heading, 90.0f, 0.01f));

    // 18. tick arrives after enough time
    compass_init(&c, 0.0f);
    c.turn_rate = 180.0f;
    compass_set_target(&c, 90.0f);
    arrived = compass_tick(&c, 1.0f);
    T("tick: arrives at 90 after 1s");
        END(arrived == 1 && c.heading == 90.0f);

    // 19. facing checks tolerance
    compass_init(&c, 45.0f);
    T("facing: 45 within 5 of 50");
        END(compass_facing(&c, 50.0f, 5.0f));

    // 20. facing rejects out of tolerance
    T("facing: 45 not within 3 of 50");
        END(!compass_facing(&c, 50.0f, 3.0f));

    // 21. distance
    Vec2 a = {0, 0}, b = {3, 4};
    T("distance: (0,0)-(3,4) = 5");
        END(CLOSE(compass_distance(a, b), 5.0f, 0.001f));

    // 22. angle_between North to East
    a = (Vec2){0, 0}; b = (Vec2){100, 0};
    T("angle_between: N->E = 90");
        END(CLOSE(compass_angle_between(a, b), 90.0f, 0.1f));

    // 23. offset moves in heading direction
    compass_init(&c, 90.0f);
    c.position = (Vec2){0, 0};
    Vec2 off = compass_offset(&c, 100.0f);
    T("offset: East 100 -> x=100");
        END(off.x == 100 && off.y == 0);

    // 24. tick takes shortest path (CCW)
    compass_init(&c, 350.0f);
    c.turn_rate = 360.0f;
    compass_set_target(&c, 10.0f);
    compass_tick(&c, 1.0f);
    T("tick: shortest path 350->10 (CCW 20)");
        END(CLOSE(c.heading, 10.0f, 0.01f));

    // 25. set_target normalizes
    compass_init(&c, 0.0f);
    compass_set_target(&c, 400.0f);
    T("set_target: 400 -> 40");
        END(CLOSE(c.target_heading, 40.0f, 0.001f));

    printf("\n%d / %d tests passed\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
