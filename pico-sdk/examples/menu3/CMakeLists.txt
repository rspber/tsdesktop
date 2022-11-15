add_executable(menu3
  menu3.cpp
)

target_link_libraries(menu3
  setup
  desktop
  xpt2046
)

pico_add_extra_outputs(menu3)
