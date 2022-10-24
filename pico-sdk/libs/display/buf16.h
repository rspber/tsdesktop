/*
  Pixel's buffer for TSD_ILI9341

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <stdint.h>

#define SWAP16(x) (x >> 8 | x << 8)

uint16_t* uint16_color(uint16_t* buf, const uint16_t color, int size);
uint16_t* uint16_array(uint16_t* buf, const int size);

uint16_t* buffer_uint16_color(const uint16_t color, int size);
uint16_t* buffer_uint16_array(const int size);
