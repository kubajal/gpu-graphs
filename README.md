# GPU Pattern Match Stub (Ubuntu 22.04, clang, CMake, Ninja, mold, OpenCL, Intel Arc)

This repo is a minimal, reproducible scaffold:
- CSR-only tree representation
- bottom-up pattern matching stub
- OpenCL smoke test (verifies GPU execution path)

## Educational materials

1. [Run HPC Applications on CPUs & GPUs with Xe Architecture Using Intel® C++ & Intel® Fortran Compilers with OpenMP*
by Xianmin Tian](https://www.intel.com/content/www/us/en/developer/videos/run-hpc-apps-intel-c-fortran-compilers-with-openmp.html): video and slides unavailable elsewhere

# Prerequisities

## Host requirements (important for Intel Arc A770)
The Intel kernel driver and `/dev/dri` live on the host, not inside Docker.
To access the GPU from the container you must pass `/dev/dri` through.

## Build the Docker image
From this directory:

```bash
docker build -t arc-opencl-dev .
```

## Run the container (with GPU access)

44 = `video` group
110 = `render` group

```
docker run -d \
  --name arc-opencl-dev \
  --device=/dev/dri \
  --group-add 110 \
  --group-add 44 \
  -v "$(pwd)":/workspace \
  --workdir /workspace \
  --restart unless-stopped \
  arc-opencl-dev \
  sleep infinity
```

Inside the container, check OpenCL visibility:

```
clinfo | sed -n '1,120p'
```

You should see an Intel platform/device. If not, it's usually a host permission issue
(you may need your host user in the render group).

# Build

## Release

```
$ cd build/Release
$ cmake ../.. -DCMAKE_BUILD_TYPE=Release
$ cmake --build . -j 8
```

Run:

```
$ ./stub
```

## Debug

Debugging prerequisities: 
 1. https://www.intel.com/content/www/us/en/docs/distribution-for-gdb/get-started-guide-linux/2025-2/overview.html
 2. https://dgpu-docs.intel.com/driver/installation.html

```
$ cd build/Release
$ cmake ../.. -DCMAKE_BUILD_TYPE=Debug
$ cmake --build . -j 8
```

Run:

```
$ gdb-oneapi ./stub
```

## VSCode IntelliSense

 1. make sure you used `CMAKE_EXPORT_COMPILE_COMMANDS=1`
 2. disable MS C++ extensions's IntelliSense, ie. in .vscode/settings.json put `"C_Cpp.intelliSenseEngine": "disabled"`
 3. make sure that the path set in the `.clangd` file points to the correct `compile_commands.json`

## Build Python bindings

```
# in project root folder
$ python3 -m pip install --user -U build
$ cmake --install build/Release/ --prefix /workspace/bindings/python/src
$ python3 -m pip install -e bindings/python
```

# Build Python bindings

Run Python test:

```
/workspace$ cmake --build ./build/Release/ \
  && python3 -m pip install . \
  && PYTHONPATH=./build/Release/ python3 test.py
```