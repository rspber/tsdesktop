add_executable(boing_ball
  boing_ball.cpp
)

target_link_libraries(boing_ball
  media
)

pico_enable_stdio_usb(boing_ball 1)
pico_enable_stdio_uart(boing_ball 0)

pico_add_extra_outputs(boing_ball)
