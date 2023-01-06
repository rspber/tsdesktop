add_executable(menu2
  menu2.cpp
)

target_link_libraries(menu2
  media
  tsdesktop
)

pico_enable_stdio_usb(menu2 1)
pico_enable_stdio_uart(menu2 0)

pico_add_extra_outputs(menu2)
