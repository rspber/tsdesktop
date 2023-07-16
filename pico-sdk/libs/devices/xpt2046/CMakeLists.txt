add_library(xpt2046 INTERFACE)

target_include_directories(xpt2046 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(xpt2046 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/TSD_XPT2046.cpp
)

target_link_libraries(xpt2046 INTERFACE
  spi
)
