/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "GFXObject.h"
#include "Display.h"

/// @GFXPixel

class GFXPixel : public GFXObject {
public:
  GFXPixel(Button* parent, int16_t ax1, int16_t ay1, rgb_t color)
   : GFXObject(parent, color), x1(ax1), y1(ay1)
  {
    over.size = 1;
  }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.drawPixel(clip, x + x1, y + y1, getOver());
  }

  void setX1(int16_t x) { x1 = x; }
  int16_t getX1() { return x1; }

  void setY1(int16_t y) { y1 = y; }
  int16_t getY1() { return y1; }

protected:
  int16_t x1, y1;
};


/// @GFXLine

class GFXLine : public GFXPixel {
public:
  GFXLine(Button* parent, int16_t ax1, int16_t ay1, int16_t ax2, int16_t ay2, rgb_t color, int16_t ats)
   : GFXPixel(parent, ax1, ay1, color), x2(ax2), y2(ay2), ts(ats)
   {
    over.size = (abs(x2 - x1) + abs (y2 - y1)) * 1.2 * ts;
   }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.drawLine(clip, x + x1, y + y1, x + x2, y + y2, getOver(), ts, mode);
  }

  uint8_t mode = 0;   // 0 flat, 1 cut

  void setX2(int16_t x) { x2 = x; }
  int16_t getX2() { return x2; }

  void setY2(int16_t y) { y2 = y; }
  int16_t getY2() { return y2; }

protected:
  int16_t x2, y2, ts;
};


/// @GFXRect

class GFXRect : public GFXObject {
public:
  GFXRect(Button* parent, int16_t ax1, int16_t ay1, int16_t aw, int16_t ah, rgb_t color, int16_t ats)
   : GFXObject(parent, color), x1(ax1), y1(ay1), w(aw), h(ah), ts(ats)
  {
    over.size = (2 * w + ts + 2 * h + ts) * ts;
  }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.drawRect(clip, x + x1, y + y1, w, h, getOver(), ts);
  }

  void setX1(int16_t x) { x1 = x; }
  int16_t getX1() { return x1; }

  void setY1(int16_t y) { y1 = y; }
  int16_t getY1() { return y1; }

  void setW(int16_t aw) { w = aw; }
  int16_t getW() { return w; }

  void setH(int16_t ah) { h = ah; }
  int16_t getH() { return h; }

protected:
  int16_t x1, y1, w, h, ts;
};


/// @GFXRoundRect

class GFXRoundRect : public GFXRect {
public:
  GFXRoundRect(Button* parent, int16_t x1, int16_t y1, int16_t w, int16_t h, int16_t aradius, rgb_t color, int16_t ts)
   : GFXRect(parent, x1, y1, w, h, color, ts), radius(aradius)
   {
     over.size = (2 * w + ts + 2 * h + ts) * ts;
   }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.drawRoundRect(clip, x + x1, y + y1, w, h, radius, getOver(), ts);
  }

  void setRound(int16_t r) { radius = r; }
  int16_t getRound() { return radius; }

protected:
  int16_t radius;
};


/// @GFXFillRect

class GFXFillRect : public GFXRect {
public:
  GFXFillRect(Button* parent, int16_t x1, int16_t y1, int16_t w, int16_t h, rgb_t color)
   : GFXRect(parent, x1, y1, w, h, color, 0)
   {
     over.size = w * h;
   }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.fillRect(clip, x + x1, y + y1, w, h, getOver());
  }
};


/// @GFXFillRectGradient

class GFXFillRectGradient : public GFXRect {
public:
  GFXFillRectGradient(Button* parent, int16_t x1, int16_t y1, int16_t w, int16_t h, gradient_t* z)
   : GFXRect(parent, x1, y1, w, h, z->color1, 0), gradient(*z)  {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.fillRectGradient(clip, x + x1, y + y1, w, h, &gradient);
  }

  gradient_t gradient;
};

/// @GFXFillRoundRect

class GFXFillRoundRect : public GFXRoundRect {
public:
  GFXFillRoundRect(Button* parent, int16_t x1, int16_t y1, int16_t w, int16_t h, int16_t radius, rgb_t color)
   : GFXRoundRect(parent, x1, y1, w, h, radius, color, 0)
   {
     over.size = w * h;
   }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.fillRoundRect(clip, x + x1, y + y1, w, h, radius, getOver());
  }

};


/// @GFXCircle

