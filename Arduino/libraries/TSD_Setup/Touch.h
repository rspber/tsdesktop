/*
  Touch Screen for TSDesktop
  implemented on XPT2046_Touchscreen

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "Setup.h"

#include <XPT2046_Touchscreen.h>
#include <Display.h>

typedef struct {
  int16_t x, y;
} point_t;

class Touch : public XPT2046_Touchscreen {
public:
  Touch() : XPT2046_Touchscreen(TOUCH_CS) {}

  void setRotation(int8_t aRotation)
  {
    XPT2046_Touchscreen::setRotation(aRotation);
    rotation = aRotation;
  }

  int8_t getRotation()
  {
    return rotation;
  }

  bool getTouch(point_t* p)
  {
    if (XPT2046_Touchscreen::touched()) {
      // A point objects holds x, y, and z coordinates
      TS_Point tsp = XPT2046_Touchscreen::getPoint();
      tsp.x = 4095 - tsp.x;
      int16_t minx = 0;
      int16_t maxx = 4095;
      int16_t miny = 0;
      int16_t maxy = 4095;
      switch (abs(getRotation())) {
      case 1:
        minx += TS_TOP;
        maxx -= TS_BOTTOM;
        miny += TS_RIGHT;
        maxy -= TS_LEFT;
        break;
      case 3:
        minx += TS_BOTTOM;
        maxx -= TS_TOP;
        miny += TS_LEFT;
        maxy -= TS_RIGHT;
        break;
      case 0:
        minx += TS_LEFT;
        maxx -= TS_RIGHT;
        miny += TS_BOTTOM;
        maxy -= TS_TOP;
        break;
      case 2:
        minx += TS_RIGHT;
        maxx -= TS_LEFT;
        miny += TS_TOP;
        maxy -= TS_BOTTOM;
        break;
      }
      p->x = map(tsp.x, minx, maxx, 0, display.width());
      p->y = map(tsp.y, miny, maxy, 0, display.height());
      return true;
    }
    return false;
  }

  int8_t rotation;
};

extern Touch touch;
