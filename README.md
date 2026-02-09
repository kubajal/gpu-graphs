# GPU Pattern Match Stub (Ubuntu 22.04, clang, CMake, Ninja, mold, OpenCL, Intel Arc)

This repo is a minimal, reproducible scaffold:
- CSR-only tree representation
- bottom-up pattern matching stub
- OpenCL smoke test (verifies GPU execution path)

## Educational materials

1. [Run HPC Applications on CPUs & GPUs with Xe Architecture Using Intel® C++ & Intel® Fortran Compilers with OpenMP*
by Xianmin Tian](https://www.intel.com/content/www/us/en/developer/videos/run-hpc-apps-intel-c-fortran-compilers-with-openmp.html): video and slides unavailable elsewhere

# Prerequisities

This project was tested on Ubuntu 22.04, kernel version: 6.8.0-90-generic.

Firstly, see https://www.intel.com/content/www/us/en/docs/oneapi/installation-guide-linux/2023-0/apt.html and install all the requirements there (most importantly: `sudo apt install intel-basekit`).

Then, install:
```
sudo apt-get install linux-tools-6.8.0-90
sudo apt -y install cmake pkg-config build-essential
cd neo/
wget -q https://github.com/intel/intel-graphics-compiler/releases/download/igc-1.0.17537.20/intel-igc-core_1.0.17537.20_amd64.deb;
 wget -q https://github.com/intel/intel-graphics-compiler/releases/download/igc-1.0.17537.20/intel-igc-opencl_1.0.17537.20_amd64.deb;
 wget -q https://github.com/intel/compute-runtime/releases/download/24.35.30872.22/libigdgmm12_22.5.0_amd64.deb;
 wget -q https://github.com/intel/compute-runtime/releases/download/24.35.30872.22/intel-level-zero-gpu_1.3.30872.22_amd64.deb;
 wget -q https://github.com/intel/compute-runtime/releases/download/24.35.30872.22/intel-level-zero-gpu-legacy1_1.3.30872.22_amd64.deb;
 wget -q https://github.com/intel/compute-runtime/releases/download/24.35.30872.22/intel-opencl-icd_24.35.30872.22_amd64.deb;
 wget -q https://github.com/intel/compute-runtime/releases/download/24.35.30872.22/intel-opencl-icd-legacy1_24.35.30872.22_amd64.deb;
 dpkg -i ./*.deb || true;
 apt-get update;
 apt-get install -y -f;
```

#### VTune

If you want to use VTune, install Intel SEP drivers (requires `intel-basekit`):
```
$ cd /opt/intel/oneapi/vtune/latest/sepdk/src
$ ./build-driver -c gcc-12
$ ./boot-script -i
$ sudo ./insmod-sep -r
```

I couldn't make Ubuntu 22.04 and kernel version 6.8.0-90-generic collect the default CPU measurements because of this error:
```
vtune: Error: The following events cannot be collected: INST_RETIRED.ANY,CPU_CLK_UNHALTED.THREAD,CPU_CLK_UNHALTED.REF_TSC,CPU_CLK_UNHALTED.DISTRIBUTED. Consider removing the events from the collection, loading the VTune Profiler sampling driver using the root credentials, or updating the OS kernel.
```

Setting this helped, this disables the Linux `perf` tool for VTune and the error is gone but it narrows down what gets collected:
```
sudo sysctl -w dev.i915.perf_stream_paranoid=3
```

Then to partially compensate CPU stack collection use:
```
-knob enable-stack-collection=false -knob enable-tasks-stack-collection=true
```

So to sum up use this to collect performance statistics in VTune:
```
$ PYTHONPATH=<path>/gpu/bindings/python/src/ vtune -c gpu-offload -knob enable-stack-collection=false -knob enable-tasks-stack-collection=true ./test.py
```

# Build

## Release

```
/workspace$ mkdir -p build/Release
/workspace$ cd build/Release
/workspace/build/Release$ cmake ../.. -DCMAKE_BUILD_TYPE=Release
/workspace/build/Release$ cmake --build . -j 8
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
/workspace$ mkdir -p build/Debug
/workspace$ cd build/Debug
/workspace/build/Debug$ cmake ../.. -DCMAKE_BUILD_TYPE=Debug
/workspace/build/Debug$ cmake --build . -j 8
```

Run:

```
$ gdb-oneapi ./stub
```

## VSCode IntelliSense

 1. make sure you used `CMAKE_EXPORT_COMPILE_COMMANDS=1`
 2. disable MS C++ extensions's IntelliSense, ie. in .vscode/settings.json put `"C_Cpp.intelliSenseEngine": "disabled"`
 3. make sure that the path set in the `.clangd` file points to the correct `compile_commands.json`

# Run

## Quick Python test

Please note: build the C code in Release mode (refer to the [Release build](#release) section).

```
/workspace$ PYTHONPATH=`realpath ./bindings/python/src/` python3 test.py
```

## Debug C in a Python script

Please note: build the C code first in Debug mode (refer to the [Debug build](#debug) section).

```
/workspace$ PYTHONPATH=`realpath ./bindings/python/src/` gdb-oneapi python3
(gdb) set args test.py
(gdb) b src/algorithms.cpp:11
No symbol table is loaded.  Use the "file" command.
Make breakpoint pending on future shared library load? (y or [n]) y
Breakpoint 1 (src/algorithms.cpp:11) pending.
(gdb) r
Starting program: /usr/bin/python3 test.py
```
