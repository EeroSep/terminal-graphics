# Usage:

## Dependencies

notcurses:
```
sudo apt-get install build-essential cmake doctest-dev libavdevice-dev libdeflate-dev libgpm-dev libncurses-dev libqrcodegen-dev libswscale-dev libunistring-dev pandoc pkg-config
```

## Build

```
cmake -S . -B build
cmake --build build
```

## Run

Run from the repository root — model paths (e.g. `assets/pyramid.obj`) are loaded relative to the current working directory:

```
./build/terminal_graphics
```

There is no in-app quit key yet; exit with Ctrl+C.

## Controls

- Mouse drag: look around
- Arrow keys: look around
- `w` / `a` / `s` / `d`: move forward / left / back / right

# To-do:
- backface culling
- normals
- materials
- multithreaading
- UI?
- double buffering