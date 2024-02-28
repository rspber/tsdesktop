/*
  TSD_SCREEN for TSDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

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
#define LIME         RGB( 0x80, 0xFC,    0)
#define VIOLET       RGB( 0xA0, 0x30, 0xC8)
#define SILVER       RGB( 0xC0, 0xC0, 0x80)
#define BROWN        RGB( 0x80, 0x2A, 0x2A)
//#define BROWN        RGB( 0x96, 0x4B, 0x00)
#define GREEN_YELLOW RGB( 0xAD, 0xFF, 0x29)
#define LIGHT_GRAY   RGB( 0xD3, 0xD3, 0xD3)
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
  TSD_SCREEN(const int16_t w, const int16_t h)
  {
    clip = {0, 0, WIDTH = w, HEIGHT = h};
  }

  const int16_t width() { return clip.width(); }
  const int16_t height() { return clip.height(); }

  const int16_t getWIDTH() { return WIDTH; }
  const int16_t getHEIGHT() { return HEIGHT; }

  void setSize(const int16_t w, const int16_t h)
  {
    clip.x2 = clip.x1 + w;
    clip.y2 = clip.y1 + h;
  }

  void fillScreen(const rgb_t color = BLACK);
  void clearDisplay() { fillScreen(BLACK); }

  void pushMDTBuffer(clip_t& window, const uint8_t* buffer);
  void pushMDTBuffer(clip_t& window, const uint8_t* buffer, const rgb_t transparent);
  virtual void drawMDTBuffer(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const uint8_t* buffer) = 0;

  void drawLine(clip_t& clip, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts, uint8_t mode); // thickness, mode:1 cut
  void drawRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color, int16_t ts); // thickness
  void drawTriangle(clip_t& clip, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, rgb_t color, int16_t ts, uint8_t mode); // thickness, mode:1 cut
  void drawRoundRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, rgb_t color, int16_t ts); // thickness
  void drawCircle(clip_t& clip, int16_t x0, int16_t y0, int16_t r, rgb_t color, int16_t ts); // thickness
  void drawCircleFragment(clip_t& clip, int16_t x0, int16_t y0, int16_t r, uint8_t corners, rgb_t color, int16_t ts); // thickness

  void drawPixel(int16_t x, int16_t y, rgb_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, rgb_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, rgb_t color);
  void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts = 1, uint8_t mode = 0); // thickness, mode:1 cut
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color, int16_t ts = 1); // thickness
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color);
  void fillRectGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t& z);
  void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, rgb_t color, int16_t ts = 1); // thickness
  void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, rgb_t color);
  void drawCircle(int16_t x0, int16_t y0, int16_t r, rgb_t color, int16_t ts = 1); // thickness
  void drawCircleFragment(int16_t x0, int16_t y0, int16_t r, uint8_t corners, rgb_t color, int16_t ts = 1); // thickness
  void fillCircle(int16_t x0, int16_t y0, int16_t r, rgb_t color);
  void fillCircleFragment(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, rgb_t color);
  void drawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, rgb_t color);
  void fillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, rgb_t color);
  void drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, rgb_t color, int16_t ts = 1, uint8_t mode = 0); // thickness, mode:1 cut
  void fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, rgb_t color);
  void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color);
  void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color, rgb_t bg);
  void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h);
  void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t* bitmap, const uint8_t* mask, int16_t w, int16_t h);
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t* bitmap, int16_t w, int16_t h); // 565 color
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t* bitmap, const uint8_t *mask, int16_t w, int16_t h); // 565 color
  void drawRGBBitmap(int16_t x, int16_t y, const uint32_t* bitmap, int16_t w, int16_t h); // 666 color
  void drawRGBBitmap(int16_t x, int16_t y, const uint32_t* bitmap, const uint8_t *mask, int16_t w, int16_t h); // 666 color

  // utf-8
  void drawChar(cursor_t& cursor, font_t& font, const char* c, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);
  void drawTextLine(cursor_t& cursor, font_t& font, const char* text, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);

  void drawText(const int16_t x, const int16_t y, const char* text, const int8_t fontSize = 1, rgb_t colorh = WHITE, rgb_t bg = BLACK, rgb_t colorl = BLACK, const int8_t spacing = 0);
  void drawText(const int16_t x, const int16_t y, const char* text, const GFXfont** gfxFont, const int8_t fontSize = 1, rgb_t colorh = WHITE, rgb_t bg = BLACK, rgb_t colorl = BLACK, const int8_t spacing = 0);

  // unicode
  void drawChar(cursor_t& cursor, font_t& font, const uint16_t uchar, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);
  const uint16_t* drawTextLine(cursor_t& cursor, font_t& font, const uint16_t* utext, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);

  void drawText(const int16_t x, const int16_t y, const uint16_t* utext, const int8_t fontSize = 1, rgb_t colorh = WHITE, rgb_t bg = BLACK, rgb_t colorl = BLACK, const int8_t spacing = 0);
  void drawText(const int16_t x, const int16_t y, const uint16_t* utext, const GFXfont** gfxFont, const int8_t fontSize = 1, rgb_t colorh = WHITE, rgb_t bg = BLACK, rgb_t colorl = BLACK, const int8_t spacing = 0);

  using TSD_GFX::drawPixel;
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

private:
  int16_t WIDTH, HEIGHT;
protected:
  clip_t clip;      ///< Display width/height as modified by rotation
};
