/*
  Touch Screen for TSDesktop
  implemented on XPT2046_Touchscreen

  Copyright (c) 2023, rspber (https://github.com/rspber)

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

  void setRotation(const uint8_t aRotation, const bool aReverseMode);
  uint8_t getRotation() { return rotation; };
  bool getReverseMode() { return reverse; };
  bool getTouch(point_t* p);

private:
  uint8_t rotation;
  bool reverse;
};

extern Touch touch;
