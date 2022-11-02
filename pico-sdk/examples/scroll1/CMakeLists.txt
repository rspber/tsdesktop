add_executable(scroll1
  scroll1.cpp
)

target_link_libraries(scroll1
  setup
  display
  desktop
  xpt2046
)

pico_add_extra_outputs(scroll1)
