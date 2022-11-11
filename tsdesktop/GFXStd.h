/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "GFXObject.h"

/// @GFXPixel

class GFXPixel : public GFXObject {
public:
  GFXPixel(Button* parent, int16_t ax1, int16_t ay1, rgb_t color)
   : GFXObject(parent, color), x1(ax1), y1(ay1)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  int16_t x1, y1;
};


/// @GFXLine

class GFXLine : public GFXObject {
public:
  GFXLine(Button* parent, int16_t ax1, int16_t ay1, int16_t ax2, int16_t ay2, rgb_t color, int16_t ats)
   : GFXObject(parent, color), x1(ax1), y1(ay1), x2(ax2), y2(ay2), ts(ats)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  int16_t x1, y1, x2, y2, ts;
  uint8_t mode = 0;   // 0 flat, 1 arrow
};


/// @GFXRect

class GFXRect : public GFXObject {
public:
  GFXRect(Button* parent, int16_t ax1, int16_t ay1, int16_t aw, int16_t ah, rgb_t color, int16_t ats)
   : GFXObject(parent, color), x1(ax1), y1(ay1), w(aw), h(ah), ts(ats)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  int16_t x1, y1, w, h, ts;
};


/// @GFXRoundRect

class GFXRoundRect : public GFXRect {
public:
  GFXRoundRect(Button* parent, int16_t x1, int16_t y1, int16_t w, int16_t h, int16_t aradius, rgb_t color, int16_t ts)
   : GFXRect(parent, x1, y1, w, h, color, ts), radius(aradius)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  int16_t radius;
};


/// @GFXFillRect

class GFXFillRect : public GFXRect {
public:
  GFXFillRect(Button* parent, int16_t x1, int16_t y1, int16_t w, int16_t h, rgb_t color)
   : GFXRect(parent, x1, y1, w, h, color, 0)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);
};


/// @GFXFillRoundRect

class GFXFillRoundRect : public GFXRoundRect {
public:
  GFXFillRoundRect(Button* parent, int16_t x1, int16_t y1, int16_t w, int16_t h, int16_t radius, rgb_t color)
   : GFXRoundRect(parent, x1, y1, w, h, radius, color, 0)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);
};


/// @GFXCircle

class GFXCircle : public GFXObject {
public:
  GFXCircle(Button* parent, int16_t ax0, int16_t ay0, int16_t ar, rgb_t color, int16_t ats)
   : GFXObject(parent, color), x0(ax0), y0(ay0), r(ar), ts(ats)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  int16_t x0, y0, r, ts;
};


/// @GFXCircleFragment

class GFXCircleFragment : public GFXCircle {
public:
  GFXCircleFragment(Button* parent, int16_t x0, int16_t y0, int16_t r, uint8_t afragment, rgb_t color, int16_t ts)
   : GFXCircle(parent, x0, y0, r, color, ts), fragment(afragment) {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  int8_t fragment;
};


/// @GFXFillCircle

class GFXFillCircle : public GFXCircle {
public:
  GFXFillCircle(Button* parent, int16_t x0, int16_t y0, int16_t r, rgb_t color)
   : GFXCircle(parent, x0, y0, r, color, 0)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);
};


/// @GFXFillCircleFragment

class GFXFillCircleFragment : public GFXCircleFragment {
public:
  GFXFillCircleFragment(Button* parent, int16_t x0, int16_t y0, int16_t r, uint8_t fragment, int16_t adelta, rgb_t color)
   : GFXCircleFragment(parent, x0, y0, r, fragment, color, 0), delta(adelta) {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  int16_t delta;
};


/// @GFXTriangle

class GFXTriangle : public GFXLine {
public:
  GFXTriangle(Button* parent, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ax3, int16_t ay3, rgb_t color, int16_t ts)
   : GFXLine(parent, x1, y1, x2, y2, color, ts), x3(ax3), y3(ay3)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  int16_t x3, y3;
};


/// @GFXFillTriangle

class GFXFillTriangle : public GFXTriangle {
public:
  GFXFillTriangle(Button* parent, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, rgb_t color)
   : GFXTriangle(parent, x1, y1, x2, y2, x3, y3, color, 0)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);
};


/// @GFXBitmapXY

