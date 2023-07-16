/*
  OVERLAID buffer

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include "rgb.h"

typedef struct {
  uint8_t mode;     // 1 - pushPixels, 2 - getPixels
  rgb_t color;		// real color if mode = 0
  uint8_t* buf;
  int16_t len, size;

} over_t;
