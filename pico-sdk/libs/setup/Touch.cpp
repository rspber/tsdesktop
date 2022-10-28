/*
  Touch Screen for TSDesktop
  implemented on XPT2046_Touchscreen

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
  added support fo 2.4" TFT SPI 240X320 V1.3
*/

#include "Touch.h"
#include "Setup.h"

int16_t inline map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
  return (long)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool Touch::getTouch(point_t* p)
{
  int16_t tx, ty;
  if (TSD_XPT2046::getTouch(&tx, &ty)) {
    int8_t m = getRotation();
    if (ILI9341_VERSION < 3) {   // < v1.2
      tx = 4095 - tx;
      m = m & 1 ? m : (2 + m) % 4;
    }
    int16_t minx = 0;
    int16_t maxx = 4095;
    int16_t miny = 0;
    int16_t maxy = 4095;
    switch (abs(m)) {
      case 0:                 // VLR
        minx += TS_RIGHT;
        maxx -= TS_LEFT;
        miny += TS_TOP;
        maxy -= TS_BOTTOM;
        break;
      case 1:                 // HLR
        minx += TS_TOP;
        maxx -= TS_BOTTOM;
        miny += TS_LEFT;
        maxy -= TS_RIGHT;
        break;
      case 2:                 // VRL
        minx += TS_LEFT;
        maxx -= TS_RIGHT;
        miny += TS_BOTTOM;
        maxy -= TS_TOP;
        break;
      case 3:                 // HRL
        minx += TS_BOTTOM;
        maxx -= TS_TOP;
        miny += TS_RIGHT;
        maxy -= TS_LEFT;
        break;
    }
    p->x = map(tx, minx, maxx, 0, display.width());
    p->y = map(ty, miny, maxy, 0, display.height());
    return true;
  }
  return false;
}
