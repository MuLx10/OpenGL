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
$ build/bin/{executable} # eg. build/bin/triangle
```

**Supported executable**
- triangle
- texture
- multi-texture

