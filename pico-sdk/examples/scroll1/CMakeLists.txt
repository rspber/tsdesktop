add_executable(scroll1
  scroll1.cpp
)

target_link_libraries(scroll1
  media
  tsdesktop
)

pico_enable_stdio_usb(scroll1 1)
pico_enable_stdio_uart(scroll1 0)

pico_add_extra_outputs(scroll1)