class GFXCircle : public GFXObject {
public:
  GFXCircle(Button* parent, int16_t ax0, int16_t ay0, int16_t ar, rgb_t color, int16_t ats)
   : GFXObject(parent, color), x0(ax0), y0(ay0), r(ar), ts(ats)
   {
     over.size = 2 * r * 3.5 * ts;
   }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.drawCircle(clip, x + x0, y + y0, r, getOver(), ts);
  }

  void setX0(int16_t x) { x0 = x; }
  int16_t getX0() { return x0; }

  void setY0(int16_t y) { y0 = y; }
  int16_t getY0() { return y0; }

  void setR(int16_t ar) { r = ar; }
  int16_t getR() { return r; }

protected:
  int16_t x0, y0, r, ts;
};


/// @GFXCircleFragment

class GFXCircleFragment : public GFXCircle {
public:
  GFXCircleFragment(Button* parent, int16_t x0, int16_t y0, int16_t r, uint8_t afragment, rgb_t color, int16_t ts)
   : GFXCircle(parent, x0, y0, r, color, ts), fragment(afragment)
   {
     over.size = 2 * r * 3.5 * ts;
   }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.drawCircleFragment(clip, x + x0, y + y0, r, fragment, getOver(), ts);
  }

protected:
  int8_t fragment;
};


/// @GFXFillCircle

class GFXFillCircle : public GFXCircle {
public:
  GFXFillCircle(Button* parent, int16_t x0, int16_t y0, int16_t r, rgb_t color)
   : GFXCircle(parent, x0, y0, r, color, 0)
   {
     over.size = r * r * 3.5;
   }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.fillCircle(clip, x + x0, y + y0, r, getOver());
  }
};


/// @GFXFillCircleFragment

class GFXFillCircleFragment : public GFXCircleFragment {
public:
  GFXFillCircleFragment(Button* parent, int16_t x0, int16_t y0, int16_t r, uint8_t fragment, int16_t adelta, rgb_t color)
   : GFXCircleFragment(parent, x0, y0, r, fragment, color, 0), delta(adelta)
   {
     over.size = r * r * 3.5;
   }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.fillCircleFragment(clip, x + x0, y + y0, r, fragment, delta, getOver());
  }

protected:
  int16_t delta;
};


/// @GFXTriangle

class GFXTriangle : public GFXLine {
public:
  GFXTriangle(Button* parent, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ax3, int16_t ay3, rgb_t color, int16_t ts)
   : GFXLine(parent, x1, y1, x2, y2, color, ts), x3(ax3), y3(ay3)
   {
     over.size = (abs(x2 - x1) + abs (y2 - x1) + abs (x3 - x2) + abs (y3 - y2) + abs (x1 - x3) + abs (y1 - y3)) * 1.2 * ts;
   }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.drawTriangle(clip, x + x1, y + y1, x + x2, y + y2, x + x3, y + y3, getOver(), ts, mode);
  }

  void setX3(int16_t x) { x3 = x; }
  int16_t getX3() { return x3; }

  void setY3(int16_t y) { y3 = y; }
  int16_t getY3() { return y3; }

protected:
  int16_t x3, y3;
};


/// @GFXFillTriangle

class GFXFillTriangle : public GFXTriangle {
public:
  GFXFillTriangle(Button* parent, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, rgb_t color)
   : GFXTriangle(parent, x1, y1, x2, y2, x3, y3, color, 0)
   {
    uint16_t aw = abs(x2 - x1);
    uint16_t bw = abs(x3 - x2);
    uint16_t cw = abs(x1 - x3);
    uint16_t w = aw >= bw ? aw >= cw ? aw : cw : bw > cw ? bw : cw;
    uint16_t ah = abs(y2 - y1);
    uint16_t bh = abs(y3 - y2);
    uint16_t ch = abs(y1 - y3);
    uint16_t h = ah >= bh ? ah >= ch ? ah : ch : bh > ch ? bh : ch;
    over.size = w * h / 2;
   }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    display.fillTriangle(clip, x + x1, y + y1, x + x2, y + y2, x + x3, y + y3, getOver());
  }

};


/// @GFXBitmapXY

class GFXBitmapXY : public GFXObject {
public:
  GFXBitmapXY(Button* parent, int16_t ax1, int16_t ay1, int16_t aw, int16_t ah)
   : GFXObject(parent, 0), x1(ax1), y1(ay1), w(aw), h(ah) {}

