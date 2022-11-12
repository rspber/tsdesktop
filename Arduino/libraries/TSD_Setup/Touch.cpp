/*
  Touch Screen for TSDesktop
  implemented on XPT2046_Touchscreen

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
  added support fo 2.4" TFT SPI 240X320 V1.3
*/

#include "Touch.h"
#include "Display.h"

void Touch::setRotation(int8_t aRotation)
{
  XPT2046_Touchscreen::setRotation(aRotation);
  rotation = aRotation;
}

int8_t Touch::getRotation()
{
  return rotation;
}

bool Touch::getTouch(point_t* p)
{
  if (XPT2046_Touchscreen::touched()) {
    // A point objects holds x, y, and z coordinates
    TS_Point tsp = XPT2046_Touchscreen::getPoint();
    int8_t m = getRotation();
    if (ILI9341_VERSION < 3) {   // < v1.2
      tsp.x = 4095 - tsp.x;
      m = m & 1 ? m : (2 + m) % 4;
    }
    int16_t minx = 0;
    int16_t maxx = 4095;
    int16_t miny = 0;
    int16_t maxy = 4095;
    switch (abs(m)) {
      case 0:                 // VBT
        minx += TS_RIGHT;
        maxx -= TS_LEFT;
        miny += TS_BOTTOM;
        maxy -= TS_TOP;
        break;
      case 1:                 // HLR
        minx += TS_BOTTOM;
        maxx -= TS_TOP;
        miny += TS_LEFT;
        maxy -= TS_RIGHT;
        break;
      case 2:                 // VTB
        minx += TS_LEFT;
        maxx -= TS_RIGHT;
        miny += TS_TOP;
        maxy -= TS_BOTTOM;
        break;
      case 3:                 // HRL
        minx += TS_TOP;
        maxx -= TS_BOTTOM;
        miny += TS_RIGHT;
        maxy -= TS_LEFT;
        break;
    }
    p->x = map(tsp.x, minx, maxx, 0, display.width());
    p->y = map(tsp.y, miny, maxy, 0, display.height());
    return true;
  }
  return false;
}
