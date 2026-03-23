# Simba

The Simple Budgeting App.

## Building
This project uses CMake to as it's (meta) build system.

You need **CMake 3.23 or newer**, **Ninja**, a **C++23** toolchain, and **Qt 6** (Core, Gui, Widgets). If CMake does not find Qt, set `CMAKE_PREFIX_PATH` to your Qt installation (see below).

### Presets

| Preset | CMake configuration | Role |
|--------|---------------------|------|
| `debug` | `Debug` | Development builds |
| `release-fast` | `Release` | Speed-optimized release |
| `release-small` | `MinSizeRel` | Size-optimized release |

List available presets:

```bash
cmake --list-presets
cmake --list-presets build
cmake --list-presets test
```

### Configure

From the repository root:

```bash
cmake --preset debug
# or
cmake --preset release-fast
cmake --preset release-small
```

This configures under `build/<preset-name>/` (for example `build/release-fast/`).

### Build

After configuring with a preset, build with the matching build preset:

```bash
cmake --build --preset debug
cmake --build --preset release-fast
cmake --build --preset release-small
```

### Tests

```bash
ctest --preset debug
ctest --preset release-fast
ctest --preset release-small
```

### Typical workflow

```bash
cmake --preset release-fast
cmake --build --preset release-fast
ctest --preset release-fast
```

### Qt location

If `find_package(Qt6)` fails, point CMake at your Qt prefix. One approach is to copy `CMakeUserPresets.json.example` to `CMakeUserPresets.json`, adjust the preset you use, and set `CMAKE_PREFIX_PATH` there. `CMakeUserPresets.json` is gitignored so local paths stay off the repo.
