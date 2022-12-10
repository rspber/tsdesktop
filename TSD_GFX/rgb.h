/*
  Pixel's buffer for TSD_ILI9341

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <stdint.h>
#include <Setup.h>

#ifdef COLOR_565
  typedef uint16_t rgb_t;
  #define RGB(R,G,B)  (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))
  #define MDT_SIZE 2
#else
  typedef uint32_t rgb_t;
  #define RGB(R,G,B)  ((((uint32_t)(R) << 16)) | ((uint16_t)(G) << 8) | (B))
  #define MDT_SIZE 3
#endif

#define WHITE RGB(0xFF, 0xFF, 0xFF)
#define BLACK RGB(0x00, 0x00, 0x00)

uint8_t* mdt_color(uint8_t* buf, const rgb_t color, int size);
