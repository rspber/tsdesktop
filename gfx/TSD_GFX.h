/*
  Graphics library for TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

  More information in TSD_GFX.cpp

*/

#pragma once

#include <t_clip.h>
#include <t_gradient.h>
#include <stddef.h>
#include "rgb.h"

class TSD_GFX {
public:
  void drawPixel(clip_t& clip, int32_t x, int32_t y, rgb_t color);
  void drawFastVLine(clip_t& clip, int32_t x, int32_t y, int32_t h, rgb_t color);
  void drawFastHLine(clip_t& clip, int32_t x, int32_t y, int32_t w, rgb_t color);
  void drawLine(clip_t& clip, int32_t x0, int32_t y0, int32_t x1, int32_t y1, rgb_t color);
  void drawRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color);
  void fillRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color);
  void fillRectGradient(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, gradient_t& z);
  void drawRoundRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color);
  void fillRoundRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color);
  void drawCircle(clip_t& clip, int32_t x, int32_t y, int32_t r, rgb_t color);
  void drawCircleFragment(clip_t& clip, int32_t x, int32_t y, int32_t r, uint8_t corners, rgb_t color);
  void fillCircle(clip_t& clip, int32_t x, int32_t y, int32_t r, rgb_t color);
  void fillCircleFragment(clip_t& clip, int32_t x, int32_t y, int32_t r, uint8_t corners, int32_t delta, rgb_t color);
  void drawEllipse(clip_t& clip, int32_t x0, int32_t y0, int32_t rx, int32_t ry, rgb_t color);
  void fillEllipse(clip_t& clip, int32_t x0, int32_t y0, int32_t rx, int32_t ry, rgb_t color);
  void drawTriangle(clip_t& clip, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, rgb_t color);
  void fillTriangle(clip_t& clip, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, rgb_t color);

  // Smooth (anti-aliased) graphics drawing
           // Draw a pixel blended with the background pixel colour (bg_color) specified,  return blended colour
           // If the bg_color is not specified, the background pixel colour will be read from TFT or sprite
  uint16_t drawPixel(clip_t& clip, int32_t x, int32_t y, rgb_t color, uint8_t alpha, rgb_t bg_color = WHITE);

  virtual rgb_t readPixel(clip_t& clip, int32_t x, int32_t y) = 0;

           // Draw an anti-aliased (smooth) arc between start and end angles. Arc ends are anti-aliased.
           // By default the arc is drawn with square ends unless the "roundEnds" parameter is included and set true
           // Angle = 0 is at 6 o'clock position, 90 at 9 o'clock etc. The angles must be in range 0-360 or they will be clipped to these limits
           // The start angle may be larger than the end angle. Arcs are always drawn clockwise from the start angle.
  void     drawSmoothArc(clip_t& clip, int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, rgb_t fg_color, rgb_t bg_color, bool roundEnds = false);
           // As per "drawSmoothArc" except the ends of the arc are NOT anti-aliased, this facilitates dynamic arc length changes with
           // arc segments and ensures clean segment joints.
           // The sides of the arc are anti-aliased by default. If smoothArc is false sides will NOT be anti-aliased
  void     drawArc(clip_t& clip, int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, rgb_t fg_color, rgb_t bg_color, bool smoothArc = true);

           // Draw an anti-aliased filled circle at x, y with radius r
           // Note: The thickness of line is 3 pixels to reduce the visible "braiding" effect of anti-aliasing narrow lines
           //       this means the inner anti-alias zone is always at r-1 and the outer zone at r+1
  void     drawSmoothCircle(clip_t& clip, int32_t x, int32_t y, int32_t r, rgb_t fg_color, rgb_t bg_color);

           // Draw an anti-aliased filled circle at x, y with radius r
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     fillSmoothCircle(clip_t& clip, int32_t x, int32_t y, int32_t r, rgb_t color, rgb_t bg_color = WHITE);

           // Draw a rounded rectangle that has a line thickness of r-ir+1 and bounding box defined by x,y and w,h
           // The outer corner radius is r, inner corner radius is ir
           // The inside and outside of the border are anti-aliased
  void     drawSmoothRoundRect(clip_t& clip, int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, rgb_t fg_color, rgb_t bg_color = WHITE, uint8_t quadrants = 0xF);

           // Draw a filled rounded rectangle , corner radius r and bounding box defined by x,y and w,h
  void     fillSmoothRoundRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color, rgb_t bg_color = WHITE);

           // Draw a small anti-aliased filled circle at ax,ay with radius r (uses drawWideLine)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawSpot(clip_t& clip, float ax, float ay, float r, rgb_t fg_color, rgb_t bg_color = WHITE);

           // Draw an anti-aliased wide line from ax,ay to bx,by width wd with radiused ends (radius is wd/2)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWideLine(clip_t& clip, float ax, float ay, float bx, float by, float wd, rgb_t fg_color, rgb_t bg_color = WHITE);

           // Draw an anti-aliased wide line from ax,ay to bx,by with different width at each end aw, bw and with radiused ends
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWedgeLine(clip_t& clip, float ax, float ay, float bx, float by, float aw, float bw, rgb_t fg_color, rgb_t bg_color = WHITE);

 private:
           // Smooth graphics helper
  uint8_t  sqrt_fraction(uint32_t num);

           // Helper function: calculate distance of a point from a finite length line between two points
  float    wedgeLineDistance(float pax, float pay, float bax, float bay, float dr);

 public:
  void drawBitmap(clip_t& clip, int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h, rgb_t color);
  void drawBitmap(clip_t& clip, int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h, rgb_t color, rgb_t bg);
  void drawGrayscaleBitmap(clip_t& clip, int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h);
  void drawGrayscaleBitmap(clip_t& clip, int32_t x, int32_t y, const uint8_t* bitmap, const uint8_t* mask, int32_t w, int32_t h);
  void drawRGBBitmap(clip_t& clip, int32_t x, int32_t y, const uint16_t* bitmap, int32_t w, int32_t h);   // 565 color
  void drawRGBBitmap(clip_t& clip, int32_t x, int32_t y, const uint16_t* bitmap, const uint8_t *mask, int32_t w, int32_t h); // 565 color
  void drawRGBBitmap(clip_t& clip, int32_t x, int32_t y, const uint32_t* bitmap, int32_t w, int32_t h);   // 666 color
  void drawRGBBitmap(clip_t& clip, int32_t x, int32_t y, const uint32_t* bitmap, const uint8_t *mask, int32_t w, int32_t h); // 666 color

           // These are used to render images or sprites stored in RAM arrays (used by Sprite class for 16bpp Sprites)
  void     pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
  void     pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, rgb_t transparent);

           // These are used to render images stored in FLASH (PROGMEM)
