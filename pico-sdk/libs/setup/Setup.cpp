/*
  Setup for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "Setup.h"
#include <pico/stdio.h>

#include <Display.h>
#include <Touch.h>

void init_hardware()
{
  stdio_init_all();
}

PicoSPI displaySPI(TFT_CS, SPI0_DC);
PicoSPI touchSPI(TOUCH_CS, SPI0_DC);

void media_begin(int8_t rotation)
{
  if (ILI9341_VERSION < 3) {     // < v1.2
    rotation = rotation & 1 ? rotation : (rotation + 2) % 4;
  }
  display.begin(&displaySPI, TFT_RST);
  display.setRotation(rotation);
//  display.invertDisplay(true);    // invert display colors  WHITE <-> BLACK

  touch.begin(&touchSPI);
  touch.setRotation(rotation & 1 ? rotation : (2 - rotation) % 4);

  display.clearDisplay();
}
