cmake_minimum_required(VERSION 3.20)
project(tree_openmp C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)

add_executable(cpu_tree main.c)
target_link_libraries(cpu_tree PRIVATE OpenMP::OpenMP_C)
target_compile_options(cpu_tree PRIVATE -Wall -Wextra -Wpedantic)

add_executable(gpu_tree main.c)
target_link_libraries(gpu_tree PRIVATE OpenMP::OpenMP_C)
target_compile_options(gpu_tree PRIVATE -Wall -Wextra -Wpedantic)

target_compile_options(gpu_tree PRIVATE
  -fiopenmp
  -fopenmp-targets=spir64
)
target_link_options(gpu_tree PRIVATE
  -fiopenmp
  -fopenmp-targets=spir64
)
target_compile_definitions(gpu_tree PRIVATE OPENMP_TARGET_GPU=1)
