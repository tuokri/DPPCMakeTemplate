cmake_minimum_required(VERSION 3.29)
project(DPPCMakeTemplate)

set(CMAKE_CXX_STANDARD 20)

# For cmake_print_variables.
include(CMakePrintHelpers)

# Print MSVC version to help debug potential issues.
if (MSVC)
    cmake_print_variables(MSVC_VERSION)
endif ()

# To make things simpler, instruct .exe, .dll, and .pdb files to be placed
# in the same directory.
if (WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    cmake_print_variables(CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    cmake_print_variables(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    cmake_print_variables(CMAKE_PDB_OUTPUT_DIRECTORY)
endif ()

include(cmake/dpp.cmake)

add_subdirectory(src)
