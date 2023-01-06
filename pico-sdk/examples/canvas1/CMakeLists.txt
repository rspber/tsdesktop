add_executable(canvas1
  canvas1.cpp
)

target_link_libraries(canvas1
  media
  tsdesktop
)

pico_enable_stdio_usb(canvas1 1)
pico_enable_stdio_uart(canvas1 0)

pico_add_extra_outputs(canvas1)
