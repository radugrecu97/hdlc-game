cmake_minimum_required(VERSION 3.0.0)
project(hdlc-game VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_subdirectory(lib/yahdlc)  # Assuming the CMakeLists.txt for yahdlc is in lib/yahdlc

add_executable(hdlc-game main.c)

target_include_directories(hdlc-game PRIVATE lib/yahdlc)
target_link_libraries(hdlc-game PRIVATE yahdlc)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
