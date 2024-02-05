add_library(gpio_8bitp INTERFACE)

target_include_directories(gpio_8bitp INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../../rp2040/gpio_8bitp)

target_sources(gpio_8bitp INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../../rp2040/gpio_8bitp/RP2040_TFT_GPIO_8BITP.cpp
)

target_link_libraries(gpio_8bitp INTERFACE
  hardware_gpio
  tft
)
