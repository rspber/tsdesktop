/*
  Graphics library for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

This is an extract from Adafruit's core graphics library: Adafruit_GFX,
platform independent.

missed:
- missed print due to Arduino software dependency and lack of use in new solution,
- missed wrap,
- missed button, buttons and more gui objects are handled by TSDesktop,
- missed PROGMEM routines due to no need to use on modern hardware,
- missed handle _cp437 mistake for chars greather then 176,
- missed relationship to screen coordinates

changes:
- rotation and invertDisplay are maintain in TSD_ILI9341,
- no integrated cursor, now it is a parameter,
- no integrated gfxFont, textSizes, textColor, bakcgroundColor:
  they are parameters too and are handled by TSDesktop,
- drawChar was modified to the new solution,
- integrated fixed font in file glcdfont.c was reformatted and renamed to
  dafault_font,
- color type was changed from uint16_t to rgb_t to allow 666 colors
- overflow: clip was implemented in all functions,
- canvas was implemented in TSDesktop in GFXButton

notes:
- original gfxfont.h and Fonts directory from Adafruit's library are included,

Original Adafruit's license bellow
*/

/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!

Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */

#include "TSD_GFX.h"
#include <math.h>
#include "glcdfont.c.h"

const int16_t clip_t::width()
{
  return x2 > x1 ? x2 - x1 : 0;
}

const int16_t clip_t::height()
{
  return y2 > y1 ? y2 - y1 : 0;
}

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

/**************************************************************************/
/*!
   @brief    Write a line.  Bresenham's algorithm - thx wikpedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void TSD_GFX::writeLine(clip_t* clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, rgb_t color) {
#if defined(ESP8266)
  yield();
#endif
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  }
  else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      writePixel(clip, y0, x0, color);
    }
    else {
      writePixel(clip, x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

/**************************************************************************/
/*!
   @brief    Start a display-writing routine, overwrite in subclasses.
*/
/**************************************************************************/
void TSD_GFX::startWrite() {}

/**************************************************************************/
/*!
   @brief    Write a pixel, overwrite in subclasses if startWrite is defined!
    @param   x   x coordinate
    @param   y   y coordinate
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void TSD_GFX::writePixel(clip_t* clip, int16_t x, int16_t y, rgb_t color) {
  drawPixel(clip, x, y, color);
}

/**************************************************************************/
/*!
   @brief    Write a perfectly vertical line, overwrite in subclasses if
   startWrite is defined!
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void TSD_GFX::writeFastVLine(clip_t* clip, int16_t x, int16_t y, int16_t h, rgb_t color) {
  // Overwrite in subclasses if startWrite is defined!
  // Can be just writeLine(x, y, x, y+h-1, color);
  // or writeFillRect(x, y, 1, h, color);
  drawFastVLine(clip, x, y, h, color);
}

/**************************************************************************/
/*!
   @brief    Write a perfectly horizontal line, overwrite in subclasses if
   startWrite is defined!
    @param    x   Left-most x coordinate
    @param    y   Left-most y coordinate
    @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void TSD_GFX::writeFastHLine(clip_t* clip, int16_t x, int16_t y, int16_t w, rgb_t color) {
  // Overwrite in subclasses if startWrite is defined!
  // Example: writeLine(x, y, x+w-1, y, color);
  // or writeFillRect(x, y, w, 1, color);
  drawFastHLine(clip, x, y, w, color);
}

/**************************************************************************/
/*!
   @brief    Write a rectangle completely with one color, overwrite in
   subclasses if startWrite is defined!
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void TSD_GFX::writeFillRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color) {
  // Overwrite in subclasses if desired!
  fillRect(clip, x, y, w, h, color);
}

/**************************************************************************/
/*!
   @brief    End a display-writing routine, overwrite in subclasses if
   startWrite is defined!
*/
/**************************************************************************/
void TSD_GFX::endWrite() {}

