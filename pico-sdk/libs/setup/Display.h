/*
  Graphics display adapter for TSDesktop
     implemented on TSD_ILI9341_SPI

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "TSD_ILI9341_SPI.h"
#include <Setup.h>

class Display : public TSD_ILI9341_SPI {
public:
  Display() : TSD_ILI9341_SPI(DISPLAY_WIDTH, DISPLAY_HEIGHT) {}
};

extern bool screenEnabled;
extern Display display;

