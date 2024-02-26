add_executable(quadraball
  quadraball.cpp
)

target_link_libraries(quadraball
  media
)

pico_enable_stdio_usb(quadraball 1)
pico_enable_stdio_uart(quadraball 0)

pico_add_extra_outputs(quadraball)
