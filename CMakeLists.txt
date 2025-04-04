cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(glitcher)
pico_sdk_init()

add_executable(glitcher main.c)

# Agregar archivo PIO
pico_generate_pio_header(glitcher glitch.pio)

# Enlazar con bibliotecas necesarias
target_link_libraries(glitcher pico_stdlib hardware_gpio hardware_clocks hardware_pio hardware_sync hardware_irq)

pico_add_extra_outputs(glitcher)
