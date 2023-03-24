# Learn OpenGL

这个仓库以及[ Dusk 引擎](https://github.com/DuskNgai/Dusk)包含了我学习 `OpenGL` 的过程中的大部分代码，主要是一些 `OpenGL` 的基础知识，以及一些常用的 `OpenGL` 使用方法，大部分的笔记存储于 Dusk 引擎中。这个项目还有一个作用是测试初代 Dusk 引擎。

## Requirement & Usage

- 一个支持 `C++17` 标准的编译器。我们在以下平台 + 编译器的组合上测试了我们的程序：
    - `Windows` + `Visual Studio 2022`
    - `Ubuntu 20.04/22.04` + `GCC 11.3/Clang 13.0.0`

请按照以下的流程编译程序：

```bash
git clone https://github.com/DuskNgai/Learn-OpenGL
cd Learn-OpenGL
git submodule sync --recursive
git submodule update --init --recursive

mkdir build
cd build
cmake ..
###### If you are using Visual Studio #####
# Double click `LearnOpenGL.sln`
# set `learngl` as starting project
# make and run.
###### End if you are using Visual Studio #####

###### If you are using GCC/Clang #####
make -j
###### End if you are using GCC/Clang #####
```

## Todo List

目前进度：

- [x] Getting Started
    - [x] OpenGL
    - [x] Creating a Window
    - [x] Hello Window
    - [x] Hello Triangle
    - [x] Shaders
    - [x] Textures
    - [x] Transformations
    - [x] Coordinate System
    - [x] Camera
- [ ] Lighting
    - [x] Colors
    - [x] Basic Lighting
    - [x] Materials
    - [x] Lighting Maps
    - [ ] Light Casters
    - [ ] Multiple Lights
- [ ] Advanced OpenGL
    - [ ] Depth Testing
    - [ ] Stencil Testing
    - [ ] Blending
    - [ ] Framebuffers
