add_library(ili9341 INTERFACE)

target_include_directories(ili9341 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../TSD_ILI9341)

target_sources(ili9341 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../TSD_ILI9341/TSD_ILI9341.cpp
)

target_link_libraries(ili9341 INTERFACE
  gfx
)
