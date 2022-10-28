/*
  Touch Screen for TSDesktop
  implemented on TSD_XPT2046

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <TSD_XPT2046.h>
#include <Display.h>

typedef struct {
  int16_t x, y;
} point_t;

class Touch : public TSD_XPT2046 {
public:
  Touch() : TSD_XPT2046() {}

  bool getTouch(point_t* p);
};

extern Touch touch;
