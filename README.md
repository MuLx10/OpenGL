# OpenGL
Repository containing OpenGL examples implementation.

## Deps
- CMake >= 3
- g++ (MinGW in Windows)
- GLFW      (present in `deps` extract the zip)
- GLEW      (present in `deps` extract the zip)
- GLM       (present in `deps` extract the zip)
- STB_IMAGE (present in `deps` extract the zip)
## Build

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

#### Individual build
```bash
$ make {executable} # eg. make triangle
```


## Run

```bash
$ cd src/{project}/ # $ cd src/01-triangle/
$ {executable} # $./triangle
```

**Supported executable**
- triangle
- texture
- multi-texture
- controls
- 3d-model
