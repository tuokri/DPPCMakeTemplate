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
# in the same directory. Also get rid of Visual Studio generator's default
# Debug and Release directories to make sure all of our executable and library
# files get placed in the same directory.
if (MSVC)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    cmake_print_variables(CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    cmake_print_variables(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    cmake_print_variables(CMAKE_PDB_OUTPUT_DIRECTORY)
    cmake_print_variables(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)

    foreach (CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE} ${CMAKE_BINARY_DIR})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE} ${CMAKE_BINARY_DIR})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE} ${CMAKE_BINARY_DIR})
    endforeach ()
endif ()

include(cmake/dpp.cmake)

# Use find_package to find any dependencies installed by vcpkg.
find_package(
    spdlog
    CONFIG
    REQUIRED
)

add_subdirectory(src)
