add_executable(programmer
  Pgms.cpp

  programmer.cpp
)

target_link_libraries(programmer
  setup
  display
  gfx
  desktop
  xpt2046
  ds3231
  pcf8575
  flash
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(programmer)

