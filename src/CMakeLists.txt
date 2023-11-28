# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.12)

add_library(gamelogic gamelogic.c gamelogic.h)

# Specify include directories for the library
target_include_directories(gamelogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gamelogic yahdlc)