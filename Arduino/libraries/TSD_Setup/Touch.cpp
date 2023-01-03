/*
  Touch Screen for TSDesktop
  implemented on XPT2046_Touchscreen

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include "Touch.h"
#include "Display.h"
#include "Setup.h"

void Touch::setRotation(const uint8_t aRotation, const bool aReverseMode)
{
  XPT2046_Touchscreen::setRotation(aRotation);
  rotation = aRotation;
  reverse = aReverseMode;
}

bool Touch::getTouch(point_t* p)
{
  if (XPT2046_Touchscreen::touched()) {
    // A point objects holds x, y, and z coordinates
    TS_Point t_ = XPT2046_Touchscreen::getPoint();
    int16_t x = t_.x;
    int16_t y = t_.y;
    uint8_t m = getRotation();
    bool r = getReverseMode();
#if defined(ILI9341) || defined(ILI9481) || defined(ILI9486) || defined(ILI9488)
      x = 4095 - x;
      m = m & 1 ? m : ((2 + m) % 4);
#endif
    int16_t minx = 0;
    int16_t maxx = 4095;
    int16_t miny = 0;
    int16_t maxy = 4095;
    switch (m) {
      case 0:                 // VBT
        if (r) {
          x = 4095 - x;
        }
        minx += TS_RIGHT;
        maxx -= TS_LEFT;
        miny += TS_BOTTOM;
        maxy -= TS_TOP;
        break;
      case 1:                 // HLR
        if (r) {
          y = 4095 - y;
        }
        minx += TS_BOTTOM;
        maxx -= TS_TOP;
        miny += TS_LEFT;
        maxy -= TS_RIGHT;
        break;
      case 2:                 // VTB
        if (r) {
          x = 4095 - x;
        }
        minx += TS_LEFT;
        maxx -= TS_RIGHT;
        miny += TS_TOP;
        maxy -= TS_BOTTOM;
        break;
      case 3:                 // HRL
        if (r) {
          y = 4095 - y;
        }
        minx += TS_TOP;
        maxx -= TS_BOTTOM;
        miny += TS_RIGHT;
        maxy -= TS_LEFT;
        break;
    }
    p->x = map(x, minx, maxx, 0, display.width());
    p->y = map(y, miny, maxy, 0, display.height());
    return true;
  }
  return false;
}
