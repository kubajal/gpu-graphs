
cmake_minimum_required(VERSION 3.22)

project(GPU-graph-test)

find_package(OpenMP REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable(gpu_tree main.c)
target_link_libraries(gpu_tree PRIVATE OpenMP::OpenMP_C)
target_compile_options(gpu_tree PUBLIC
  -fiopenmp
  -fopenmp-targets=spir64
)
