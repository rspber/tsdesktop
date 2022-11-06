add_library(display INTERFACE)

target_include_directories(display INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(display INTERFACE
  ili9341
)
