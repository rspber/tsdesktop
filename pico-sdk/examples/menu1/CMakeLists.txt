add_executable(menu1
  menu1.cpp
)

target_link_libraries(menu1
  setup
  desktop
  xpt2046
)

pico_add_extra_outputs(menu1)
