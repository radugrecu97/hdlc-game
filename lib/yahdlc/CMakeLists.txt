# lib/yahdlc/C/CMakeLists.txt

# Create a library from the source files
add_library(yahdlc STATIC
    yahdlc.c
    fcs16.c
)

# Include the header files
target_include_directories(yahdlc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)