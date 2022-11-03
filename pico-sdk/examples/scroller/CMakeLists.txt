add_executable(scroller
  scroller.cpp
)

target_link_libraries(scroller
  setup
  display
  gfx
  desktop
  xpt2046
)

pico_add_extra_outputs(scroller)
