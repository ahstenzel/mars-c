# simple
Simple example of using the raw C API to make a game. Creates an engine and an entity with
coordinates and a step event.

## Build
Use CMake to generate the build files: `cmake -S . -B build`. Then build the exe: 
`cmake --build build`. Builds are in Debug mode by default. To build in Release mode, use:
`cmake --build build --config Release`. This will create an executable in the `bin` folder and 
copy over the runtime library (specifically the x64 version).