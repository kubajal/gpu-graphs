
cmake_minimum_required(VERSION 3.22)

project(GPU-graph-test)

find_package(OpenMP REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_library(arboqllib SHARED src/algorithms.c src/graph.c src/logger.c)
target_include_directories(arboqllib
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(arboqllib PUBLIC OpenMP::OpenMP_C)

set(_PY_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/src/arboqllib/.lib")

add_custom_target(copy_arboqllib ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory "${_PY_LIB_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "$<TARGET_FILE:arboqllib>"
          "${_PY_LIB_DIR}/$<TARGET_FILE_NAME:arboqllib>"
  DEPENDS arboqllib
  VERBATIM
)

target_link_directories(arboqllib PRIVATE /opt/intel/oneapi/compiler/latest/lib)

target_link_libraries(arboqllib PRIVATE
  omptarget
  omptarget.rtl.level0
)

# Make it work without env vars
target_link_options(arboqllib PRIVATE "LINKER:-rpath,/opt/intel/oneapi/compiler/latest/lib")
