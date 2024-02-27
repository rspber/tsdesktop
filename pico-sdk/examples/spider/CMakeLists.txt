add_executable(spider
  spider.cpp
)

target_link_libraries(spider
  media
)

pico_enable_stdio_usb(spider 1)
pico_enable_stdio_uart(spider 0)

pico_add_extra_outputs(spider)