class GFXBitmapXY : public GFXObject {
public:
  GFXBitmapXY(Button* parent, int16_t ax1, int16_t ay1, int16_t aw, int16_t ah)
   : GFXObject(parent, 0), x1(ax1), y1(ay1), w(aw), h(ah) {}

  int16_t x1, y1, w, h;
};


/// @GFXGrayscaleBitmap

class GFXGrayscaleBitmap : public GFXBitmapXY {
public:
  GFXGrayscaleBitmap(Button* parent, int16_t x1, int16_t y1, const uint8_t* abitmap, const uint8_t* amask, int16_t w, int16_t h)
   : GFXBitmapXY(parent, x1, y1, w, h), bitmap(abitmap), mask(amask)  {}

  GFXGrayscaleBitmap(Button* parent, int16_t x1, int16_t y1, const uint8_t* bitmap, int16_t w, int16_t h)
   : GFXGrayscaleBitmap(parent, x1, y1, bitmap, NULL, w, h) {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  const uint8_t* bitmap;
  const uint8_t* mask;
};


/// @GFXBitmap

class GFXBitmap : public GFXBitmapXY {
public:
  GFXBitmap(Button* parent, int16_t x1, int16_t y1, const uint8_t* abitmap, int16_t w, int16_t h, rgb_t acolor, rgb_t abg)
   : GFXBitmapXY(parent, x1, y1, w, h), bitmap(abitmap), color(acolor), bg(abg) {}

  GFXBitmap(Button* parent, int16_t x1, int16_t y1, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color)
   : GFXBitmap(parent, x1, y1, bitmap, w, h, color, 1)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  const uint8_t* bitmap;
  rgb_t color, bg;
};


/// @GFXRGBBitmap

class GFXRGBBitmap : public GFXBitmapXY {
public:
  GFXRGBBitmap(Button* parent, int16_t x1, int16_t y1, const rgb_t* abitmap, const uint8_t* amask, int16_t w, int16_t h)
   : GFXBitmapXY(parent, x1, y1, w, h), bitmap(abitmap), mask(amask) {}

  GFXRGBBitmap(Button* parent, int16_t x1, int16_t y1, const rgb_t* bitmap, int16_t w, int16_t h)
   : GFXRGBBitmap(parent, x1, y1, bitmap, NULL, w, h) {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  const rgb_t* bitmap;
  const uint8_t* mask;
};


/// @GFXFontAndColor

class GFXCursorFontColor : public GFXObject {
public:
  GFXCursorFontColor(Button* parent, int16_t ax1, int16_t ay1, rgb_t color = WHITE)
   : GFXObject(parent, color), x1(ax1), y1(ay1)
  {
    font.gfxFont = NULL;
    font.fontSizeX = 1;
    font.fontSizeY = 1;
  }

  void setFont(const GFXfont* gfxFont, int8_t fontSizeX, int8_t fontSizeY)
  {
    font.gfxFont = (GFXfont*)gfxFont;
    font.fontSizeX = fontSizeX;
    font.fontSizeY = fontSizeY;
  }

  void setFont(const GFXfont* gfxFont, int8_t fontSize = 1)
  {
    setFont(gfxFont, fontSize, fontSize);
  }

  void setFontSize(int8_t fontSizeX, int8_t fontSizeY)
  {
    font.fontSizeX = fontSizeX;
    font.fontSizeY = fontSizeY;
  }

  void setFontSize(int8_t fontSize)
  {
    font.fontSizeX = fontSize;
    font.fontSizeY = fontSize;
  }

  int16_t x1, y1;
  font_t font;
};


/// @GFXChar

class GFXChar : public GFXCursorFontColor {
public:
  GFXChar(Button* parent, int16_t x1, int16_t y1, char ac, rgb_t color = WHITE)
   : GFXCursorFontColor(parent, x1, y1, color), c(ac) {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  char c;
};


/// @GFXTextLine

class GFXTextLine : public GFXCursorFontColor {
public:
  GFXTextLine(Button* parent, int16_t x1, int16_t y1, const char* atext, rgb_t color = WHITE)
   : GFXCursorFontColor(parent, x1, y1, color), text(atext) {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color);

  const char* text;
};
