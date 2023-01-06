add_executable(programmer
  Pgms.cpp

  programmer.cpp
)

target_link_libraries(programmer
  media
  tsdesktop
  ds3231
  pcf8575
  flash
)

pico_enable_stdio_usb(programmer 1)
pico_enable_stdio_uart(programmer 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(programmer)

