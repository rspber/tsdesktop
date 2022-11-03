add_library(gfx INTERFACE)

target_include_directories(gfx INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../TSD_GFX)

target_sources(gfx INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../TSD_GFX/rgb.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../TSD_GFX/font.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../TSD_GFX/TSD_GFX.cpp
)
