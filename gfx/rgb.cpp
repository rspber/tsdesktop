/*
  Pixel's buffer for TSD_ILI9341

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "rgb.h"

uint8_t* mdt_color(uint8_t* buf, const rgb_t color, int size)
{
  uint8_t b[4];
  *(uint32_t *)b = color;
  if (MDT_SIZE == 2) {
    *(uint16_t *)b = RGB565(b[2], b[1], b[0]);
  }
  uint8_t* p = buf;
  while (--size >= 0) {
    int8_t i = MDT_SIZE;
    while (--i >= 0) {
      *p++ = b[i];
    }
  }
  return buf;
}
