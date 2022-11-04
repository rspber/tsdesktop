/*
  Pixel's buffer for TSD_ILI9341

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "rgb.h"

#define MAXLEN 2048

uint8_t buffer[MAXLEN * MDT_SIZE + 4];

uint8_t* mdt_color(uint8_t* buf, const rgb_t color, int size)
{
  uint8_t b[4];
  *(uint32_t *)b = color;
  uint8_t* p = buf;
  while (--size >= 0) {
    int8_t i = MDT_SIZE;
    while( --i >= 0 ) {
      *p++ = b[i];
    }
  }
  return buf;
}

uint8_t* buffer_mdt_color(const rgb_t color, const int size)
{
  return mdt_color(buffer, color, size > MAXLEN ? MAXLEN : size);
}
