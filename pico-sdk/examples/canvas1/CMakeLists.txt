add_executable(canvas1
  canvas1.cpp
)

target_link_libraries(canvas1
  setup
  desktop
  xpt2046
)

pico_add_extra_outputs(canvas1)
