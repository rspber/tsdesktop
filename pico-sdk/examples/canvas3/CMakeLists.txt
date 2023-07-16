add_executable(canvas3
  canvas3.cpp
)

target_link_libraries(canvas3
  media
  tsdesktop
)

pico_enable_stdio_usb(canvas3 1)
pico_enable_stdio_uart(canvas3 0)

pico_add_extra_outputs(canvas3)
