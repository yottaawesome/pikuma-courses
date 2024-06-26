# Pikuma courses

## Introduction

My code and exercises as part of doing [Pikuma's](https://pikuma.com/) 3D graphics related courses. Currently, I'm going through [3D Graphics Programming from Scratch](https://courses.pikuma.com/courses/learn-computer-graphics-programming).

## Building and running

You'll need Visual Studio 2022 with the _Desktop development with C++_ workload and you'll also need to have [vcpkg](https://github.com/microsoft/vcpkg) installed on your system. Once set up, you only need to build the project and the dependencies will be downloaded and installed (the initial build may fail so just build again).

## Changes

The [3D Graphics Programming from Scratch](https://courses.pikuma.com/courses/learn-computer-graphics-programming) course is implemented with pure C on Pikuma. I've instead chosen to implement this project in C++23 and using C++'s idioms and library features, but I may do a C implementation at some point just to compare and contrast.

I've added and implemented a small custom unit testing framework in the `unit_tests` project, because the existing MS unit testing framework is incompatible with standard library module imports due to compiler bugs.

## Additional resources

* [Scratchapixel 3.0](https://scratchapixel.com/)
