/*
  Graphics display adapter for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "TSD_ILI9341.h"
#include <Setup.h>

class Display : public TSD_ILI9341 {
public:
  Display() : TSD_ILI9341(DISPLAY_WIDTH, DISPLAY_HEIGHT) {}
};

extern bool screenEnabled;
extern Display display;
