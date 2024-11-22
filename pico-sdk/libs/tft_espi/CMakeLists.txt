add_library(tft_espi INTERFACE)

target_include_directories(tft_espi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../tft_espi)

target_sources(tft_espi INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../tft_espi/TFT_eSprite.cpp
)

target_link_libraries(tft_espi INTERFACE
  gfx
)
