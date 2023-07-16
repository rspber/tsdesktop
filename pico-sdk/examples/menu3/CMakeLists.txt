add_executable(menu3
  menu3.cpp
)

target_link_libraries(menu3
  media
  tsdesktop
)

pico_enable_stdio_usb(menu3 1)
pico_enable_stdio_uart(menu3 0)

pico_add_extra_outputs(menu3)
