project(TetWrap)

cmake_minimum_required(VERSION 2.8)

add_library(tetwrap tetgen/predicates.cxx tetgen/tetgen.cxx tetwrap.cpp)
# we want to compile the library, not the standalone tetgen program
set_target_properties(tetwrap PROPERTIES COMPILE_FLAGS "-D\"TETLIBRARY\"")

add_executable(example main.cpp)
target_link_libraries(example tetwrap)