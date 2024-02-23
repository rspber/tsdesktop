add_library(tft INTERFACE)

target_include_directories(tft INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../tft)

target_sources(tft INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../tft/TFT_SCREEN.cpp
)

target_link_libraries(tft INTERFACE
  gfx
)