  void setX1(int16_t x) { x1 = x; }
  int16_t getX1() { return x1; }

  void setY1(int16_t y) { y1 = y; }
  int16_t getY1() { return y1; }

  void setW(int16_t aw) { w = aw; }
  int16_t getW() { return w; }

  void setH(int16_t ah) { h = ah; }
  int16_t getH() { return h; }

protected:
  int16_t x1, y1, w, h;
};


/// @GFXGrayscaleBitmap

class GFXGrayscaleBitmap : public GFXBitmapXY {
public:
  GFXGrayscaleBitmap(Button* parent, int16_t x1, int16_t y1, const uint8_t* abitmap, const uint8_t* amask, int16_t w, int16_t h)
   : GFXBitmapXY(parent, x1, y1, w, h), bitmap(abitmap), mask(amask)  {}

  GFXGrayscaleBitmap(Button* parent, int16_t x1, int16_t y1, const uint8_t* bitmap, int16_t w, int16_t h)
   : GFXGrayscaleBitmap(parent, x1, y1, bitmap, NULL, w, h) {}

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    if (mask) {
      display.drawGrayscaleBitmap(clip, x + x1, y + y1, bitmap, mask, w, h);
    }
    else {
      display.drawGrayscaleBitmap(clip, x + x1, y + y1, bitmap, w, h);
    }
  }

protected:
  const uint8_t* bitmap;
  const uint8_t* mask;
};


/// @GFXBitmap

class GFXBitmap : public GFXBitmapXY {
public:
  GFXBitmap(Button* parent, int16_t x1, int16_t y1, const uint8_t* abitmap, int16_t w, int16_t h, rgb_t acolor, rgb_t abg)
   : GFXBitmapXY(parent, x1, y1, w, h), bitmap(abitmap), color(acolor), bg(abg)
  {
    over.size = w * h;
  }

  GFXBitmap(Button* parent, int16_t x1, int16_t y1, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color)
   : GFXBitmap(parent, x1, y1, bitmap, w, h, color, 1)
  {
    over.size = w * h;
  }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    if (bg != 1) {
      display.drawBitmap(clip, x + x1, y + y1, bitmap, w, h, getOver(), bg);
    }
    else {
      display.drawBitmap(clip, x + x1, y + y1, bitmap, w, h, getOver());
    }
  }

protected:
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

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    if (mask) {
      display.drawRGBBitmap(clip, x + x1, y + y1, bitmap, mask, w, h);
    }
    else {
      display.drawRGBBitmap(clip, x + x1, y + y1, bitmap, w, h);
    }
  }

protected:
  const rgb_t* bitmap;
  const uint8_t* mask;
};


/// @GFXFontAndColor

class GFXCursorFontColor : public GFXObject {
public:
  GFXCursorFontColor(Button* parent, int16_t ax1, int16_t ay1, rgb_t color = WHITE)
   : GFXObject(parent, color), x1(ax1), y1(ay1)
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

  void setX1(int16_t x) { x1 = x; }
  int16_t getX1() { return x1; }

  void setY1(int16_t y) { y1 = y; }
  int16_t getY1() { return y1; }

protected:
  int16_t x1, y1;
  font_t font;
};


/// @GFXChar

class GFXChar : public GFXCursorFontColor {
public:
  GFXChar(Button* parent, int16_t x1, int16_t y1, const char *ac, rgb_t color = WHITE, rgb_t abg = BLACK)
   : GFXCursorFontColor(parent, x1, y1, color), c((char *)ac), bg(abg)
  {
    over.size = 100;
  }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    cursor_t cursor{(int16_t)(x + x1), (int16_t)(y + y1)};
    display.drawChar(clip, &cursor, &font, &c, getOver(), bg);
  }

private:
  char* c;
  rgb_t bg;
};


/// @GFXTextLine

class GFXTextLine : public GFXCursorFontColor {
public:
  GFXTextLine(Button* parent, int16_t x1, int16_t y1, const char* atext, rgb_t color = WHITE, rgb_t abg = BLACK)
   : GFXCursorFontColor(parent, x1, y1, color), text(atext), bg(abg)
  {
    over.size = 100;
  }

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y)
  {
    cursor_t cursor{(int16_t)(x + x1), (int16_t)(y + y1)};
    display.drawTextLine(clip, &cursor, &font, text, getOver(), bg);
  }

private:
  const char* text;
  rgb_t bg;
};
