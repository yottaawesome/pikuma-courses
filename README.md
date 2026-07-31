# Pikuma courses

## Introduction

My code and exercises as part of doing [Pikuma's](https://pikuma.com/) 3D graphics and games related courses.

## Building and running

You'll need Visual Studio 2026 with the _Desktop development with C++_ workload and you'll also need to have [vcpkg](https://github.com/microsoft/vcpkg) installed on your system. Once set up, you only need to build the project and the dependencies will be downloaded and installed (the initial build may fail so just build again).

## Changes

The coursework for the 3D Graphics Programming course is implemented in C on Pikuma, whereas I've chosen to implement my coursework in C++23, as C is cumbersome, error-prone and primitive to work with. The 2D Game Engine course is done in C++17, but I've likewise elected to use C++23, mainly for use of C++ modules, aggregate initialisation, and other convenient features. I work on Windows and don't have easy access to a Linux or Mac device, so I haven't bothered making anything cross-platform. I use Visual Studio 2026, with `vcpkg` to manage dependencies such as sdl (this means I don't package any of the dependencies), and I've used newer versions of said dependencies. This required updating some of the course code that worked with the dated dependencies. 

## Additional resources

* [Scratchapixel 3.0](https://scratchapixel.com/)
