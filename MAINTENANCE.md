# MAINTENANCE.md

## Overview

Single-file C library (compass.h + compass.c). No external dependencies beyond `<math.h>` for trig.

## Code Style

- C11 strict (`-std=c11 -Wall -Wextra -Wpedantic`)
- All public API in compass.h, implementation in compass.c
- Static helpers prefixed with no prefix (internal only)
- Float math throughout (no double)

## Testing

```sh
make test
```

Tests live in test_compass.c. Add new `T("name")` / `END(condition)` blocks. Run after any change.

## Architecture Decisions

- **Fixed-point forward vectors (×10000)**: Avoids floats in Vec2 for deterministic game/entity logic. Trade-off: ~0.01% error.
- **Normalized to 0-360**: All headings stored normalized. `norm()` helper handles wrapping.
- **Shortest-path rotation**: `compass_diff` and `compass_tick` always take the shortest angular path.
- **No heap**: No malloc/free. All state lives in the Compass struct passed by pointer.

## Things to Watch

- `compass_forward` truncates to int — precision loss at high distances in `compass_offset`
- No handling of NaN/Inf inputs
- Thread safety is caller's responsibility (no locking)
- `compass_distance` returns float from int inputs — fine for comparisons, lossy for exact distances

## Adding Features

- New functions: add declaration to compass.h, implementation to compass.c, tests to test_compass.c
- New fields to Compass struct: update compass_init to zero them
- Keep the "no malloc" rule — stack allocation only
