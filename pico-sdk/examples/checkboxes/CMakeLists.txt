add_executable(checkboxes
  checkboxes.cpp
)

target_link_libraries(checkboxes
  setup
  display
  gfx
  desktop
  xpt2046
  pcf8575
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(checkboxes)
