/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "TSDesktop.h"
#include "GFXObject.h"
#include "GFXStd.h"

/// @GFXButton

class GFXButton : public Button {
public:
  GFXButton() : GFXButton(-1, -1, ALIGN_CLIENT, ALIGN_CLIENT) {}
  virtual ~GFXButton();

  GFXButton(
    const int16_t aLeft,
    const int16_t aTop,
    const int16_t aWidth,
    const int16_t aHeight,
    rgb_t aBackgroundColor = BLACK,
    rgb_t aBorderColor = WHITE)
    : Button(TYPE_GFXBUTTON, aLeft, aTop, aWidth, aHeight, aBackgroundColor, aBorderColor)
  {
  }

  GFXObject* add(GFXObject* item);
  GFXObject* remove(GFXObject* item);
  GFXObject* remove(int16_t index);
  int16_t index(GFXObject* item);

  GFXPixel* pixel(int16_t x1, int16_t y1, rgb_t color);
  GFXLine* line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts = 1); // thickness
  GFXRect* rect(int16_t x1, int16_t y1, int16_t w, int16_t h, rgb_t color, int16_t ts = 1); // thickness
  GFXRoundRect* roundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, rgb_t color, int16_t ts = 1); // thickness
  GFXFillRect* fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, rgb_t color);
  GFXFillRoundRect* fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, rgb_t color);
  GFXCircle* circle(int16_t x0, int16_t y0, int16_t r, rgb_t color, int16_t ts = 1); // thickness
  GFXCircleFragment* circleFragment(int16_t x0, int16_t y0, int16_t r, uint8_t fragment, rgb_t color, int16_t ts = 1); // thickness
  GFXFillCircle* fillCircle(int16_t x0, int16_t y0, int16_t r, rgb_t color);
  GFXFillCircleFragment* fillCircleFragment(int16_t x0, int16_t y0, int16_t r, uint8_t fragment, int16_t delta, rgb_t color);
  GFXTriangle* triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts = 1); // thickness
  GFXFillTriangle* fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color);
  GFXBitmap* bitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color);
  GFXBitmap* bitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color, rgb_t bg);
  GFXGrayscaleBitmap* grayscaleBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h);
  GFXGrayscaleBitmap* grayscaleBitmap(int16_t x, int16_t y, const uint8_t* bitmap, const uint8_t* mask, int16_t w, int16_t h);
  GFXRGBBitmap* RGBBitmap(int16_t x, int16_t y, const rgb_t* bitmap, int16_t w, int16_t h);
  GFXRGBBitmap* RGBBitmap(int16_t x, int16_t y, const rgb_t* bitmap, const uint8_t *mask, int16_t w, int16_t h);

  GFXChar* character(int16_t x1, int16_t y1, const char* c, rgb_t color = WHITE, rgb_t bg = BLACK);
  GFXTextLine* textLine(int16_t x1, int16_t y1, const char* text, rgb_t color = WHITE, rgb_t bg = BLACK);

  virtual void innerDraw(const bool redraw);

  void drawAll(bool redraw = true);

private:
  GFXObject** list = NULL;
  int16_t len = 0, count = 0;
};
