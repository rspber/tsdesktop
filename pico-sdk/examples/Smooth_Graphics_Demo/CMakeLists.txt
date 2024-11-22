add_executable(Smooth_Graphics_Demo Smooth_Graphics_Demo.cpp
)

target_link_libraries(Smooth_Graphics_Demo
  media
)

pico_enable_stdio_usb(Smooth_Graphics_Demo 1)
pico_enable_stdio_uart(Smooth_Graphics_Demo 0)

pico_add_extra_outputs(Smooth_Graphics_Demo)
