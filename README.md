# tools

Various tools I use in some projects.

## Modules

- sdl : Multiple SDL wrappers
- utils : Random stuff
- waveform : Classes to generate waveforms samples easily

## Build

    # configure
    cmake --preset=rel

    # build
    cmake --build --preset=rel

    # test
    ctest --preset=rel
    ctest --preset=rel -R <regex> # filter by regex
    ctest --preset=rel -V # print output of every tests
    ctest --preset=rel --output-on-failure # print output of failing tests
