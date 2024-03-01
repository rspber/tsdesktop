add_executable(spider2
  spider2.cpp
)

target_link_libraries(spider2
  media
)

pico_enable_stdio_usb(spider2 1)
pico_enable_stdio_uart(spider2 0)

pico_add_extra_outputs(spider2)
