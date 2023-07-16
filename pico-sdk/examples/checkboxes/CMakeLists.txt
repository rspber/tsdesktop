add_executable(checkboxes
  checkboxes.cpp
)

target_link_libraries(checkboxes
  media
  tsdesktop
  pcf8575
)

pico_enable_stdio_usb(checkboxes 1)
pico_enable_stdio_uart(checkboxes 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(checkboxes)
