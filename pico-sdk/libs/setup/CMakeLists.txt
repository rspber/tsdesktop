add_library(setup INTERFACE)

target_include_directories(setup INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(setup INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/Picoino.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Setup.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Display.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Touch.cpp
)

target_link_libraries(setup INTERFACE
  pico_stdlib
  hardware_spi
  hardware_i2c
)
