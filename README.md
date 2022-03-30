## Build instructions

### Windows

This project uses CMake as its build system, theoretically making it possible
to build this codebase on any major operating system. There is supposedly a way
to get Visual Studio to interoperate with CMake-based projects but I don't
personally do that so I can't vouch for how straightforward it is. In lieu of
that, here's what I do on Windows. This sets up a Linux-like environment that
builds natively to Windows.

1. Install [MSYS2](https://www.msys2.org/)
1. Install MinGW GCC (part of MSYS2 setup): `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`
1. Install additional development dependencies: `pacman -S git mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja`
1. Install actual dependencies: `pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image ...`
1. `git clone` this repo and `cd` into it
1. Pull down third-party CMake modules: `git submodule update --init`
1. Generate build directory: `cmake -B build -DCMAKE_BUILD_TYPE=Release`
1. Compile and link: `cmake --build build`
1. Execute program: `build/MyGame.exe`

### Web

(Tested on Linux)

1. Install [Emscripten](https://emscripten.org/docs/getting_started/downloads.html)
1. Install additional development dependences: cmake, make, git, python3
1. `git clone` this repo and `cd` into it
1. Pull down third-party CMake modules: `git submodule update --init`
1. Generate build directory: `emcmake cmake -B build`
1. Compile and link: `emmake cmake --build build`
1. `cd build`
1. Run a local server: `python3 -m http.server`
1. Visit the served page in your browser e.g. `http://localhost:8000`

### Troubleshooting

- If CMake is having trouble finding SDL2 libraries, make sure that:
  - SDL2 development libraries are installed on your machine
  - `git submodule update --init` to get the CMake modules for finding SDL2 libraries
