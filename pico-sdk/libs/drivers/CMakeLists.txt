add_library(drivers INTERFACE)

target_include_directories(drivers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../drivers)

target_sources(drivers INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9341.cpp
)
