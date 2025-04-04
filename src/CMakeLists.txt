add_executable(
    DPPExampleBot
    main.cpp
)

target_compile_features(
    DPPExampleBot
    PRIVATE
    cxx_std_20
)

# DCT_COMPILE_OPTIONS is defined in CMakePresets.json.
# (DCT prefix stands for DPP CMake Template).
target_compile_options(
    DPPExampleBot
    PRIVATE
    ${DCT_COMPILE_OPTIONS}
)

# DCT_LINK_OPTIONS is defined in CMakePresets.json.
target_link_options(
    DPPExampleBot
    PRIVATE
    ${DCT_LINK_OPTIONS}
)

target_include_directories(
    DPPExampleBot
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include/
)

set(
    DPP_EXAMPLE_BOT_LIBS
    dpp
    spdlog::spdlog
)

target_link_libraries(
    DPPExampleBot
    PRIVATE
    ${DPP_EXAMPLE_BOT_LIBS}
)

target_compile_definitions(
    DPPExampleBot
    PUBLIC
    DPP_CORO
)
