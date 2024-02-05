add_library(pio_8bitp INTERFACE)

target_include_directories(pio_8bitp INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../../rp2040/pio_8bitp)

target_sources(pio_8bitp INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../../rp2040/pio_8bitp/RP2040_TFT_PIO_8BITP.cpp
)

target_link_libraries(pio_8bitp INTERFACE
  hardware_pio
  tft
)
