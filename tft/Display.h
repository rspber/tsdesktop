/*
  Graphics display adapter for TSDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include "Setup.h"

#if defined GC9A01
  #include "TFT_GC9A01.h"
#endif
#if defined HX8357B
  #include "TFT_HX8357B.h"
#endif
#if defined HX8357C
  #include "TFT_HX8357C.h"
#endif
#if defined HX8357D
  #include "TFT_HX8357D.h"
#endif
#if defined ILI9341
  #include "TFT_ILI9341.h"
#endif
#if defined ILI9481_1
  #include "TFT_ILI9481_1.h"
#endif
#if defined ILI9481_AUO317
  #include "TFT_ILI9481_AUO317.h"
#endif
#if defined ILI9481_CMO35
  #include "TFT_ILI9481_CMO35.h"
#endif
#if defined ILI9481_CPT29
  #include "TFT_ILI9481_CPT29.h"
#endif
#if defined ILI9481_PVI35
  #include "TFT_ILI9481_PVI35.h"
#endif
#if defined ILI9481_RGB
  #include "TFT_ILI9481_RGB.h"
#endif
#if defined ILI9481_YA1
  #include "TFT_ILI9481_YA1.h"
#endif
#if defined ILI9481_YA2
  #include "TFT_ILI9481_YA2.h"
#endif
#if defined ILI9486
  #include "TFT_ILI9486.h"
#endif
#if defined ILI9488
  #include "TFT_ILI9488.h"
#endif
#if defined R61581
  #include "TFT_R61581.h"
#endif
#if defined RM68140
  #include "TFT_RM68140.h"
#endif
#if defined SSD1963_480
  #include "TFT_SSD1963_480.h"
#endif
#if defined SSD1963_800
  #include "TFT_SSD1963_800.h"
#endif
#if defined SSD1963_800ALT
  #include "TFT_SSD1963_800ALT.h"
#endif
#if defined SSD1963_800BD
  #include "TFT_SSD1963_800BD.h"
#endif
#if defined ST7789
  #include "TFT_ST7789.h"
#endif
#if defined ST7796
  #include "TFT_ST7796.h"
#endif

class Display : public TFT_DRIVER {
public:
  Display() : TFT_DRIVER(DISPLAY_WIDTH, DISPLAY_HEIGHT) {}  
};

extern bool screenEnabled;
extern Display display;
