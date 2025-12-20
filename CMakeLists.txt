cmake_minimum_required(VERSION 3.20)
project(tree_openmp C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)

add_executable(tree_cpu main.c)
target_link_libraries(tree_cpu PRIVATE OpenMP::OpenMP_C)
target_compile_options(tree_cpu PRIVATE -Wall -Wextra -Wpedantic)

add_executable(tree_gpu main.c)
target_link_libraries(tree_gpu PRIVATE OpenMP::OpenMP_C)
target_compile_options(tree_gpu PRIVATE -Wall -Wextra -Wpedantic)

target_compile_options(tree_gpu PRIVATE
  -fiopenmp
  -fopenmp-targets=spir64
)
target_link_options(tree_gpu PRIVATE
  -fiopenmp
  -fopenmp-targets=spir64
)
target_compile_definitions(tree_gpu PRIVATE OPENMP_TARGET_GPU=1)
