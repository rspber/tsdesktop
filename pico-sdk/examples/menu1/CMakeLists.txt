add_executable(menu1
  menu1.cpp
)

target_link_libraries(menu1
  setup
  desktop
  xpt2046
)

pico_enable_stdio_usb(menu1 1)
pico_enable_stdio_uart(menu1 0)

pico_add_extra_outputs(menu1)
