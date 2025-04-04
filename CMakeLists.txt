cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(glitcher C CXX ASM)
pico_sdk_init()

# Directorio de salida para los archivos compilados
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Compilados)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Compilados)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Compilados)

# Archivo main
add_executable(glitcher main.c)

# Agregar archivo PIO
pico_generate_pio_header(glitcher ${CMAKE_CURRENT_LIST_DIR}/glitch.pio)

# Enlazar con bibliotecas necesarias
target_link_libraries(glitcher pico_stdlib hardware_pio hardware_clocks hardware_gpio hardware_sync hardware_irq)

# Genera los archivos (.uf2, .hex, etc.)
pico_add_extra_outputs(glitcher)
