# GPU Pattern Match Stub (Ubuntu 22.04, clang, CMake, Ninja, mold, OpenCL, Intel Arc)

This repo is a minimal, reproducible scaffold:
- CSR-only tree representation
- bottom-up pattern matching stub
- OpenCL smoke test (verifies GPU execution path)

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

## Build (Release)

```
cmake -S . -B build -G Ninja \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build --parallel
```

Run:

```
./build/stub
```

## Build (Debug)

```
cmake -S . -B build_dbg -G Ninja \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_SANITIZERS=ON

cmake --build build_dbg --parallel
```

Run:

```
./build_dbg/stub
```