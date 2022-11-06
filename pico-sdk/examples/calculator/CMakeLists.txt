add_executable(calculator
  calculator.cpp
)

target_link_libraries(calculator
  setup
  desktop
  xpt2046
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(calculator)

