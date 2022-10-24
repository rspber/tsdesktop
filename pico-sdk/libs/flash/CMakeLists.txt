add_library(flash INTERFACE)

target_include_directories(flash INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(flash INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/PicoFlash.cpp
)

target_link_libraries(flash INTERFACE
  hardware_flash
)
