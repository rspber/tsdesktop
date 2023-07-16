add_library(spfd5408 INTERFACE)

target_include_directories(spfd5408 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(spfd5408 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/TSD_SPFD5408.cpp
)

target_link_libraries(spfd5408 INTERFACE
  hardware_gpio
  hardware_adc
)
