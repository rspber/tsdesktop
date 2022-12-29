/*
  Pixel's buffer for TSD_ILI9341

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <stdint.h>
#include <Setup.h>

typedef uint32_t rgb_t;
#define RGB(R,G,B)  (0xFF000000 | (((uint32_t)(R) << 16)) | ((uint16_t)(G) << 8) | (B))
#define RGB565(R,G,B) (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))
inline rgb_t rgb(const uint16_t color)
{
  return (rgb_t)(0xFF000000 | ((uint32_t)(color & 0xF800) << 8) | ((color & 0x07E0) << 5) | (color & 0x1F));
}

#ifdef COLOR_565
  #define MDT_SIZE 2
#else
  #define MDT_SIZE 3
#endif

#define WHITE RGB(0xFF, 0xFF, 0xFF)
#define BLACK RGB(0x00, 0x00, 0x00)

uint8_t* mdt_color(uint8_t* buf, const rgb_t color, int size);

typedef struct {
  uint8_t mode;     // 1 - pushPixels, 2 - getPixels
  rgb_t color;		// real color if mode = 0
  uint8_t* buf;
  int len, size;
} over_t;
