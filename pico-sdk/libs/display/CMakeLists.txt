add_library(display INTERFACE)

target_include_directories(display INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(display INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/TSD_ILI9341.cpp
)

target_link_libraries(display INTERFACE
  gfx
)
