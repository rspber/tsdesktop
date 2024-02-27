/*
  Graphics library for TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

  More information in TSD_GFX.cpp

*/

#pragma once

#include "font.h"
#include <stddef.h>
#include "rgb.h"

typedef struct {
  int16_t x, y;
} cursor_t;

#define GNT_LR 1 // horizontal gradient left to right
#define GNT_TB 2 // vertical gradient top to bottom
#define GNT_RL 3 // horizontal gradient right to left
#define GNT_BT 4 // vertical gradient bottom to top

typedef struct {
  uint8_t deg;  // 1,3 - horizontal, 2,4 -vertical
  rgb_t color1;
  int8_t percent; // 0 .. 50 .. 100
  rgb_t color2;
//  int8_t gradient = 0;
} gradient_t;

struct t_clip_t {
  int16_t x1, y1, x2, y2;
  const int16_t width();
  const int16_t height();
};

typedef t_clip_t clip_t;

class TSD_GFX {
public:
  void drawPixel(clip_t& clip, int16_t x, int16_t y, rgb_t color);
  void drawFastVLine(clip_t& clip, int16_t x, int16_t y, int16_t h, rgb_t color);
  void drawFastHLine(clip_t& clip, int16_t x, int16_t y, int16_t w, rgb_t color);
  void drawLine(clip_t& clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, rgb_t color);
  void drawRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color);
  void fillRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color);
  void fillRectGradient(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, gradient_t& z);
  void drawRoundRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, rgb_t color);
  void fillRoundRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, rgb_t color);
  void drawCircle(clip_t& clip, int16_t x, int16_t y, int16_t r, rgb_t color);
  void drawCircleFragment(clip_t& clip, int16_t x, int16_t y, int16_t r, uint8_t corners, rgb_t color);
  void fillCircle(clip_t& clip, int16_t x, int16_t y, int16_t r, rgb_t color);
  void fillCircleFragment(clip_t& clip, int16_t x, int16_t y, int16_t r, uint8_t corners, int16_t delta, rgb_t color);
  void drawEllipse(clip_t& clip, int16_t x0, int16_t y0, int16_t rx, int16_t ry, rgb_t color);
  void fillEllipse(clip_t& clip, int16_t x0, int16_t y0, int16_t rx, int16_t ry, rgb_t color);
  void drawTriangle(clip_t& clip, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, rgb_t color);
  void fillTriangle(clip_t& clip, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, rgb_t color);
  void drawBitmap(clip_t& clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color);
  void drawBitmap(clip_t& clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color, rgb_t bg);
  void drawGrayscaleBitmap(clip_t& clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h);
  void drawGrayscaleBitmap(clip_t& clip, int16_t x, int16_t y, const uint8_t* bitmap, const uint8_t* mask, int16_t w, int16_t h);
  void drawRGBBitmap(clip_t& clip, int16_t x, int16_t y, const uint16_t* bitmap, int16_t w, int16_t h);   // 565 color
  void drawRGBBitmap(clip_t& clip, int16_t x, int16_t y, const uint16_t* bitmap, const uint8_t *mask, int16_t w, int16_t h); // 565 color
  void drawRGBBitmap(clip_t& clip, int16_t x, int16_t y, const uint32_t* bitmap, int16_t w, int16_t h);   // 666 color
  void drawRGBBitmap(clip_t& clip, int16_t x, int16_t y, const uint32_t* bitmap, const uint8_t *mask, int16_t w, int16_t h); // 666 color

  // utf-8
  void drawChar(clip_t& clip, cursor_t& cursor, font_t& font, char** c, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);
  const char* drawTextLine(clip_t& clip, cursor_t& cursor, font_t& font, const char* text, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);

  // unicode
  void drawChar(clip_t& clip, cursor_t& cursor, font_t& font, const uint16_t uchar, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);
  const uint16_t* drawTextLine(clip_t& clip, cursor_t& cursor, font_t& font, const uint16_t* utext, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);

  void fillRectHelper(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color);
  void drawCircleHelper(clip_t& clip, int16_t x, int16_t y, int16_t r, uint8_t corners, rgb_t color);
  void fillCircleHelper(clip_t& clip, int16_t x, int16_t y, int16_t r, uint8_t corners, int16_t delta, rgb_t color);

  void fillRectVGradient(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, gradient_t& z);
  void fillRectHGradient(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, gradient_t& z);

  virtual void startWrite() = 0;
  virtual void endWrite() = 0;
  virtual void writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h) = 0;
  virtual void sendMDTColor1(const mdt_t c) = 0;
  virtual void sendMDTColor(const mdt_t c, const int32_t len);
  virtual void drawPixel1(const int16_t x, const int16_t y, const rgb_t color);
  virtual void drawPixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color);
};
