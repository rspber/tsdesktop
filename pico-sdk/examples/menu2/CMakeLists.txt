add_executable(menu2
  menu2.cpp
)

target_link_libraries(menu2
  setup
  display
  gfx
  desktop
  xpt2046
)

pico_add_extra_outputs(menu2)
