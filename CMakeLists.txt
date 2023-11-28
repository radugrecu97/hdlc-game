cmake_minimum_required(VERSION 3.12)
project(hdlc-game VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_subdirectory(lib/yahdlc)
add_subdirectory(util)
add_subdirectory(src)

add_executable(hdlc-game main.c)

# Include directories
target_include_directories(hdlc-game PUBLIC src util lib/yahdlc)

# Link libraries
target_link_libraries(hdlc-game gamelogic util yahdlc)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)