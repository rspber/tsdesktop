/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "GFXButton.h"
#include <cstring>

/// @GFXButton

GFXButton::~GFXButton()
{
  if (list) {
    for( int16_t i = len; --i >= 0; ) {
      GFXObject* g = list[i];
      delete g;
    }
    free(list);
  }
}

int16_t GFXButton::index(GFXObject* item)
{
  for( int16_t i = len; --i >= 0; ) {
    if (list[i] == item) {
      return i;
    }
  }
  return -1;
}

GFXObject* GFXButton::remove(GFXObject* item)
{
  for( int16_t i = len; --i >= 0; ) {
    if (list[i] == item) {
      remove(i);
    } 
  }
  return item;
}

GFXObject* GFXButton::remove(int16_t index)
{
  if (index >= 0 && index < len) {
    GFXObject* item = list[index];
    memcpy(&list[index], &list[index+1], (--len - index) * sizeof(GFXObject*));
    return item;
  }
  return NULL;
}

GFXObject* GFXButton::add(GFXObject* item)
{
  remove(item);
  if (len >= count) {
    int16_t nc = count+16;
    list = (GFXObject**)realloc(list, nc * sizeof(GFXObject*));
    count = nc;
  }
  list[len++] = item;
  return item;
}


///

GFXPixel* GFXButton::pixel(int16_t x1, int16_t y1, rgb_t color)
{
  return (GFXPixel*)add(new GFXPixel(this, x1, y1, color));
}

GFXLine* GFXButton::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts)
{
  return (GFXLine*)add(new GFXLine(this, x1, y1, x2, y2, color, ts));
}

GFXRect* GFXButton::rect(int16_t x1, int16_t y1, int16_t w, int16_t h, rgb_t color, int16_t ts)
{
  return (GFXRect*)add(new GFXRect(this, x1, y1, w, h, color, ts));
}

GFXRoundRect* GFXButton::roundRect(int16_t x1, int16_t y1, int16_t w, int16_t h, int16_t radius, rgb_t color, int16_t ts)
{
  return (GFXRoundRect*)add(new GFXRoundRect(this, x1, y1, w, h, radius, color, ts));
}

GFXFillRect* GFXButton::fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, rgb_t color)
{
  return (GFXFillRect*)add(new GFXFillRect(this, x1, y1, w, h, color));
}

GFXFillRoundRect* GFXButton::fillRoundRect(int16_t x1, int16_t y1, int16_t w, int16_t h, int16_t radius, rgb_t color)
{
  return (GFXFillRoundRect*)add(new GFXFillRoundRect(this, x1, y1, w, h, radius, color));
}

GFXCircle* GFXButton::circle(int16_t x0, int16_t y0, int16_t r, rgb_t color, int16_t ts)
{
  return (GFXCircle*)add(new GFXCircle(this, x0, y0, r, color, ts));
}

GFXCircleFragment* GFXButton::circleFragment(int16_t x0, int16_t y0, int16_t r, uint8_t fragment, rgb_t color, int16_t ts)
{
  return (GFXCircleFragment*)add(new GFXCircleFragment(this, x0, y0, r, fragment, color, ts));
}

GFXFillCircle* GFXButton::fillCircle(int16_t x0, int16_t y0, int16_t r, rgb_t color)
{
  return (GFXFillCircle*)add(new GFXFillCircle(this, x0, y0, r, color));
}

GFXFillCircleFragment* GFXButton::fillCircleFragment(int16_t x0, int16_t y0, int16_t r, uint8_t fragment, int16_t delta, rgb_t color)
{
  return (GFXFillCircleFragment*)add(new GFXFillCircleFragment(this, x0, y0, r, fragment, delta, color));
}

GFXTriangle* GFXButton::triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts)
{
  return (GFXTriangle*)add(new GFXTriangle(this, x0, y0, x1, y1, x2, y2, color, ts));
}

GFXFillTriangle* GFXButton::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color)
{
  return (GFXFillTriangle*)add(new GFXFillTriangle(this, x0, y0, x1, y1, x2, y2, color));
}

GFXBitmap* GFXButton::bitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color)
{
  return (GFXBitmap*)add(new GFXBitmap(this, x, y, bitmap, w, h, color));
}

GFXBitmap* GFXButton::bitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color, rgb_t bg)
{
  return (GFXBitmap*)add(new GFXBitmap(this, x, y, bitmap, w, h, color, bg));
}

GFXGrayscaleBitmap* GFXButton::grayscaleBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h)
{
  return (GFXGrayscaleBitmap*)add(new GFXGrayscaleBitmap(this, x, y, bitmap, w, h));
}

GFXGrayscaleBitmap* GFXButton::grayscaleBitmap(int16_t x, int16_t y, const uint8_t* bitmap, const uint8_t* mask, int16_t w, int16_t h)
{
  return (GFXGrayscaleBitmap*)add(new GFXGrayscaleBitmap(this, x, y, bitmap, mask, w, h));
}

GFXRGBBitmap* GFXButton::RGBBitmap(int16_t x, int16_t y, const rgb_t* bitmap, int16_t w, int16_t h)
{
  return (GFXRGBBitmap*)add(new GFXRGBBitmap(this, x, y, bitmap, w, h));
}

GFXRGBBitmap* GFXButton::RGBBitmap(int16_t x, int16_t y, const rgb_t* bitmap, const uint8_t *mask, int16_t w, int16_t h)
{
  return (GFXRGBBitmap*)add(new GFXRGBBitmap(this, x, y, bitmap, mask, w, h));
}


GFXChar* GFXButton::character(int16_t x1, int16_t y1, const char* c, rgb_t color, rgb_t bg)
{
  return (GFXChar*)add(new GFXChar(this, x1, y1, c, color, bg));
}

GFXTextLine* GFXButton::textLine(int16_t x1, int16_t y1, const char* text, rgb_t color, rgb_t bg)
{
  return (GFXTextLine*)add(new GFXTextLine(this, x1, y1, text, color, bg));
}


///

void GFXButton::innerDraw(bool redraw)
{
  clip_t clip;
  getClip(clip);
  int16_t x = getAbsInnerLeft(0);
  int16_t y = getAbsInnerTop(0);
  for( int16_t i = 0; i < len; ++i) {
    GFXObject* g = list[i];
    if (g->getVisible()) {
      g->doDraw(&clip, x, y, redraw);
    }
  }
}

void GFXButton::drawAll(bool redraw)
{
  if (screenEnabled) {
    if (getAbsVisible()) {
      innerDraw(redraw);
    }
  }
}
