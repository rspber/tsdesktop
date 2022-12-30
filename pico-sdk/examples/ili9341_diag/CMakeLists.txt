add_executable(ili9341_diag
  ili9341_diag.cpp
)

target_link_libraries(ili9341_diag
  setup
  xpt2046
)

pico_enable_stdio_usb(ili9341_diag 1)
pico_enable_stdio_uart(ili9341_diag 0)

pico_add_extra_outputs(ili9341_diag)
