cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(glitcher)
pico_sdk_init()

add_executable(glitcher main.c)
target_link_libraries(glitcher pico_stdlib hardware_gpio hardware_clocks)
pico_add_extra_outputs(glitcher)

