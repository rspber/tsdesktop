/*
  TSD_SCREEN for TSDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

  Smooth graphics comes from Bodmer's TFT_eSPI

*/

#pragma once

#include <TSD_GFX.h>

//#define BLACK        RGB(    0,    0,    0)
#define NAVY         RGB(    0,    0, 0x7B)
#define BLUE         RGB(    0,    0, 0xFF)
#define DARK_GREEN   RGB(    0, 0x7D,    0)
#define DARK_CYAN    RGB(    0, 0x7D, 0x7B)
#define TEAL         RGB(    0, 0x80, 0x80)
#define GREEN        RGB(    0, 0x80,    0)
#define CYAN         RGB(    0, 0xFF, 0xFF)
#define DIMM_CYAN    RGB( 0x09, 0x73, 0xC0)
#define DODGER_BLUE  RGB( 0x1E, 0x90, 0xFF)
#define CADET_BLUE   RGB( 0x5F, 0x9E, 0xA0)
#define AQUA         RGB( 0x68, 0xC0, 0xE0)
#define MAROON       RGB( 0x7B,    0,    0)
#define PURPLE       RGB( 0x7B,    0, 0x7B)
#define OLIVE        RGB( 0x7B, 0x7D,    0)
#define GRAY         RGB( 0x7B, 0x7D, 0x7B)
#define GREY         RGB( 0x7B, 0x7D, 0x7B)
#define DARKGREY     RGB( 0x80, 0x80, 0x80)
#define LIME         RGB( 0x80, 0xFC,    0)
#define SKYBLUE      RGB( 0x87, 0xCE, 0xEB)
#define VIOLET       RGB( 0xA0, 0x30, 0xC8)
#define SILVER       RGB( 0xC0, 0xC0, 0x80)
#define BROWN        RGB( 0x80, 0x2A, 0x2A)
//#define BROWN        RGB( 0x96, 0x4B, 0x00)
#define GREEN_YELLOW RGB( 0xAD, 0xFF, 0x29)
#define LIGHT_GRAY   RGB( 0xD3, 0xD3, 0xD3)
#define LIGHT_GREY   RGB( 0xD3, 0xD3, 0xD3)
#define LIGHT_CYAN   RGB( 0xE0, 0xFF, 0xFF)
#define RED          RGB( 0xFF,    0,    0)
#define MAGENTA      RGB( 0xFF,    0, 0xFF)
#define CORAL        RGB( 0xFF, 0x7F, 0x50)
#define PINK         RGB( 0xFF, 0x82, 0xC6)
//#define ORANGE       RGB( 0xFF, 0x98,    0)
#define ORANGE       RGB( 0xFF, 0xA5,    0)
#define GOLDEN       RGB( 0xFF, 0xD7,    0)
#define YELLOW       RGB( 0xFF, 0xFF,    0)
#define LIGHT_YELLOW RGB( 0xFF, 0xFF, 0xE0)
//#define WHITE        RGB( 0xFF, 0xFF, 0xFF)

class TSD_SCREEN : public TSD_GFX {
public:
  TSD_SCREEN(const int32_t w, const int32_t h): TSD_GFX(), xWIDTHx(w), yHEIGHTy(h)
  {
    clip = {0, 0, w, h};
  }

  int32_t width() { return clip.width(); }
  int32_t height() { return clip.height(); }

  int32_t getWIDTH() { return xWIDTHx; }
  int32_t getHEIGHT() { return yHEIGHTy; }

  void setSize(const int32_t w, const int32_t h)
  {
    clip.x2 = clip.x1 + w;
    clip.y2 = clip.y1 + h;
  }

  void fillScreen(const rgb_t color = BLACK);
  void clearDisplay() { fillScreen(BLACK); }

  void pushMDTBuffer(clip_t& window, const uint8_t* buffer);
  void pushMDTBuffer(clip_t& window, const uint8_t* buffer, const rgb_t transparent);
  virtual void drawMDTBuffer(const int32_t x, const int32_t y, const int32_t w, const int32_t h, const uint8_t* buffer) = 0;

  void drawLine(clip_t& clip, int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgb_t color, int32_t ts, uint8_t mode); // thickness, mode:1 cut
  void drawRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color, int32_t ts); // thickness
  void drawTriangle(clip_t& clip, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, rgb_t color, int32_t ts, uint8_t mode); // thickness, mode:1 cut
  void drawRoundRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color, int32_t ts); // thickness
  void drawCircle(clip_t& clip, int32_t x0, int32_t y0, int32_t r, rgb_t color, int32_t ts); // thickness
  void drawCircleFragment(clip_t& clip, int32_t x0, int32_t y0, int32_t r, uint8_t corners, rgb_t color, int32_t ts); // thickness

  void drawPixel(int32_t x, int32_t y, rgb_t color);
  void drawFastVLine(int32_t x, int32_t y, int32_t h, rgb_t color);
  void drawFastHLine(int32_t x, int32_t y, int32_t w, rgb_t color);
  void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgb_t color, int32_t ts = 1, uint8_t mode = 0); // thickness, mode:1 cut
  void drawRect(int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color, int32_t ts = 1); // thickness
  void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color);
  void fillRectGradient(int32_t x, int32_t y, int32_t w, int32_t h, gradient_t& z);
  void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color, int32_t ts = 1); // thickness
  void fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color);
  void drawCircle(int32_t x0, int32_t y0, int32_t r, rgb_t color, int32_t ts = 1); // thickness
  void drawCircleFragment(int32_t x0, int32_t y0, int32_t r, uint8_t corners, rgb_t color, int32_t ts = 1); // thickness
  void fillCircle(int32_t x0, int32_t y0, int32_t r, rgb_t color);
  void fillCircleFragment(int32_t x0, int32_t y0, int32_t r, uint8_t corners, int32_t delta, rgb_t color);
  void drawEllipse(int32_t x0, int32_t y0, int32_t rx, int32_t ry, rgb_t color);
  void fillEllipse(int32_t x0, int32_t y0, int32_t rx, int32_t ry, rgb_t color);
  void drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, rgb_t color, int32_t ts = 1, uint8_t mode = 0); // thickness, mode:1 cut
  void fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, rgb_t color);

