add_library(pcf8575 INTERFACE)

target_include_directories(pcf8575 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(pcf8575 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/TSD_PCF8575.cpp
)
