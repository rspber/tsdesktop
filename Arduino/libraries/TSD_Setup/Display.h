/*
  Graphics display adapter for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "TFT_ILI9341.h"
#include <Setup.h>

class Display : public TFT_ILI9341 {
public:
  Display() : TFT_ILI9341(DISPLAY_WIDTH, DISPLAY_HEIGHT) {}
};

extern bool screenEnabled;
extern Display display;
