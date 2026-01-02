# gdf
## Gus Dwarf Fortress

A Dwarf Fortress clone in C and Raylib.

I was playing a lot of Dwarf Fortress and kept thinking it would be really fun to implement the mechanics, so I'm putting this clone together in C.

## Structure 

- `doc/`: Miscellaneous thoughts and documentation
- `frontend/`: The part that uses Raylib to put the game on screen
- `gdf_int/`: The gdf internals, the entire simulation is in here. Ideally, this can be compiled and run on its own with whatever parameters and state. It knows nothing about the player or the way it's being presented
- `raylib/`: Platform/graphics/audio library (https://github.com/raysan5/raylib)
- `resources/`: All the data, images, and sound for the game
- `tomlc17/`: A TOML parser (https://github.com/cktan/tomlc17)

## Building

gdf is built with CMake, so the following command will generate your platform's build files into a folder called `build`:

`cmake -S . -B build/`

**NOTE:** The code is platform independent but the build flags I set are Linux/GCC specific at the time of writing. You can comment them out and it should work fine on Windows or macOS.
