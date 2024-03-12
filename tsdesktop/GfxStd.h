/*
  GfxStd

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

*/

#pragma once

#include "Canvas.h"

/// @GfxPixel

class GfxPixel : public CanvasGfxObject {
public:
  GfxPixel(int16_t ax1, int16_t ay1, rgb_t color)
   : CanvasGfxObject(ax1, ay1, color) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->drawPixel(clip, x + x1, y + y1, getOver());
  }

  int16_t lW() override { return 0; }
  int16_t rW() override { return 1; }
  int16_t tH() override { return 0; }
  int16_t bH() override { return 1; }
};


/// @GfxLine

class GfxLine : public CanvasGfxObject {
public:
  GfxLine(int16_t ax1, int16_t ay1, int16_t ax2, int16_t ay2, rgb_t color, int16_t ats = 1)
   : CanvasGfxObject(ax1, ay1, color), x2(ax2), y2(ay2), ts(ats) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->drawLine(clip, x + x1, y + y1, x + x2, y + y2, getOver(), ts, mode);
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
  int16_t x2, y2;
  int16_t ts;
};


/// @GfxXYWH

class GfxXYWH : public CanvasGfxObject {
public:
  GfxXYWH(int16_t ax1, int16_t ay1, int16_t aw, int16_t ah, rgb_t color)
   : CanvasGfxObject(ax1, ay1, color), w(aw), h(ah) {}

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



/// @GfxRect

class GfxRect : public GfxXYWH {
public:
  GfxRect(int16_t ax1, int16_t ay1, int16_t aw, int16_t ah, rgb_t color, int16_t ats)
   : GfxXYWH(ax1, ay1, aw, ah, color), ts(ats) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->drawRect(clip, x + x1, y + y1, w, h, getOver(), ts);
  }

  int16_t lW() override { return ts/2; }
  int16_t rW() override { return w + ts/2; }
  int16_t tH() override { return ts/2; }
  int16_t bH() override { return h + ts/2; }

protected:
  int16_t ts;
};


/// @GfxRoundRect

class GfxRoundRect : public GfxRect {
public:
  GfxRoundRect(int16_t x1, int16_t y1, int16_t w, int16_t h, int16_t aradius, rgb_t color, int16_t ts)
   : GfxRect(x1, y1, w, h, color, ts), radius(aradius) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->drawRoundRect(clip, x + x1, y + y1, w, h, radius, getOver(), ts);
  }

  void setRound(int16_t r) { radius = r; }
  int16_t getRound() { return radius; }

protected:
  int16_t radius;
};


/// @GfxFillRect

class GfxFillRect : public GfxXYWH {
public:
  GfxFillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, rgb_t color)
   : GfxXYWH(x1, y1, w, h, color) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->fillRect(clip, x + x1, y + y1, w, h, getOver());
  }
};


/// @GfxFillRectGradient

class GfxFillRectGradient : public GfxXYWH {
public:
  GfxFillRectGradient(int16_t x1, int16_t y1, int16_t w, int16_t h, gradient_t& z)
   : GfxXYWH(x1, y1, w, h, z.color1), gradient(z)  {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->fillRectGradient(clip, x + x1, y + y1, w, h, gradient);
  }

  gradient_t gradient;
};

/// @GfxFillRoundRect

class GfxFillRoundRect : public GfxRoundRect {
public:
  GfxFillRoundRect(int16_t x1, int16_t y1, int16_t w, int16_t h, int16_t radius, rgb_t color)
   : GfxRoundRect(x1, y1, w, h, radius, color, 0) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->fillRoundRect(clip, x + x1, y + y1, w, h, radius, getOver());
  }
};


/// @GfxCircle

class GfxCircle : public GfxPixel {
public:
  GfxCircle(int16_t ax1, int16_t ay1, int16_t ar, rgb_t color, int16_t ats)
   : GfxPixel(ax1, ay1, color), r(ar), ts(ats) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->drawCircle(clip, x + x1, y + y1, r, getOver(), ts);
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


/// @GfxCircleFragment

class GfxCircleFragment : public GfxCircle {
public:
  GfxCircleFragment(int16_t x1, int16_t y1, int16_t r, uint8_t afragment, rgb_t color, int16_t ts)
   : GfxCircle(x1, y1, r, color, ts), fragment(afragment) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->drawCircleFragment(clip, x + x1, y + y1, r, fragment, getOver(), ts);
  }

protected:
  int8_t fragment;
};


/// @GfxFillCircle

class GfxFillCircle : public GfxCircle {
public:
  GfxFillCircle(int16_t x1, int16_t y1, int16_t r, rgb_t color)
   : GfxCircle(x1, y1, r, color, 0) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->fillCircle(clip, x + x1, y + y1, r, getOver());
  }
};


