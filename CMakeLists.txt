
cmake_minimum_required(VERSION 3.22)

project(GPU-graph-test)

find_package(OpenMP REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_library(gpu_graphlib SHARED src/algorithms.c src/graph.c src/logger.c)
target_include_directories(gpu_graphlib
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(gpu_graphlib PUBLIC OpenMP::OpenMP_C)
target_compile_options(gpu_graphlib PUBLIC
  -fiopenmp
  -fopenmp-targets=spir64
)

set(_PY_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/src/gpu_graphlib/.lib")

add_custom_command(TARGET gpu_graphlib POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${_PY_LIB_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "$<TARGET_FILE:gpu_graphlib>"
          "${_PY_LIB_DIR}/$<TARGET_FILE_NAME:gpu_graphlib>"
)