add_library(snakes INTERFACE)

target_include_directories(snakes INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../../utils/snakes)

target_sources(snakes INTERFACE
${CMAKE_CURRENT_LIST_DIR}/../../../../utils/snakes/Snakes.cpp
${CMAKE_CURRENT_LIST_DIR}/../../../../utils/snakes/SnakeStamp.cpp
)
