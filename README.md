# Pikuma courses

## Introduction

My code and exercises as part of doing [Pikuma's](https://pikuma.com/) 3D graphics related courses. Currently, I'm going through [3D Graphics Programming from Scratch](https://courses.pikuma.com/courses/learn-computer-graphics-programming).

## Building and running

You'll need Visual Studio 2022 with the _Desktop development with C++_ workload and you'll also need to have [vcpkg](https://github.com/microsoft/vcpkg) installed on your system. Once set up, you only need to build the project and the dependencies will be downloaded and installed (the initial build may fail so just build again).

## Controls

Use the following controls to adjust the rendering.

* `1`: render a mesh wireframe with dots at the vertices.
* `2`: render a mesh wireframe.
* `3`: render a filled-triangle mesh.
* `4`: render a filled-triangle mesh with edges.
* `c`: enables backface culling.
* `d`: disables backface culling.
* `up arrow`: rotates the mesh.
* `down arrow`: rotates the mesh.
* `left arrow`: rotates the mesh.
* `right arrow`: rotates the mesh.

## Changes

The [3D Graphics Programming from Scratch](https://courses.pikuma.com/courses/learn-computer-graphics-programming) coursework is implemented in C on Pikuma, whereas I've chosen to implement the coursework in C++23.

## Credits

F22 mesh provided by [Gustavo Pezzi](https://github.com/gustavopezzi) from his [3D Graphics Programming from Scratch](https://courses.pikuma.com/courses/learn-computer-graphics-programming) course.

## Additional resources

* [Scratchapixel 3.0](https://scratchapixel.com/)
