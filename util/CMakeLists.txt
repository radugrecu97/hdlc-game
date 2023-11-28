# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.12)


# Add the source files to the project
add_library(util util.c)

# Add the include directory to the project
target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(util yahdlc)