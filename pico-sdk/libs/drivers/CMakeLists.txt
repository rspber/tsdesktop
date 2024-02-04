add_library(drivers INTERFACE)

target_include_directories(drivers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../drivers)

target_sources(drivers INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_GC9A01.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_HX8357B.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_HX8357C.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_HX8357D.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9341.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9481_1.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9481_AUO317.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9481_CMO35.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9481_CPT29.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9481_PVI35.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9481_RGB.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9481_YA1.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9481_YA2.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9486.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9488.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9x81.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ILI9xxx.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_R61581.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_RM68140.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_SSD1963_480.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_SSD1963_800ALT.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_SSD1963_800BD.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_SSD1963_800.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_SSD196x.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ST7789.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers/TFT_ST7796.cpp
)
