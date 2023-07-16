add_library(media INTERFACE)

target_include_directories(media INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../media)

target_sources(media INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../media/Touch.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../media/media.cpp
)

target_link_libraries(media INTERFACE
  env
  xpt2046
  spfd5408
)
