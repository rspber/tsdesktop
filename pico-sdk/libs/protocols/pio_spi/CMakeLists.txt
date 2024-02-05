add_library(pio_spi INTERFACE)

target_include_directories(pio_spi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../../rp2040/pio_spi)

target_sources(pio_spi INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../../rp2040/pio_spi/RP2040_TFT_PIO_SPI.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../../rp2040/pio_spi/RP2040_TOUCH_PIO_SPI.cpp
)

target_link_libraries(pio_spi INTERFACE
  hardware_pio
  tft
)
