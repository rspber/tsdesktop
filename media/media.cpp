/*
  Media

  Display & Touch

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include "Media.h"

void media_begin(uint8_t rotation)
{
  display.begin();

  display.setRotation(rotation, TFT_REV);

//  display.invertDisplay(true);    // invert display colors  WHITE <-> BLACK

  touch.begin();

  touch.setRotation(rotation, TOUCH_REV);

  display.clearDisplay();
}
