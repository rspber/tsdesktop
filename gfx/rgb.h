/*
  RGB

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#pragma once

#include <Setup.h>
#include <stdint.h>

typedef uint32_t rgb_t;

inline rgb_t RGB(uint8_t R, uint8_t G, uint8_t B) { return 0xFF000000 | (((uint32_t)(R) << 16)) | ((uint16_t)(G) << 8) | B; }

inline uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B) { return ((uint16_t)(R >> 3) << 11) | ((uint16_t)(G >> 2) << 5) | (B >> 3); }

inline rgb_t rgb16(const uint16_t color)
{
  return (rgb_t)(0xFF000000 | ((uint32_t)(color & 0xF800) << 8) | ((color & 0x07E0) << 5) | (color & 0x1F) << 3);
}

inline rgb_t rgb24(const uint32_t color)
{
  return (rgb_t)(0xFF000000 | color);
}

#ifdef COLOR_565
  #define MDT_SIZE 2
  #define mdt_t uint16_t
  #define rgb rgb16
#else
  #define MDT_SIZE 3
  #define mdt_t uint32_t
  #define rgb rgb24
#endif

#define WHITE RGB(0xFF, 0xFF, 0xFF)
#define BLACK RGB(0x00, 0x00, 0x00)

inline mdt_t mdt_color(const rgb_t color)
{
#ifdef COLOR_565
  return RGB565(color >> 16, color >> 8, color);
#else
  return color & 0x00ffffff;
#endif
}

inline uint16_t swap16(const uint16_t v) { return (v >> 8) | (v << 8); }
