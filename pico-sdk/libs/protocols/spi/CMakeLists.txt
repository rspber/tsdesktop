add_library(spi INTERFACE)

target_include_directories(spi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../../rp2040/spi)

target_sources(spi INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../../rp2040/spi/RP2040_TFT_SPI.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../../rp2040/spi/RP2040_TOUCH_SPI.cpp
)

target_link_libraries(spi INTERFACE
  hardware_spi
  tft
)
