/*
  Graphics library for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

  More information in TSD_GFX.cpp

*/

#pragma once

#include "font.h"
#include <stddef.h>
#include "rgb.h"

typedef struct {
  int16_t x, y;
} cursor_t;

typedef struct {
  int16_t x1, y1, x2, y2;
} clip_t;

class TSD_GFX {

public:
  /**********************************************************************/
  /*!
    @brief  Draw to the screen/framebuffer/etc.
    Must be overridden in subclass.
    @param  x    X coordinate in pixels
    @param  y    Y coordinate in pixels
    @param color  16-bit pixel color.
  */
  /**********************************************************************/
  virtual void drawPixel(clip_t* clip, int16_t x, int16_t y, rgb_t color) = 0;

  // TRANSACTION API / CORE DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void startWrite(void);
  virtual void writePixel(clip_t* clip, int16_t x, int16_t y, rgb_t color);
  virtual void writeFillRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color);
  virtual void writeFastVLine(clip_t* clip, int16_t x, int16_t y, int16_t h, rgb_t color);
  virtual void writeFastHLine(clip_t* clip, int16_t x, int16_t y, int16_t w, rgb_t color);
  virtual void writeLine(clip_t* clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, rgb_t color);
  virtual void endWrite(void);

  // BASIC DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.

  // It's good to implement those, even if using transaction API
  virtual void drawFastVLine(clip_t* clip, int16_t x, int16_t y, int16_t h, rgb_t color);
  virtual void drawFastHLine(clip_t* clip, int16_t x, int16_t y, int16_t w, rgb_t color);
  virtual void fillRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color);
  // Optional and probably not necessary to change
  virtual void drawLine(clip_t* clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, rgb_t color);
  virtual void drawRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color);

  // These exist only with Adafruit_GFX (no subclass overrides)
  void drawCircle(clip_t* clip, int16_t x0, int16_t y0, int16_t r, rgb_t color);
  void drawCircleHelper(clip_t* clip, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, rgb_t color);
  void fillCircle(clip_t* clip, int16_t x0, int16_t y0, int16_t r, rgb_t color);
  void fillCircleHelper(clip_t* clip, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, rgb_t color);
  void drawTriangle(clip_t* clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color);
  void fillTriangle(clip_t* clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color);
  void drawRoundRect(clip_t* clip, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, rgb_t color);
  void fillRoundRect(clip_t* clip, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, rgb_t color);
  void drawBitmap(clip_t* clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color);
  void drawBitmap(clip_t* clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color, rgb_t bg);
  void drawGrayscaleBitmap(clip_t* clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h);
  void drawGrayscaleBitmap(clip_t* clip, int16_t x, int16_t y, const uint8_t* bitmap, const uint8_t* mask, int16_t w, int16_t h);
  void drawRGBBitmap(clip_t* clip, int16_t x, int16_t y, const rgb_t* bitmap, int16_t w, int16_t h);
  void drawRGBBitmap(clip_t* clip, int16_t x, int16_t y, const rgb_t* bitmap, uint8_t *mask, int16_t w, int16_t h);

  void drawChar(clip_t* clip, cursor_t* cursor, font_t* font, const char c, const rgb_t color);
  void drawTextLine(clip_t* clip, cursor_t* cursor, font_t* font, const char* text, const rgb_t color = WHITE, const int8_t spacing = 0);
};

