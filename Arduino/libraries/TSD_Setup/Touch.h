/*
  Touch Screen for TSDesktop
  implemented on XPT2046_Touchscreen

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "Setup.h"

#include <XPT2046_Touchscreen.h>

typedef struct {
  int16_t x, y;
} point_t;

class Touch : public XPT2046_Touchscreen {
public:
  Touch() : XPT2046_Touchscreen(TOUCH_CS) {}

  void setRotation(int8_t aRotation);
  int8_t getRotation();
  bool getTouch(point_t* p);

private:
  int8_t rotation;
};

extern Touch touch;