public:
  void drawBitmap(int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h, rgb_t color);
  void drawBitmap(int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h, rgb_t color, rgb_t bg);
  void drawGrayscaleBitmap(int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h);
  void drawGrayscaleBitmap(int32_t x, int32_t y, const uint8_t* bitmap, const uint8_t* mask, int32_t w, int32_t h);
  void drawRGBBitmap(int32_t x, int32_t y, const uint16_t* bitmap, int32_t w, int32_t h); // 565 color
  void drawRGBBitmap(int32_t x, int32_t y, const uint16_t* bitmap, const uint8_t *mask, int32_t w, int32_t h); // 565 color
  void drawRGBBitmap(int32_t x, int32_t y, const uint32_t* bitmap, int32_t w, int32_t h); // 666 color
  void drawRGBBitmap(int32_t x, int32_t y, const uint32_t* bitmap, const uint8_t *mask, int32_t w, int32_t h); // 666 color

public:
  void pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
  void pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint16_t transp);

public:
  // utf-8
  void drawChar(cursor_t& cursor, tsd_font_t& font, const char* c, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);
  void drawTextLine(cursor_t& cursor, tsd_font_t& font, const char* text, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);

  void drawText(const int32_t x, const int32_t y, const char* text, const int8_t fontSize = 1, rgb_t colorh = WHITE, rgb_t bg = BLACK, rgb_t colorl = BLACK, const int8_t spacing = 0);
  void drawText(const int32_t x, const int32_t y, const char* text, const TSD_GFXfont** gfxFont, const int8_t fontSize = 1, rgb_t colorh = WHITE, rgb_t bg = BLACK, rgb_t colorl = BLACK, const int8_t spacing = 0);

  // unicode
  void drawChar(cursor_t& cursor, tsd_font_t& font, const uint16_t uchar, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);
  const uint16_t* drawTextLine(cursor_t& cursor, tsd_font_t& font, const uint16_t* utext, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);

  void drawText(const int32_t x, const int32_t y, const uint16_t* utext, const int8_t fontSize = 1, rgb_t colorh = WHITE, rgb_t bg = BLACK, rgb_t colorl = BLACK, const int8_t spacing = 0);
  void drawText(const int32_t x, const int32_t y, const uint16_t* utext, const TSD_GFXfont** gfxFont, const int8_t fontSize = 1, rgb_t colorh = WHITE, rgb_t bg = BLACK, rgb_t colorl = BLACK, const int8_t spacing = 0);

#include "TSD_Smooth.h.hh"

public:
 // using TSD_GFX::drawPixel;
  using TSD_GFX::drawFastVLine;
  using TSD_GFX::drawFastHLine;
  using TSD_GFX::drawLine;
  using TSD_GFX::drawRect;
  using TSD_GFX::fillRect;
  using TSD_GFX::fillRectGradient;
  using TSD_GFX::drawRoundRect;
  using TSD_GFX::fillRoundRect;
  using TSD_GFX::drawEllipse;
  using TSD_GFX::fillEllipse;
  using TSD_GFX::drawCircle;
  using TSD_GFX::drawCircleFragment;
  using TSD_GFX::fillCircle;
  using TSD_GFX::fillCircleFragment;
  using TSD_GFX::drawTriangle;
  using TSD_GFX::fillTriangle;
  using TSD_GFX::drawBitmap;
  using TSD_GFX::drawGrayscaleBitmap;
  using TSD_GFX::drawRGBBitmap;
  using TSD_GFX::drawChar;
  using TSD_GFX::drawTextLine;
  using TSD_GFX::pushImage;

  void setPivot(const int16_t x, const int16_t y) {
    xPivotx = x;
    yPivoty = y;
  }

  const int16_t getPivotX() { return xPivotx; }
  const int16_t getPivotY() { return yPivoty; }

private:
  int32_t xWIDTHx, yHEIGHTy;
  int16_t xPivotx, yPivoty;

public:
  clip_t clip;      ///< Display width/height as modified by rotation
};

           // Alpha blend 2 colours, see generic "alphaBlend_Test" example
           // alpha =   0 = 100% background colour
           // alpha = 255 = 100% foreground colour
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

           // 16-bit colour alphaBlend with alpha dither (dither reduces colour banding)
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);
           // 24-bit colour alphaBlend with optional alpha dither
  uint32_t alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither = 0);

