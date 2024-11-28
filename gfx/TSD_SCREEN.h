/*
  TSD_SCREEN for TSDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

  Smooth graphics comes from Bodmer's TFT_eSPI

*/

#pragma once

#include <TSD_CHAR.h>

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

class TSD_SCREEN : public TSD_CHAR {
public:
  TSD_SCREEN(const int32_t w, const int32_t h): TSD_CHAR()
  {
    _init_width = w;
    _init_height = h;
    _clip = {0, 0, w, h};
  }

  int32_t width() { return _clip.width(); }
  int32_t height() { return _clip.height(); }

  int32_t getWIDTH() { return _init_width; }
  int32_t getHEIGHT() { return _init_height; }

  void setSize(const int32_t w, const int32_t h)
  {
    _clip.x2 = _clip.x1 + w;
    _clip.y2 = _clip.y1 + h;
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

           // Draw an anti-aliased (smooth) arc between start and end angles. Arc ends are anti-aliased.
           // By default the arc is drawn with square ends unless the "roundEnds" parameter is included and set true
           // Angle = 0 is at 6 o'clock position, 90 at 9 o'clock etc. The angles must be in range 0-360 or they will be clipped to these limits
           // The start angle may be larger than the end angle. Arcs are always drawn clockwise from the start angle.
  void     drawSmoothArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, rgb_t fg_color, rgb_t bg_color, bool roundEnds = false);
           // As per "drawSmoothArc" except the ends of the arc are NOT anti-aliased, this facilitates dynamic arc length changes with
           // arc segments and ensures clean segment joints.
           // The sides of the arc are anti-aliased by default. If smoothArc is false sides will NOT be anti-aliased
  void     drawArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, rgb_t fg_color, rgb_t bg_color, bool smoothArc = true);

           // Draw an anti-aliased filled circle at x, y with radius r
           // Note: The thickness of line is 3 pixels to reduce the visible "braiding" effect of anti-aliasing narrow lines
           //       this means the inner anti-alias zone is always at r-1 and the outer zone at r+1
  void     drawSmoothCircle(int32_t x, int32_t y, int32_t r, rgb_t fg_color, rgb_t bg_color);

           // Draw an anti-aliased filled circle at x, y with radius r
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     fillSmoothCircle(int32_t x, int32_t y, int32_t r, rgb_t color, rgb_t bg_color = WHITE);

           // Draw a rounded rectangle that has a line thickness of r-ir+1 and bounding box defined by x,y and w,h
           // The outer corner radius is r, inner corner radius is ir
           // The inside and outside of the border are anti-aliased
  void     drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, rgb_t fg_color, rgb_t bg_color = WHITE, uint8_t quadrants = 0xF);

           // Draw a filled rounded rectangle , corner radius r and bounding box defined by x,y and w,h
  void     fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color, rgb_t bg_color = WHITE);

           // Draw a small anti-aliased filled circle at ax,ay with radius r (uses drawWideLine)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawSpot(float ax, float ay, float r, rgb_t fg_color, rgb_t bg_color = WHITE);

           // Draw an anti-aliased wide line from ax,ay to bx,by width wd with radiused ends (radius is wd/2)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWideLine(float ax, float ay, float bx, float by, float wd, rgb_t fg_color, rgb_t bg_color = WHITE);

           // Draw an anti-aliased wide line from ax,ay to bx,by with different width at each end aw, bw and with radiused ends
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWedgeLine(float ax, float ay, float bx, float by, float aw, float bw, rgb_t fg_color, rgb_t bg_color = WHITE);


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
           // These are used to render images or sprites stored in RAM arrays (used by Sprite class for 16bpp Sprites)
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, rgb_t transparent);

           // These are used to render images stored in FLASH (PROGMEM)
//  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, rgb_t transparent);
//  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data);

           // These are used by Sprite class pushSprite() member function for 1, 4 and 8 bits per pixel (bpp) colours
           // They are not intended to be used with user sketches (but could be)
           // Set bpp8 true for 8bpp sprites, false otherwise. The cmap pointer must be specified for 4bpp
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, bool bpp8, cmap_t& cmap);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, rgb_t transp, bool bpp8, cmap_t& cmap);
           // FLASH version
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8, cmap_t& cmap);

           // Render a 16-bit colour image with a 1bpp mask
  void     pushMaskedImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *img, uint8_t *mask);

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

public:
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

  using TSD_GFX::drawArc;
  using TSD_GFX::drawSmoothArc;
  using TSD_GFX::fillSmoothCircle;
  using TSD_GFX::drawSmoothRoundRect;
  using TSD_GFX::fillSmoothRoundRect;
  using TSD_GFX::drawWedgeLine;

  using TSD_GFX::drawBitmap;
  using TSD_GFX::drawGrayscaleBitmap;
  using TSD_GFX::drawRGBBitmap;

  using TSD_CHAR::drawChar;
  using TSD_CHAR::drawTextLine;

  using TSD_GFX::pushImage;
  using TSD_GFX::pushMaskedImage;


  void setPivot(const int16_t x, const int16_t y) {
    _xPivot = x;
    _yPivot = y;
  }

  const int16_t getPivotX() { return _xPivot; }
  const int16_t getPivotY() { return _yPivot; }

public:
  int16_t _xPivot, _yPivot;

private:
  int16_t _init_width,
          _init_height;

public:
  clip_t _clip;      ///< Display width/height as modified by rotation
};
