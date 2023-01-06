add_library(setup INTERFACE)

target_include_directories(setup INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(setup INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/Picoino.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Touch.cpp
  ${CMAKE_CURRENT_LIST_DIR}/RP2040_TFT_SPI.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Wire.cpp
)

target_link_libraries(setup INTERFACE
  pico_stdlib
  hardware_spi
  hardware_i2c
  tft
)
