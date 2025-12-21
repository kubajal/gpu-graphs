
cmake_minimum_required(VERSION 3.22)

project(GPU-graph-test)

find_package(OpenMP REQUIRED)

add_executable(gpu_tree main.c)
target_link_libraries(gpu_tree PRIVATE OpenMP::OpenMP_C)
target_compile_options(gpu_tree PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(gpu_tree PRIVATE OPENMP_TARGET_GPU=1)

# OpenMP offload (Clang/ICX-style)
target_compile_options(gpu_tree PRIVATE
  -fiopenmp
  -fopenmp-targets=spir64
)
target_link_options(gpu_tree PRIVATE
  -fiopenmp
  -fopenmp-targets=spir64
)

# ---- Debug-friendly flags (host + device) ----
# Use generator expressions so this only applies for Debug builds.
target_compile_options(gpu_tree PRIVATE
  $<$<CONFIG:Debug>:-O0 -g -ggdb3 -fno-omit-frame-pointer -fno-inline>
  $<$<CONFIG:Debug>:-fno-lto>
)
target_link_options(gpu_tree PRIVATE
  $<$<CONFIG:Debug>:-fno-lto>
)

# Pass flags specifically to the *device compilation* (Clang offload).
# These are the most important ones for GPU variable visibility.
target_compile_options(gpu_tree PRIVATE
  $<$<CONFIG:Debug>:SHELL:-Xopenmp-target=spir64 -O0>
  $<$<CONFIG:Debug>:SHELL:-Xopenmp-target=spir64 -g>
  $<$<CONFIG:Debug>:SHELL:-Xopenmp-target=spir64 -fno-inline>
)

# If you had any global fast-math elsewhere, override it for Debug:
target_compile_options(gpu_tree PRIVATE
  $<$<CONFIG:Debug>:-fno-fast-math>
)
