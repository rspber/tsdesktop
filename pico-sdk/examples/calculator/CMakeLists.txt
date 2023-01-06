add_executable(calculator
  calculator.cpp
)

target_link_libraries(calculator
  media
  tsdesktop
  utils
)

pico_enable_stdio_usb(calculator 1)
pico_enable_stdio_uart(calculator 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(calculator)

