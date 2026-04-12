# flux-compass-c

Pure C11 orientation, heading, and direction management. Zero dependencies. No malloc.

## Build

```sh
make          # libcompass.a + test_compass
make test     # run tests
make clean
```

## Usage

```c
#include "compass.h"

Compass c;
compass_init(&c, 0.0f);          // face North
compass_set_target(&c, 90.0f);    // turn toward East

while (!compass_tick(&c, dt)) {
    // still turning...
}
// now facing East

Direction d = compass_direction(&c);  // DIR_E
Vec2 fwd = compass_forward(45.0f);    // NE unit vector (×10000)
```

## API

| Function | Description |
|---|---|
| `compass_init` | Initialize with heading (degrees, 0=North) |
| `compass_set_heading` | Set heading directly |
| `compass_set_target` | Set turn target |
| `compass_tick` | Step rotation toward target. Returns 1 if arrived |
| `compass_diff` | Shortest angular difference (-180 to 180) |
| `compass_direction` | 8-point cardinal direction from heading |
| `compass_forward` | Unit vector (×10000 as fixed-point) for heading |
| `compass_facing` | Check if within tolerance of a heading |
| `compass_offset` | Position offset by distance along heading |
| `compass_distance` | Euclidean distance between two Vec2s |
| `compass_angle_between` | Compass bearing from one Vec2 to another (0-360) |

## Conventions

- **Heading**: 0° = North, 90° = East, 180° = South, 270° = West
- **Vec2 forward**: scaled ×10000 (fixed-point approximation of unit vector)
- **Turn rate**: degrees per second (configurable via `c.turn_rate`)
- **Angular diff**: shortest path, negative = clockwise from→to

## License

Public domain / CC0
