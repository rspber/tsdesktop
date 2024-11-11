/*
  Graphics library for TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

  More information in TSD_GFX.cpp

*/

#pragma once

#include <t_clip.h>
#include <t_cursor.h>
#include <t_gradient.h>
#include "tsd_font.h"
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

  // utf-8
  void drawChar(clip_t& clip, cursor_t& cursor, tsd_font_t& font, char** c, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);
  const char* drawTextLine(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const char* text, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);

  // unicode
  void drawChar(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const uint16_t uchar, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);
  const uint16_t* drawTextLine(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const uint16_t* utext, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);

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
