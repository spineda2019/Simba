# Simba

The Simple Budgeting App.

## Building
This project uses CMake to as it's (meta) build system.

You need **CMake 3.23 or newer**, **Ninja** (if using the presets), a **C++23**
toolchain, and **Qt 6** (Core, Gui, Widgets). If CMake does not find Qt, set
`CMAKE_PREFIX_PATH` to your cmake configuration (see below).

### Presets
This project makes use of cmake presets in an attempt to standardize DX from
machine to machine. However, this is ultimately just cmake "sugar" that hides
the normal baseline cmake commands you can normally run. Both methods should
ultimately work.

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

For convenience, these presets:
* Configures under `build/<preset>` (e.g. `build/debug`).
* Uses Ninja
* Automatically set CMAKE_BUILD_TYPE
* Automatically set the installation dir to a subtree in the source tree

If for whatever reason you don't want to/can't operate under these assumptions
(maybe you hate Ninja, maybe you want to install to /usr/foo, or literally any
other reason) vanilla cmake will still work (although it will be a bit more
verbose)

```bash
cmake -B build/ReleaseSmall -DCMAKE_BUILD_TYPE=MinSizeRel -G Makefiles
```

### Build

After configuring with a preset, build with the matching build preset:

```bash
cmake --build --preset debug
cmake --build --preset release-fast
cmake --build --preset release-small
```

Or of course, if using raw cmake, you can pass the path to the configured
directory

```bash
cmake --build ./debug/ReleaseSmall
```

### Tests

```bash
ctest --preset debug
ctest --preset release-fast
ctest --preset release-small
```

### Install

After building, install the application to a clean directory. Each preset has
an `installDir` that defaults to `install/<preset-name>/`:

```bash
cmake --install build/debug
```

You can override the destination with `--prefix`:

```bash
cmake --install build/debug --prefix /some/other/path
```

### Typical workflow using the presets

```bash
cmake --preset release-fast
cmake --build --preset release-fast
ctest --preset release-fast
cmake --install build/release-fast
```

### Qt location

If `find_package(Qt6)` fails, point CMake at your Qt prefix by setting
`CMAKE_PREFIX_PATH`:

```bash
cmake --preset debug -DCMAKE_PREFIX_PATH=~/Qt/6.8.0/gcc_64/
```

Alternatively, create a `CMakeUserPresets.json` to avoid passing the flag every
time (see [User presets](#user-presets) below). `CMakeUserPresets.json` is
gitignored so local paths stay off the repo.

#### Linux: Qt SDK vs system Qt

On Linux, the Qt deploy step during `cmake --install` requires a Qt **SDK**
installation (from the [official installer](https://www.qt.io/download) or
[aqtinstall](https://github.com/miurahr/aqtinstall)). Distribution-packaged Qt
(e.g. Arch's `qt6-base`) does not include RPATH entries in its shared libraries,
which causes the deploy step to fail with an RPATH error.

To resolve this, configure with `CMAKE_PREFIX_PATH` pointing to a Qt SDK
installation, or use a user preset that sets it for you. If you only use
system-packaged Qt and don't need bundled Qt libraries, or are just building
and running locally without intending to install, you can ignore this. This only
applies when trying to install with `cmake --install ...`

### User presets

`CMakeUserPresets.json` is gitignored and can hold machine-specific settings
such as `CMAKE_PREFIX_PATH`. Create one with presets that inherit from the repo
presets:

```json
{
  "version": 3,
  "cmakeMinimumRequired": { "major": 3, "minor": 23, "patch": 0 },
  "configurePresets": [
    {
      "name": "user-base",
      "hidden": true,
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "~/Qt/6.8.0/gcc_64/"
      }
    },
    { "name": "sdk-debug", "inherits": ["debug", "user-base"] },
    { "name": "sdk-fast",  "inherits": ["fast",  "user-base"] },
    { "name": "sdk-small", "inherits": ["small", "user-base"] }
  ],
  "buildPresets": [
    { "name": "sdk-debug", "configurePreset": "sdk-debug" },
    { "name": "sdk-fast",  "configurePreset": "sdk-fast" },
    { "name": "sdk-small", "configurePreset": "sdk-small" }
  ],
  "testPresets": [
    { "name": "sdk-debug", "configurePreset": "sdk-debug" },
    { "name": "sdk-fast",  "configurePreset": "sdk-fast" },
    { "name": "sdk-small", "configurePreset": "sdk-small" }
  ]
}
```

Adjust `CMAKE_PREFIX_PATH` to match your local Qt SDK path, then use the
`sdk-*` presets:

```bash
cmake --preset sdk-debug
cmake --build --preset sdk-debug
ctest --preset sdk-debug
cmake --install build/sdk-debug
```
