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

class TSD_GFX {

public:
  TSD_GFX(int16_t w, int16_t h); // Constructor

  /**********************************************************************/
  /*!
    @brief  Draw to the screen/framebuffer/etc.
    Must be overridden in subclass.
    @param  x    X coordinate in pixels
    @param  y    Y coordinate in pixels
    @param color  16-bit pixel color.
  */
  /**********************************************************************/
  virtual void drawPixel(int16_t x, int16_t y, rgb_t color) = 0;

  // TRANSACTION API / CORE DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void startWrite(void);
  virtual void writePixel(int16_t x, int16_t y, rgb_t color);
  virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color);
  virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, rgb_t color);
  virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, rgb_t color);
  virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, rgb_t color);
  virtual void endWrite(void);

  // BASIC DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.

  // It's good to implement those, even if using transaction API
  virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, rgb_t color);
  virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, rgb_t color);
  virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color);
  virtual void fillScreen(rgb_t color);
  // Optional and probably not necessary to change
  virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, rgb_t color);
  virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color);

  // These exist only with Adafruit_GFX (no subclass overrides)
  void drawCircle(int16_t x0, int16_t y0, int16_t r, rgb_t color);
  void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, rgb_t color);
  void fillCircle(int16_t x0, int16_t y0, int16_t r, rgb_t color);
  void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, rgb_t color);
  void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color);
  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color);
  void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, rgb_t color);
  void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, rgb_t color);
  void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, rgb_t color);
  void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, rgb_t color, rgb_t bg);
  void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
  void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
  void drawRGBBitmap(int16_t x, int16_t y, rgb_t *bitmap, int16_t w, int16_t h);
  void drawRGBBitmap(int16_t x, int16_t y, rgb_t *bitmap, uint8_t *mask, int16_t w, int16_t h);

  void drawChar(cursor_t *cursor, font_t *font, const char c, const rgb_t color);
  void drawTextLine(cursor_t *cursor, font_t *font, const char *text, const rgb_t color = WHITE, const int8_t spacing = 0);

  friend class TSD_ILI9341;
 private:
  int16_t WIDTH, HEIGHT;
  int16_t _width;       ///< Display width as modified by rotation
  int16_t _height;      ///< Display height as modified by rotation
};

