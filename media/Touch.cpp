/*
  Touch Screen for TSDesktop
  implemented on TSD_XPT2046

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include "Touch.h"

#if defined(TOUCH_DRIVER)

#include "Touch_Rotation.hh"
#include <Arduino.h>

bool Touch::WhereAreYou(point_t* p)
{
  int16_t x, y;
  if (IamHere(x, y)) {
    uint8_t r = reverse ^ TOUCH_ROTATION(rotation);

    if (r & MAD_YX) {
      int16_t tmp = x; x = y; y = tmp;
    }
    if ((r & MAD_SS) | (r & MAD_MX)) {
      x = 4095 - x;
    }
    if ((r & MAD_GS) | (r & MAD_MY)) {
      y = 4095 - y;
    }

    int16_t minx = 0;
    int16_t maxx = 4095;
    int16_t miny = 0;
    int16_t maxy = 4095;
    switch (rotation) {
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

    int16_t width, height;
    if ((rotation & 1) == 0) {
        width = TFT_WIDTH;
        height = TFT_HEIGHT;
    }
    else {
        height = TFT_WIDTH;
        width = TFT_HEIGHT;
    }

    p->x = map(x, minx, maxx, 0, width);
    p->y = map(y, miny, maxy, 0, height);
    return true;
  }
  return false;
}

#endif
