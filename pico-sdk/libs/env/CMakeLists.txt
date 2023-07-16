add_library(env INTERFACE)

target_include_directories(env INTERFACE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../setup)

target_sources(env INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/Picoino.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Wire.cpp
  ${CMAKE_CURRENT_LIST_DIR}/avr/dtostrf.cpp
)

target_link_libraries(env INTERFACE
  pico_stdlib
  hardware_i2c
  hardware_gpio
  hardware_adc
  spi
  pio_spi
  gpio_8bitp
  pio_8bitp
#  snakes
)
