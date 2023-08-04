# tools

Various tools I use in some projects.

## Modules

- sdl : Multiple SDL wrappers
- utils : Random stuff
- waveform : Classes to generate waveforms samples easily

## Build

Install [cmake](https://cmake.org/download/) and [vcpkg](https://vcpkg.io/en/getting-started.html).

Set the `VCPKG_ROOT` environment variable to the root of `vcpkg` installation directory.

Configure :

    # preset = releasewin on Windows
    cmake --preset=release

Build :

    cmake --build build/release(win)
