add_executable(idyll
  idyll.cpp
)

target_link_libraries(idyll
  media
)

pico_enable_stdio_usb(idyll 1)
pico_enable_stdio_uart(idyll 0)

pico_add_extra_outputs(idyll)
