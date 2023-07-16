add_executable(canvas2
  canvas2.cpp
)

target_link_libraries(canvas2
  media
  tsdesktop
)

pico_enable_stdio_usb(canvas2 1)
pico_enable_stdio_uart(canvas2 0)

pico_add_extra_outputs(canvas2)
