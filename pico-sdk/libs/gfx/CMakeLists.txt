add_library(gfx INTERFACE)

target_include_directories(gfx INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../gfx)

target_sources(gfx INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../gfx/t_clip.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../gfx/tsd_font.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../gfx/TSD_GFX.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../gfx/TSD_SCREEN.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../gfx/BufferedDisplay.cpp
)
