add_library(display INTERFACE)

target_include_directories(display INTERFACE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/Fonts)

target_sources(display INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/buf16.cpp
  ${CMAKE_CURRENT_LIST_DIR}/font.cpp
  ${CMAKE_CURRENT_LIST_DIR}/TSD_GFX.cpp
  ${CMAKE_CURRENT_LIST_DIR}/TSD_ILI9341.cpp
)