/**************************************************************************/
/*!
   @brief    Draw a perfectly vertical line (this is often optimized in a
   subclass!)
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void TSD_GFX::drawFastVLine(clip_t* clip, int16_t x, int16_t y, int16_t h, rgb_t color) {
  startWrite();
  writeLine(clip, x, y, x, y + h - 1, color);
  endWrite();
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly horizontal line (this is often optimized in a
   subclass!)
    @param    x   Left-most x coordinate
    @param    y   Left-most y coordinate
    @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void TSD_GFX::drawFastHLine(clip_t* clip, int16_t x, int16_t y, int16_t w, rgb_t color) {
  startWrite();
  writeLine(clip, x, y, x + w - 1, y, color);
  endWrite();
}

/**************************************************************************/
/*!
   @brief    Fill a rectangle completely with one color. Update in subclasses if
   desired!
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void TSD_GFX::fillRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color) {
  startWrite();
  for (int16_t i = x; i < x + w; i++) {
    writeFastVLine(clip, i, y, h, color);
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief    Draw a line
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void TSD_GFX::drawLine(clip_t* clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, rgb_t color) {
  // Update in subclasses if desired!
  if (x0 == x1) {
    if (y0 > y1)
      _swap_int16_t(y0, y1);
    drawFastVLine(clip, x0, y0, y1 - y0 + 1, color);
  }
  else if (y0 == y1) {
    if (x0 > x1)
      _swap_int16_t(x0, x1);
    drawFastHLine(clip, x0, y0, x1 - x0 + 1, color);
  }
  else {
    startWrite();
    writeLine(clip, x0, y0, x1, y1, color);
    endWrite();
  }
}

/**************************************************************************/
/*!
   @brief    Draw a circle outline
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void TSD_GFX::drawCircle(clip_t* clip, int16_t x0, int16_t y0, int16_t r, rgb_t color) {
#if defined(ESP8266)
  yield();
#endif
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  startWrite();
  writePixel(clip, x0, y0 + r, color);
  writePixel(clip, x0, y0 - r, color);
  writePixel(clip, x0 + r, y0, color);
  writePixel(clip, x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    writePixel(clip, x0 + x, y0 + y, color);
    writePixel(clip, x0 - x, y0 + y, color);
    writePixel(clip, x0 + x, y0 - y, color);
    writePixel(clip, x0 - x, y0 - y, color);
    writePixel(clip, x0 + y, y0 + x, color);
    writePixel(clip, x0 - y, y0 + x, color);
    writePixel(clip, x0 + y, y0 - x, color);
    writePixel(clip, x0 - y, y0 - x, color);
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief    Quarter-circle drawer, used to do circles and roundrects
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of
   the circle we're doing
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void TSD_GFX::drawCircleHelper(clip_t* clip, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, rgb_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4) {
      writePixel(clip, x0 + x, y0 + y, color);
      writePixel(clip, x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      writePixel(clip, x0 + x, y0 - y, color);
      writePixel(clip, x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      writePixel(clip, x0 - y, y0 + x, color);
      writePixel(clip, x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      writePixel(clip, x0 - y, y0 - x, color);
      writePixel(clip, x0 - x, y0 - y, color);
    }
  }
}

/**************************************************************************/
/*!
   @brief    Draw a circle with filled color
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void TSD_GFX::fillCircle(clip_t* clip, int16_t x0, int16_t y0, int16_t r, rgb_t color) {
  startWrite();
  writeFastVLine(clip, x0, y0 - r, 2 * r + 1, color);
  fillCircleHelper(clip, x0, y0, r, 3, 0, color);
  endWrite();
}

/**************************************************************************/
/*!
   @brief  Quarter-circle drawer with fill, used for circles and roundrects
    @param  x0       Center-point x coordinate
    @param  y0       Center-point y coordinate
    @param  r        Radius of circle
    @param  corners  Mask bits indicating which quarters we're doing
    @param  delta    Offset from center-point, used for round-rects
    @param  color    16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void TSD_GFX::fillCircleHelper(clip_t* clip, int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, rgb_t color) {

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  delta++; // Avoid some +1's in the loop

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // These checks avoid double-drawing certain lines, important
    // for the SSD1306 library which has an INVERT drawing mode.
    if (x < (y + 1)) {
      if (corners & 1)
        writeFastVLine(clip, x0 + x, y0 - y, 2 * y + delta, color);
      if (corners & 2)
        writeFastVLine(clip, x0 - x, y0 - y, 2 * y + delta, color);
    }
    if (y != py) {
      if (corners & 1)
        writeFastVLine(clip, x0 + py, y0 - px, 2 * px + delta, color);
      if (corners & 2)
        writeFastVLine(clip, x0 - py, y0 - px, 2 * px + delta, color);
      py = y;
    }
    px = x;
  }
}

/**************************************************************************/
/*!
   @brief   Draw a rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void TSD_GFX::drawRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color) {
  startWrite();
  writeFastHLine(clip, x, y, w, color);
  writeFastHLine(clip, x, y + h - 1, w, color);
  writeFastVLine(clip, x, y, h, color);
  writeFastVLine(clip, x + w - 1, y, h, color);
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a rounded rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    r   Radius of corner rounding
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void TSD_GFX::drawRoundRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, rgb_t color) {
  int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
  if (r > max_radius)
    r = max_radius;
  // smarter version
  startWrite();
  writeFastHLine(clip, x + r, y, w - 2 * r, color);         // Top
  writeFastHLine(clip, x + r, y + h - 1, w - 2 * r, color); // Bottom
  writeFastVLine(clip, x, y + r, h - 2 * r, color);         // Left
  writeFastVLine(clip, x + w - 1, y + r, h - 2 * r, color); // Right
  // draw four corners
  drawCircleHelper(clip, x + r, y + r, r, 1, color);
  drawCircleHelper(clip, x + w - r - 1, y + r, r, 2, color);
  drawCircleHelper(clip, x + w - r - 1, y + h - r - 1, r, 4, color);
  drawCircleHelper(clip, x + r, y + h - r - 1, r, 8, color);
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a rounded rectangle with fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    r   Radius of corner rounding
    @param    color 16-bit 5-6-5 Color to draw/fill with
*/
/**************************************************************************/
void TSD_GFX::fillRoundRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, rgb_t color) {
  int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
  if (r > max_radius)
    r = max_radius;
  // smarter version
  startWrite();
  writeFillRect(clip, x + r, y, w - 2 * r, h, color);
  // draw four corners
  fillCircleHelper(clip, x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
  fillCircleHelper(clip, x + r, y + r, r, 2, h - 2 * r - 1, color);
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a triangle with no fill color
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void TSD_GFX::drawTriangle(clip_t* clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color) {
  drawLine(clip, x0, y0, x1, y1, color);
  drawLine(clip, x1, y1, x2, y2, color);
  drawLine(clip, x2, y2, x0, y0, color);
}

/**************************************************************************/
/*!
   @brief     Draw a triangle with color-fill
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to fill/draw with
*/
/**************************************************************************/
void TSD_GFX::fillTriangle(clip_t* clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    _swap_int16_t(y2, y1);
    _swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }

  startWrite();
  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    writeFastHLine(clip, a, y0, b - a + 1, color);
    endWrite();
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
    dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      _swap_int16_t(a, b);
    writeFastHLine(clip, a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      _swap_int16_t(a, b);
    writeFastHLine(clip, a, y, b - a + 1, color);
  }
  endWrite();
}

// BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------

/**************************************************************************/
/*!
   @brief      Draw a RAM-resident 1-bit image at the specified (x,y) position,
   using the specified foreground color (unset bits are transparent).
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void TSD_GFX::drawBitmap(clip_t* clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  startWrite();
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = bitmap[j * byteWidth + i / 8];
      if (b & 0x80)
        writePixel(clip, x + i, y, color);
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief      Draw a RAM-resident 1-bit image at the specified (x,y) position,
   using the specified foreground (for set bits) and background (unset bits)
   colors.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
    @param    color 16-bit 5-6-5 Color to draw pixels with
    @param    bg 16-bit 5-6-5 Color to draw background with
*/
/**************************************************************************/
void TSD_GFX::drawBitmap(clip_t* clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color, rgb_t bg) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  startWrite();
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = bitmap[j * byteWidth + i / 8];
      writePixel(clip, x + i, y, (b & 0x80) ? color : bg);
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a RAM-resident 8-bit image (grayscale) at the specified (x,y)
   pos. Specifically for 8-bit display devices such as IS31FL3731; no color
   reduction/expansion is performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with grayscale bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void TSD_GFX::drawGrayscaleBitmap(clip_t* clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h) {
  startWrite();
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      writePixel(clip, x + i, y, bitmap[j * w + i]);
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a RAM-resident 8-bit image (grayscale) with a 1-bit mask
   (set bits = opaque, unset bits = clear) at the specified (x,y) position.
   BOTH buffers (grayscale and mask) must be RAM-residentt, no mix-and-match
   Specifically for 8-bit display devices such as IS31FL3731; no color
   reduction/expansion is performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with grayscale bitmap
    @param    mask  byte array with mask bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void TSD_GFX::drawGrayscaleBitmap(clip_t* clip, int16_t x, int16_t y, const uint8_t* bitmap, const uint8_t* mask, int16_t w, int16_t h) {
  int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t b = 0;
  startWrite();
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = mask[j * bw + i / 8];
      if (b & 0x80) {
        writePixel(clip, x + i, y, bitmap[j * w + i]);
      }
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a RAM-resident 16-bit image (RGB 5/6/5) at the specified (x,y)
   position. For 16-bit display devices; no color reduction performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with 16-bit color bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void TSD_GFX::drawRGBBitmap(clip_t* clip, int16_t x, int16_t y, const rgb_t* bitmap, int16_t w, int16_t h) {
  startWrite();
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      writePixel(clip, x + i, y, bitmap[j * w + i]);
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a RAM-resident 16-bit image (RGB 5/6/5) with a 1-bit mask (set
   bits = opaque, unset bits = clear) at the specified (x,y) position. BOTH
   buffers (color and mask) must be RAM-resident. For 16-bit display devices; no
   color reduction performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with 16-bit color bitmap
    @param    mask  byte array with monochrome mask bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void TSD_GFX::drawRGBBitmap(clip_t* clip, int16_t x, int16_t y, const rgb_t* bitmap, const uint8_t* mask, int16_t w, int16_t h) {
  int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t b = 0;
  startWrite();
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = mask[j * bw + i / 8];
      if (b & 0x80) {
        writePixel(clip, x + i, y, bitmap[j * w + i]);
      }
    }
  }
  endWrite();
}

// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    cursor top left corner x, y coordinate
    @param    font  gfxFont with fontSizeX and fontSizeY
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
*/
/**************************************************************************/
void TSD_GFX::drawChar(clip_t* clip, cursor_t* cursor, font_t* font, const char c, const rgb_t color)
{
  GFXfont* gfxFont = font->gfxFont;
  uint8_t size_x = font->fontSizeX;
  uint8_t size_y = font->fontSizeY;

  int16_t x = cursor->x;
  int16_t y = cursor->y;
  font->cursorAdjust(&x, &y);

  int8_t x_adw;
  if (!gfxFont) { // 'Classic' built-in font
    startWrite();
    for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
      uint8_t line = default_font[c * 5 + i];
      for (int8_t j = 0; j < 8; j++, line >>= 1) {
        if (line & 1) {
          if (size_x == 1 && size_y == 1)
            writePixel(clip, x + i, y + j, color);
          else
            writeFillRect(clip, x + i * size_x, y + j * size_y, size_x, size_y,
              color);
        }
      }
    }
    endWrite();
    x_adw = 6;

  }
  else { // Custom font

    GFXglyph* glyph = &gfxFont->glyph[c - gfxFont->first];
    uint8_t* bitmap = gfxFont->bitmap;

    uint16_t bo = glyph->bitmapOffset;
    uint8_t w = glyph->width, h = glyph->height;
    int8_t xo = glyph->xOffset,
      yo = glyph->yOffset;
    uint8_t xx, yy, bits = 0, bit = 0;
    int16_t xo16 = 0, yo16 = 0;

    if (size_x > 1 || size_y > 1) {
      xo16 = (xo + 1) >> 1;
      yo16 = yo;
    }

    startWrite();
    for (yy = 0; yy < h; yy++) {
      for (xx = 0; xx < w; xx++) {
        if (!(bit++ & 7)) {
          bits = bitmap[bo++];
        }
        if (bits & 0x80) {
          if (size_x == 1 && size_y == 1) {
            writePixel(clip, x + xo + xx, y + yo + yy, color);
          }
          else {
            writeFillRect(clip, x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y,
              size_x, size_y, color);
          }
        }
        bits <<= 1;
      }
    }
    endWrite();
    x_adw = glyph->xAdvance;

  } // End classic vs custom font
  cursor->x += x_adw * size_x;
}

// Draw text line
/**************************************************************************/
/*!
   @brief   Draw text line to the \r or \n character or to the end of the text
    @param    cursor top left corner x, y coordinate
    @param    font  gfxFont with fontSizeX and fontSizeY
    @param    text   text line to be drawn
    @param    color 16-bit 5-6-5 Color to draw chraracter with
  @param    spacing extra horizontal spacing for letters
*/
/**************************************************************************/
void TSD_GFX::drawTextLine(clip_t* clip, cursor_t* cursor, font_t* font, const char* text, const rgb_t color, const int8_t spacing)
{
  const char* p = text;
  char c;
  while ((c = *p++) && c != '\r' && c != '\n') {
    drawChar(clip, cursor, font, c, color);
    cursor->x += spacing * font->fontSizeX;
  }
}
