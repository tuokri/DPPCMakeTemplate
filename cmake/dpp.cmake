# These options are used to customize how D++ is built.
# Check submodules\dpp\CMakeLists.txt for documentation.
set(DPP_INSTALL OFF)
set(BUILD_VOICE_SUPPORT ON)
set(RUN_LDCONFIG OFF)
set(DPP_BUILD_TEST OFF)
set(DPP_NO_VCPKG ON)
set(DPP_CORO ON)
set(DPP_FORMATTERS ON)
set(DPP_NO_CONAN ON)
set(DPP_USE_PCH ON)
set(BUILD_SHARED_LIBS ON)

add_subdirectory("${CMAKE_SOURCE_DIR}/submodules/dpp")

# Silence compiler warnings from D++ as we have no control over them.
# If you would like to see them, remove the following block of code.
get_target_property(DPP_INCLUDE_DIRS dpp INTERFACE_INCLUDE_DIRECTORIES)
set_property(
    TARGET
    dpp
    APPEND
    PROPERTY
    INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
    ${DPP_INCLUDE_DIRS}
)
