/*
  GFXStd

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include "GFXObject.h"

/// @GFXPixel

class GFXPixel : public GFXObject {
public:
  GFXPixel(int16_t ax1, int16_t ay1, rgb_t color)
   : GFXObject(ax1, ay1, color) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->drawPixel(clip, x + x1, y + y1, getOver());
  }

  int16_t lW() override { return 0; }
  int16_t rW() override { return 1; }
  int16_t tH() override { return 0; }
  int16_t bH() override { return 1; }
};


/// @GFXLine

class GFXLine : public GFXPixel {
public:
  GFXLine(int16_t ax1, int16_t ay1, int16_t ax2, int16_t ay2, rgb_t color, int16_t ats = 1)
   : GFXPixel(ax1, ay1, color), x2(ax2), y2(ay2), ts(ats) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->drawLine(clip, x + x1, y + y1, x + x2, y + y2, getOver(), ts, mode);
  }

  uint8_t mode = 0;   // 0 flat, 1 cut

  void setX2(int16_t x) { x2 = x; }
  int16_t getX2() { return x2; }

  void setY2(int16_t y) { y2 = y; }
  int16_t getY2() { return y2; }

  int16_t lW() override { return ts/2; }
  int16_t rW() override { return x2 - x1 + ts/2; }
  int16_t tH() override { return ts/2; }
  int16_t bH() override { return y2 - y1 + ts/2; }

protected:
  int16_t x2, y2, ts;
};


/// @GFXRect

class GFXRect : public GFXPixel {
public:
  GFXRect(int16_t ax1, int16_t ay1, int16_t aw, int16_t ah, rgb_t color, int16_t ats)
   : GFXPixel(ax1, ay1, color), w(aw), h(ah), ts(ats) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->drawRect(clip, x + x1, y + y1, w, h, getOver(), ts);
  }

  void setW(int16_t aw) { w = aw; }
  int16_t getW() { return w; }

  void setH(int16_t ah) { h = ah; }
  int16_t getH() { return h; }

  int16_t lW() override { return ts/2; }
  int16_t rW() override { return w + ts/2; }
  int16_t tH() override { return ts/2; }
  int16_t bH() override { return h + ts/2; }

protected:
  int16_t w, h, ts;
};


/// @GFXRoundRect

class GFXRoundRect : public GFXRect {
public:
  GFXRoundRect(int16_t x1, int16_t y1, int16_t w, int16_t h, int16_t aradius, rgb_t color, int16_t ts)
   : GFXRect(x1, y1, w, h, color, ts), radius(aradius) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->drawRoundRect(clip, x + x1, y + y1, w, h, radius, getOver(), ts);
  }

  void setRound(int16_t r) { radius = r; }
  int16_t getRound() { return radius; }

protected:
  int16_t radius;
};


/// @GFXFillRect

class GFXFillRect : public GFXRect {
public:
  GFXFillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, rgb_t color)
   : GFXRect(x1, y1, w, h, color, 0) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->fillRect(clip, x + x1, y + y1, w, h, getOver());
  }
};


/// @GFXFillRectGradient

class GFXFillRectGradient : public GFXRect {
public:
  GFXFillRectGradient(int16_t x1, int16_t y1, int16_t w, int16_t h, gradient_t& z)
   : GFXRect(x1, y1, w, h, z.color1, 0), gradient(z)  {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->fillRectGradient(clip, x + x1, y + y1, w, h, gradient);
  }

  gradient_t gradient;
};

/// @GFXFillRoundRect

class GFXFillRoundRect : public GFXRoundRect {
public:
  GFXFillRoundRect(int16_t x1, int16_t y1, int16_t w, int16_t h, int16_t radius, rgb_t color)
   : GFXRoundRect(x1, y1, w, h, radius, color, 0) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->fillRoundRect(clip, x + x1, y + y1, w, h, radius, getOver());
  }
};


/// @GFXCircle

class GFXCircle : public GFXPixel {
public:
  GFXCircle(int16_t ax1, int16_t ay1, int16_t ar, rgb_t color, int16_t ats)
   : GFXPixel(ax1, ay1, color), r(ar), ts(ats) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->drawCircle(clip, x + x1, y + y1, r, getOver(), ts);
  }

  void setR(int16_t ar) { r = ar; }
  int16_t getR() { return r; }

  int16_t lW() override { return r + ts/2; }
  int16_t rW() override { return r + ts/2; }
  int16_t tH() override { return r + ts/2; }
  int16_t bH() override { return r + ts/2; }

protected:
  int16_t r, ts;
};


/// @GFXCircleFragment

class GFXCircleFragment : public GFXCircle {
public:
  GFXCircleFragment(int16_t x1, int16_t y1, int16_t r, uint8_t afragment, rgb_t color, int16_t ts)
   : GFXCircle(x1, y1, r, color, ts), fragment(afragment) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->drawCircleFragment(clip, x + x1, y + y1, r, fragment, getOver(), ts);
  }

protected:
  int8_t fragment;
};


/// @GFXFillCircle

class GFXFillCircle : public GFXCircle {
public:
  GFXFillCircle(int16_t x1, int16_t y1, int16_t r, rgb_t color)
   : GFXCircle(x1, y1, r, color, 0) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->fillCircle(clip, x + x1, y + y1, r, getOver());
  }
};


/// @GFXFillCircleFragment

class GFXFillCircleFragment : public GFXCircleFragment {
public:
  GFXFillCircleFragment(int16_t x1, int16_t y1, int16_t r, uint8_t fragment, int16_t adelta, rgb_t color)
   : GFXCircleFragment(x1, y1, r, fragment, color, 0), delta(adelta) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->fillCircleFragment(clip, x + x1, y + y1, r, fragment, delta, getOver());
  }

protected:
  int16_t delta;
};


/// @GFXEllipse

class GFXEllipse : public GFXPixel {
public:
  GFXEllipse(int16_t ax1, int16_t ay1, int16_t arx, int16_t ary, rgb_t color)
   : GFXPixel(ax1, ay1, color), rx(arx), ry(ary) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->drawEllipse(clip, x + x1, y + y1, rx, ry, getOver());
  }

  void setRX(int16_t x) { rx = x; }
  int16_t getRX() { return rx; }

  void setRY(int16_t y) { ry = y; }
  int16_t getRY() { return ry; }

  int16_t lW() override { return rx; }
  int16_t rW() override { return rx; }
  int16_t tH() override { return ry; }
  int16_t bH() override { return ry; }

protected:
  int16_t rx, ry;
};


/// @GFXFillEllipse

class GFXFillEllipse : public GFXEllipse {
public:
  GFXFillEllipse(int16_t x1, int16_t y1, int16_t rx, int16_t ry, rgb_t color)
   : GFXEllipse(x1, y1, rx, ry, color) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->fillEllipse(clip, x + x1, y + y1, rx, ry, getOver());
  }
};



/// @GFXTriangle

class GFXTriangle : public GFXLine {
public:
  GFXTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ax3, int16_t ay3, rgb_t color, int16_t ts)
   : GFXLine(x1, y1, x2, y2, color, ts), x3(ax3), y3(ay3) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->drawTriangle(clip, x + x1, y + y1, x + x2, y + y2, x + x3, y + y3, getOver(), ts, mode);
  }

  void setX3(int16_t x) { x3 = x; }
  int16_t getX3() { return x3; }

  void setY3(int16_t y) { y3 = y; }
  int16_t getY3() { return y3; }

  int16_t lW() override { return 0; }
  int16_t rW() override { return x2 > x3 ? x2 : x3; }
  int16_t tH() override { return 0; }
  int16_t bH() override { return y2 > y3 ? y2 : y3; }

protected:
  int16_t x3, y3;
};


/// @GFXFillTriangle

class GFXFillTriangle : public GFXTriangle {
public:
  GFXFillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, rgb_t color)
   : GFXTriangle(x1, y1, x2, y2, x3, y3, color, 0) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    writer()->fillTriangle(clip, x + x1, y + y1, x + x2, y + y2, x + x3, y + y3, getOver());
  }

};


/// @GFXBitmapXY

class GFXBitmapXY : public GFXPixel {
public:
  GFXBitmapXY(int16_t ax1, int16_t ay1, int16_t aw, int16_t ah)
   : GFXPixel(ax1, ay1, BLACK), w(aw), h(ah) {}

  void setW(int16_t aw) { w = aw; }
  int16_t getW() { return w; }

  void setH(int16_t ah) { h = ah; }
  int16_t getH() { return h; }

  int16_t lW() override { return 0; }
  int16_t rW() override { return w; }
  int16_t tH() override { return 0; }
  int16_t bH() override { return h; }

protected:
  int16_t w, h;
};


/// @GFXGrayscaleBitmap

class GFXGrayscaleBitmap : public GFXBitmapXY {
public:
  GFXGrayscaleBitmap(int16_t x1, int16_t y1, const uint8_t* abitmap, const uint8_t* amask, int16_t w, int16_t h)
   : GFXBitmapXY(x1, y1, w, h), bitmap(abitmap), mask(amask)  {}

  GFXGrayscaleBitmap(int16_t x1, int16_t y1, const uint8_t* bitmap, int16_t w, int16_t h)
   : GFXGrayscaleBitmap(x1, y1, bitmap, NULL, w, h) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    if (mask) {
      writer()->drawGrayscaleBitmap(clip, x + x1, y + y1, bitmap, mask, w, h);
    }
    else {
      writer()->drawGrayscaleBitmap(clip, x + x1, y + y1, bitmap, w, h);
    }
  }

protected:
  const uint8_t* bitmap;
  const uint8_t* mask;
};


/// @GFXBitmap

class GFXBitmap : public GFXBitmapXY {
public:
  GFXBitmap(int16_t x1, int16_t y1, const uint8_t* abitmap, int16_t w, int16_t h, rgb_t acolor, rgb_t abg)
   : GFXBitmapXY(x1, y1, w, h), bitmap(abitmap), color(acolor), bg(abg) {}

  GFXBitmap(int16_t x1, int16_t y1, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color)
   : GFXBitmap(x1, y1, bitmap, w, h, color, 1) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    if (bg != 1) {
      writer()->drawBitmap(clip, x + x1, y + y1, bitmap, w, h, getOver(), bg);
    }
    else {
      writer()->drawBitmap(clip, x + x1, y + y1, bitmap, w, h, getOver());
    }
  }

protected:
  const uint8_t* bitmap;
  rgb_t color, bg;
};


/// @GFX565Bitmap

class GFX565Bitmap : public GFXBitmapXY {
public:
  GFX565Bitmap(int16_t x1, int16_t y1, const uint16_t* abitmap, const uint8_t* amask, int16_t w, int16_t h)
   : GFXBitmapXY(x1, y1, w, h), bitmap(abitmap), mask(amask) {}

  GFX565Bitmap(int16_t x1, int16_t y1, const uint16_t* bitmap, int16_t w, int16_t h)
   : GFX565Bitmap(x1, y1, bitmap, NULL, w, h) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    if (mask) {
      writer()->drawRGBBitmap(clip, x + x1, y + y1, bitmap, mask, w, h);
    }
    else {
      writer()->drawRGBBitmap(clip, x + x1, y + y1, bitmap, w, h);
    }
  }

protected:
  const uint16_t* bitmap;
  const uint8_t* mask;
};


/// @GFXRGBBitmap

class GFXRGBBitmap : public GFXBitmapXY {
public:
  GFXRGBBitmap(int16_t x1, int16_t y1, const uint32_t* abitmap, const uint8_t* amask, int16_t w, int16_t h)
   : GFXBitmapXY(x1, y1, w, h), bitmap(abitmap), mask(amask) {}

  GFXRGBBitmap(int16_t x1, int16_t y1, const uint32_t* bitmap, int16_t w, int16_t h)
   : GFXRGBBitmap(x1, y1, bitmap, NULL, w, h) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    if (mask) {
      writer()->drawRGBBitmap(clip, x + x1, y + y1, bitmap, mask, w, h);
    }
    else {
      writer()->drawRGBBitmap(clip, x + x1, y + y1, bitmap, w, h);
    }
  }

protected:
  const uint32_t* bitmap;
  const uint8_t* mask;
};


/// @GFXFontAndColor

class GFXCursorFontColor : public GFXPixel {
public:
  GFXCursorFontColor(int16_t ax1, int16_t ay1, rgb_t color = WHITE)
   : GFXPixel(ax1, ay1, color)
  {
    font.setFont(NULL);
    font.fontSizeX = 1;
    font.fontSizeY = 1;
  }

  void setFont(const GFXfont** gfxFont, int8_t fontSizeX, int8_t fontSizeY)
  {
    font.setFont(gfxFont);
    font.fontSizeX = fontSizeX;
    font.fontSizeY = fontSizeY;
  }

  void setFont(const GFXfont** gfxFont, int8_t fontSize = 1)
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

protected:
  font_t font;
};


/// @GFXStaticTextLine

class GFXStaticTextLine : public GFXCursorFontColor {
public:
  GFXStaticTextLine(int16_t x1, int16_t y1, const char* atext, rgb_t colorh, rgb_t abg = BLACK, rgb_t acolorl = BLACK, const int8_t aspacing = 0)
   : GFXCursorFontColor(x1, y1, colorh), text((char*)atext), bg(abg), colorl(acolorl), spacing(aspacing) {}

  void dodraw(clip_t& clip, int16_t x, int16_t y) override
  {
    cursor_t cursor{(int16_t)(x + x1), (int16_t)(y + y1)};
    writer()->drawTextLine(clip, cursor, font, text, getOver(), bg, colorl, spacing);
  }

  void setText(const char* atext)
  {
    int len = strlen(atext) + 1;
    if (alloc) {
      text = (char*)realloc(text, len);
    }
    else {
      text = (char*)malloc(len);
      alloc = true;
    }
    memcpy(text, atext, len);
  }

  int16_t lW() override { return 0; }
  int16_t rW() override { return font.textLineWidth(text, false); }
  int16_t tH() override { return 0; }
  int16_t bH() override { return font.textLineHeight(); }

private:
  char* text;
  rgb_t bg, colorl;
  int8_t spacing;
  bool alloc = false;
};

/// @GFXTextLine

class GFXTextLine : public GFXStaticTextLine {
public:
  GFXTextLine(int16_t x1, int16_t y1, const char* atext, rgb_t colorh, rgb_t abg = BLACK, rgb_t acolorl = BLACK, const int8_t aspacing = 0)
   : GFXStaticTextLine(x1, y1, 0, colorh, abg, acolorl, aspacing)
  {
    setText(atext);
  }
};