/// @GfxFillCircleFragment

class GfxFillCircleFragment : public GfxCircleFragment {
public:
  GfxFillCircleFragment(int16_t x1, int16_t y1, int16_t r, uint8_t fragment, int16_t adelta, rgb_t color)
   : GfxCircleFragment(x1, y1, r, fragment, color, 0), delta(adelta) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->fillCircleFragment(clip, x + x1, y + y1, r, fragment, delta, getOver());
  }

protected:
  int16_t delta;
};


/// @GfxEllipse

class GfxEllipse : public GfxPixel {
public:
  GfxEllipse(int16_t ax1, int16_t ay1, int16_t arx, int16_t ary, rgb_t color)
   : GfxPixel(ax1, ay1, color), rx(arx), ry(ary) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->drawEllipse(clip, x + x1, y + y1, rx, ry, getOver());
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


/// @GfxFillEllipse

class GfxFillEllipse : public GfxEllipse {
public:
  GfxFillEllipse(int16_t x1, int16_t y1, int16_t rx, int16_t ry, rgb_t color)
   : GfxEllipse(x1, y1, rx, ry, color) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->fillEllipse(clip, x + x1, y + y1, rx, ry, getOver());
  }
};



/// @GfxTriangle

class GfxTriangle : public GfxLine {
public:
  GfxTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ax3, int16_t ay3, rgb_t color, int16_t ts)
   : GfxLine(x1, y1, x2, y2, color, ts), x3(ax3), y3(ay3) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->drawTriangle(clip, x + x1, y + y1, x + x2, y + y2, x + x3, y + y3, getOver(), ts, mode);
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


/// @GfxFillTriangle

class GfxFillTriangle : public GfxTriangle {
public:
  GfxFillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, rgb_t color)
   : GfxTriangle(x1, y1, x2, y2, x3, y3, color, 0) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    screen()->fillTriangle(clip, x + x1, y + y1, x + x2, y + y2, x + x3, y + y3, getOver());
  }

};


/// @GfxGrayscaleBitmap

class GfxGrayscaleBitmap : public GfxXYWH {
public:
  GfxGrayscaleBitmap(int16_t x1, int16_t y1, const uint8_t* abitmap, const uint8_t* amask, int16_t w, int16_t h)
   : GfxXYWH(x1, y1, w, h, BLACK), bitmap(abitmap), mask(amask)  {}

  GfxGrayscaleBitmap(int16_t x1, int16_t y1, const uint8_t* bitmap, int16_t w, int16_t h)
   : GfxGrayscaleBitmap(x1, y1, bitmap, NULL, w, h) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    if (mask) {
      screen()->drawGrayscaleBitmap(clip, x + x1, y + y1, bitmap, mask, w, h);
    }
    else {
      screen()->drawGrayscaleBitmap(clip, x + x1, y + y1, bitmap, w, h);
    }
  }

protected:
  const uint8_t* bitmap;
  const uint8_t* mask;
};


/// @GfxBitmap

class GfxBitmap : public GfxXYWH {
public:
  GfxBitmap(int16_t x1, int16_t y1, const uint8_t* abitmap, int16_t w, int16_t h, rgb_t acolor, rgb_t abg)
   : GfxXYWH(x1, y1, w, h, BLACK), bitmap(abitmap), color(acolor), bg(abg) {}

  GfxBitmap(int16_t x1, int16_t y1, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color)
   : GfxBitmap(x1, y1, bitmap, w, h, color, 1) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    if (bg != 1) {
      screen()->drawBitmap(clip, x + x1, y + y1, bitmap, w, h, getOver(), bg);
    }
    else {
      screen()->drawBitmap(clip, x + x1, y + y1, bitmap, w, h, getOver());
    }
  }

protected:
  const uint8_t* bitmap;
  rgb_t color, bg;
};


/// @Gfx565Bitmap

class Gfx565Bitmap : public GfxXYWH {
public:
  Gfx565Bitmap(int16_t x1, int16_t y1, const uint16_t* abitmap, const uint8_t* amask, int16_t w, int16_t h)
   : GfxXYWH(x1, y1, w, h, BLACK), bitmap(abitmap), mask(amask) {}

  Gfx565Bitmap(int16_t x1, int16_t y1, const uint16_t* bitmap, int16_t w, int16_t h)
   : Gfx565Bitmap(x1, y1, bitmap, NULL, w, h) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    if (mask) {
      screen()->drawRGBBitmap(clip, x + x1, y + y1, bitmap, mask, w, h);
    }
    else {
      screen()->drawRGBBitmap(clip, x + x1, y + y1, bitmap, w, h);
    }
  }

