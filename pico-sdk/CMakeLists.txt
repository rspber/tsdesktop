cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include("pico_sdk_import.cmake")

project(tsdesktop C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_subdirectory(libs/env)
add_subdirectory(libs/tsdesktop)
add_subdirectory(libs/gfx)
add_subdirectory(libs/tft)
add_subdirectory(libs/media)
add_subdirectory(libs/protocols/spi)
add_subdirectory(libs/protocols/pio_spi)
add_subdirectory(libs/protocols/pio_8bitp)
add_subdirectory(libs/protocols/gpio_8bitp)
add_subdirectory(libs/devices/xpt2046)
add_subdirectory(libs/devices/spfd5408)
add_subdirectory(libs/devices/flash)
add_subdirectory(libs/devices/pcf8575)
add_subdirectory(libs/devices/ds3231)
add_subdirectory(libs/utils)
#add_subdirectory(libs/utils/snakes)

add_subdirectory(examples/calculator)
add_subdirectory(examples/checkboxes)
add_subdirectory(examples/programmer)
add_subdirectory(examples/scroll1)
add_subdirectory(examples/menu1)
add_subdirectory(examples/menu2)
add_subdirectory(examples/menu3)
add_subdirectory(examples/canvas1)
add_subdirectory(examples/canvas2)
add_subdirectory(examples/canvas3)
add_subdirectory(examples/ili9341_diag)
add_subdirectory(examples/quadraball)
add_subdirectory(examples/spider)
add_subdirectory(examples/spider2)
add_subdirectory(examples/idyll)