//  void     pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, rgb_t transparent);
//  void     pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data);

           // These are used by Sprite class pushSprite() member function for 1, 4 and 8 bits per pixel (bpp) colours
           // They are not intended to be used with user sketches (but could be)
           // Set bpp8 true for 8bpp sprites, false otherwise. The cmap pointer must be specified for 4bpp
  void     pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, bool bpp8 = true, uint16_t *cmap = nullptr);
  void     pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, uint8_t  transparent, bool bpp8 = true, uint16_t *cmap = nullptr);
           // FLASH version
//  void     pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8,  uint16_t *cmap = nullptr);

           // Render a 16-bit colour image with a 1bpp mask
  void     pushMaskedImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *img, uint8_t *mask);

  void fillRectHelper(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color);
  void drawCircleHelper(clip_t& clip, int32_t x, int32_t y, int32_t r, uint8_t corners, rgb_t color);
  void fillCircleHelper(clip_t& clip, int32_t x, int32_t y, int32_t r, uint8_t corners, int32_t delta, rgb_t color);

  virtual void startWrite() = 0;
  virtual void endWrite() = 0;
  virtual void writeAddrWindow(const int32_t x, const int32_t y, const int32_t w, const int32_t h) = 0;
  virtual void writeMDTBuffer(const uint8_t* buffer, const int32_t len) = 0;
  virtual void sendMDTColor1(const mdt_t c) = 0;
  virtual void sendMDTColor(const mdt_t c, const int32_t len);
  virtual void drawClippedPixel(const int32_t x, const int32_t y, const rgb_t color);
  virtual void drawClippedPixelRec(const int32_t x, const int32_t y, const int32_t w, const int32_t h, const rgb_t color);

  uint32_t _bitmap_fg, _bitmap_bg;           // Bitmap foreground (bit=1) and background (bit=0) colours

  bool _vpOoB;

};


// Swap any type
template <typename T> static inline void
transpose(T& a, T& b) { T t = a; a = b; b = t; }


/***************************************************************************************
** Function name:           color24to16
** Description:             convert 24-bit colour to a 16-bit 565 colour value
***************************************************************************************/
inline uint16_t color24to16(rgb_t color888)
{
  uint16_t r = (color888 >> 8) & 0xF800;
  uint16_t g = (color888 >> 5) & 0x07E0;
  uint16_t b = (color888 >> 3) & 0x001F;

  return (r | g | b);
}


           // Alpha blend 2 colours, see generic "alphaBlend_Test" example
           // alpha =   0 = 100% background colour
           // alpha = 255 = 100% foreground colour
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

           // 16-bit colour alphaBlend with alpha dither (dither reduces colour banding)
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);
           // 24-bit colour alphaBlend with optional alpha dither
  uint32_t alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither = 0);
