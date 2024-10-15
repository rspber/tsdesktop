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
  TSD_SCREEN(const int16_t w, const int16_t h): TSD_GFX()
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

public:
           // Alpha blend 2 colours, see generic "alphaBlend_Test" example
           // alpha =   0 = 100% background colour
           // alpha = 255 = 100% foreground colour
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

           // 16-bit colour alphaBlend with alpha dither (dither reduces colour banding)
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);
           // 24-bit colour alphaBlend with optional alpha dither
  uint32_t alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither = 0);

  // Smooth (anti-aliased) graphics drawing
           // Draw a pixel blended with the background pixel colour (bg_color) specified,  return blended colour
           // If the bg_color is not specified, the background pixel colour will be read from TFT or sprite
  uint16_t drawPixel(clip_t& clip, int32_t x, int32_t y, uint32_t color, uint8_t alpha, uint32_t bg_color = 0x00FFFFFF);

  using TSD_GFX::drawPixel;

  virtual rgb_t readPixel(clip_t& clip, int16_t x, int16_t y) = 0;

           // Draw an anti-aliased (smooth) arc between start and end angles. Arc ends are anti-aliased.
           // By default the arc is drawn with square ends unless the "roundEnds" parameter is included and set true
           // Angle = 0 is at 6 o'clock position, 90 at 9 o'clock etc. The angles must be in range 0-360 or they will be clipped to these limits
           // The start angle may be larger than the end angle. Arcs are always drawn clockwise from the start angle.
  void     drawSmoothArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool roundEnds = false);
           // As per "drawSmoothArc" except the ends of the arc are NOT anti-aliased, this facilitates dynamic arc length changes with
           // arc segments and ensures clean segment joints.
           // The sides of the arc are anti-aliased by default. If smoothArc is false sides will NOT be anti-aliased
  void     drawArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool smoothArc = true);

           // Draw an anti-aliased filled circle at x, y with radius r
           // Note: The thickness of line is 3 pixels to reduce the visible "braiding" effect of anti-aliasing narrow lines
           //       this means the inner anti-alias zone is always at r-1 and the outer zone at r+1
  void     drawSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t fg_color, uint32_t bg_color);

           // Draw an anti-aliased filled circle at x, y with radius r
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     fillSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t color, uint32_t bg_color = 0x00FFFFFF);

           // Draw a rounded rectangle that has a line thickness of r-ir+1 and bounding box defined by x,y and w,h
           // The outer corner radius is r, inner corner radius is ir
           // The inside and outside of the border are anti-aliased
  void     drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF, uint8_t quadrants = 0xF);

           // Draw a filled rounded rectangle , corner radius r and bounding box defined by x,y and w,h
  void     fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color, uint32_t bg_color = 0x00FFFFFF);

           // Draw a small anti-aliased filled circle at ax,ay with radius r (uses drawWideLine)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawSpot(float ax, float ay, float r, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);

           // Draw an anti-aliased wide line from ax,ay to bx,by width wd with radiused ends (radius is wd/2)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWideLine(float ax, float ay, float bx, float by, float wd, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);

           // Draw an anti-aliased wide line from ax,ay to bx,by with different width at each end aw, bw and with radiused ends
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWedgeLine(float ax, float ay, float bx, float by, float aw, float bw, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);

 private:
           // Smooth graphics helper
  uint8_t  sqrt_fraction(uint32_t num);

           // Helper function: calculate distance of a point from a finite length line between two points
  float    wedgeLineDistance(float pax, float pay, float bax, float bay, float dr);


public:
  void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color);
  void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color, rgb_t bg);
  void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h);
  void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t* bitmap, const uint8_t* mask, int16_t w, int16_t h);
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t* bitmap, int16_t w, int16_t h); // 565 color
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t* bitmap, const uint8_t *mask, int16_t w, int16_t h); // 565 color
  void drawRGBBitmap(int16_t x, int16_t y, const uint32_t* bitmap, int16_t w, int16_t h); // 666 color
  void drawRGBBitmap(int16_t x, int16_t y, const uint32_t* bitmap, const uint8_t *mask, int16_t w, int16_t h); // 666 color

public:
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

  const clip_t& getClip() { return clip; }

private:
  int16_t WIDTH, HEIGHT;
protected:
  clip_t clip;      ///< Display width/height as modified by rotation
};



// Swap any type
template <typename T> static inline void
transpose(T& a, T& b) { T t = a; a = b; b = t; }




// Fast alphaBlend
template <typename A, typename F, typename B> static inline uint16_t
fastBlend(A alpha, F fgc, B bgc)
{
  // Split out and blend 5-bit red and blue channels
  uint32_t rxb = bgc & 0xF81F;
  rxb += ((fgc & 0xF81F) - rxb) * (alpha >> 2) >> 6;
  // Split out and blend 6-bit green channel
  uint32_t xgx = bgc & 0x07E0;
  xgx += ((fgc & 0x07E0) - xgx) * alpha >> 8;
  // Recombine channels
  return (rxb & 0xF81F) | (xgx & 0x07E0);
}