protected:
  const uint16_t* bitmap;
  const uint8_t* mask;
};


/// @GfxRGBBitmap

class GfxRGBBitmap : public GfxXYWH {
public:
  GfxRGBBitmap(int16_t x1, int16_t y1, const uint32_t* abitmap, const uint8_t* amask, int16_t w, int16_t h)
   : GfxXYWH(x1, y1, w, h, BLACK), bitmap(abitmap), mask(amask) {}

  GfxRGBBitmap(int16_t x1, int16_t y1, const uint32_t* bitmap, int16_t w, int16_t h)
   : GfxRGBBitmap(x1, y1, bitmap, NULL, w, h) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    if (mask) {
      screen()->drawRGBBitmap(clip, x + x1, y + y1, bitmap, mask, w, h);
    }
    else {
      screen()->drawRGBBitmap(clip, x + x1, y + y1, bitmap, w, h);
    }
  }

protected:
  const uint32_t* bitmap;
  const uint8_t* mask;
};


/// @GfxFontAndColor

class GfxCursorFontColor : public GfxPixel {
public:
  GfxCursorFontColor(int16_t ax1, int16_t ay1, rgb_t color = WHITE)
   : GfxPixel(ax1, ay1, color)
  {
    font.setFont(NULL);
    font.fontSizeX = 1;
    font.fontSizeY = 1;
  }

  void setFont(const GFXfont** GFXfont, int8_t fontSizeX, int8_t fontSizeY)
  {
    font.setFont(GFXfont);
    font.fontSizeX = fontSizeX;
    font.fontSizeY = fontSizeY;
  }

  void setFont(const GFXfont** GFXfont, int8_t fontSize = 1)
  {
    setFont(GFXfont, fontSize, fontSize);
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


/// @GfxStaticTextLine

class GfxStaticTextLine : public GfxCursorFontColor {
public:
  GfxStaticTextLine(int16_t x1, int16_t y1, const char* atext, rgb_t colorh, rgb_t abg = BLACK, rgb_t acolorl = BLACK, const int8_t aspacing = 0)
   : GfxCursorFontColor(x1, y1, colorh), text((char*)atext), bg(abg), colorl(acolorl), spacing(aspacing) {}

  void dodraw(clip_t& clip, int16_t left, int16_t top) override
  {
    int x = clip.x1 + left;
    int y = clip.y1 + top;
    cursor_t cursor{(int16_t)(x + x1), (int16_t)(y + y1)};
    screen()->drawTextLine(clip, cursor, font, text, getOver(), bg, colorl, spacing);
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

/// @GfxTextLine

class GfxTextLine : public GfxStaticTextLine {
public:
  GfxTextLine(int16_t x1, int16_t y1, const char* atext, rgb_t colorh, rgb_t abg = BLACK, rgb_t acolorl = BLACK, const int8_t aspacing = 0)
   : GfxStaticTextLine(x1, y1, 0, colorh, abg, acolorl, aspacing)
  {
    setText(atext);
  }
};



/// @GfxCanvas
// TODO
class GfxCanvasCanvas : public Canvas {
public:
  GfxCanvasCanvas()
   : Canvas(0, 0, BLACK) {
  }

  void getInnerClip(clip_t& clip) override
  {
/*
    getCanvas()->getInnerClip(clip);

    clip.x1 += x1;
    clip.y1 += y1;
    if (clip.x1 + w < clip.x2) {
      clip.x2 = clip.x1 + w;
    }
    if (clip.y1 + h < clip.y2) {
      clip.y2 = clip.y1 + h;
    }
*/
  }

  void dodraw(clip_t& clip, int16_t left, int16_t top) override {}

  TSD_SCREEN* screen() override { return 0; }

  TSD_SCREEN* superScreen() override { return 0; }

  int16_t lW() override { return 0; }
  int16_t rW() override { return 0; }
  int16_t tH() override { return 0; }
  int16_t bH() override { return 0; }
};

// not finished yet
class GfxCanvas : public CanvasGfxObject {
public:
  GfxCanvas(int16_t ax1, int16_t ay1, int16_t aw, int16_t ah, rgb_t acolor)
   : CanvasGfxObject(ax1, ay1, acolor), w(aw), h(ah) {
  }

  void setW(int16_t aw) { w = aw; }
  int16_t getW() { return w; }

  void setH(int16_t ah) { h = ah; }
  int16_t getH() { return h; }

  int16_t lW() override { return 0; }
  int16_t rW() override { return w; }
  int16_t tH() override { return 0; }
  int16_t bH() override { return h; }

protected:
  GfxCanvasCanvas canvas;

  int16_t w, h;
};
