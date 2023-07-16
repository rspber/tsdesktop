add_library(ds3231 INTERFACE)

target_include_directories(ds3231 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(ds3231 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/TSD_DS3231.cpp
)
