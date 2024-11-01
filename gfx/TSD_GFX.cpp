/*
  Graphics library for TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

  Primary code comes from Adafruit's core graphics library: Adafruit_GFX,
  but was significantly modified or replaced by those from TFT_eSP library.

  Originally Adafruit's license in file /licenses/Adafruit_gfx_license.txt

  Originally Bodmers's license in file /licenses/Bodmer_license.txt

*/

#include "TSD_GFX.h"
#include "glcdfont.c.h"
#include "overlaid.h"
#include <stdlib.h>

const int16_t clip_t::width()
{
  return x2 > x1 ? x2 - x1 : 0;
}

const int16_t clip_t::height()
{
  return y2 > y1 ? y2 - y1 : 0;
}

#ifndef abs
#define abs(x) ((x)>=0?(x):-(x))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define SWAP_INT(a, b) \
  {            \
    int16_t t = a; \
    a = b;     \
    b = t;     \
  }

#define CLIP_X \
  if (x < clip.x1) { w -= clip.x1 - x; x = clip.x1; } \
  if (x + w > clip.x2) { w = clip.x2 - x; }

#define CLIP_Y \
  if (y < clip.y1) { h -= clip.y1 - y; y = clip.y1; } \
  if (y + h > clip.y2) { h = clip.y2 - y; }

#define IF_CLIP_X x >= clip.x1 && x < clip.x2

#define IF_CLIP_Y y >= clip.y1 && y < clip.y2

// Clipping macro for pushImage
#define PI_CLIP                                        \
  if (x >= clip.x2 || y >= clip.y2) return;            \
                                                       \
  int32_t dx = 0;                                      \
  int32_t dy = 0;                                      \
  int32_t dw = w;                                      \
  int32_t dh = h;                                      \
                                                       \
  if (x < clip.x1) { dx = clip.x1 - x; dw -= dx; x = clip.x1; } \
  if (y < clip.y1) { dy = clip.y1 - y; dh -= dy; y = clip.y1; } \
                                                       \
  if ((x + dw) > clip.x2 ) dw = clip.x2 - x;                 \
  if ((y + dh) > clip.y2 ) dh = clip.y2 - y;                 \
                                                       \
  if (dw < 1 || dh < 1) return;


/**************************************************************************/

void TSD_GFX::drawPixel(clip_t& clip, int16_t x, int16_t y, rgb_t color)
{
  if (IF_CLIP_X && IF_CLIP_Y) {
    drawClippedPixel(x, y, color);
  }
}

void TSD_GFX::drawFastHLine(clip_t& clip, int16_t x, int16_t y, int16_t w, rgb_t color)
{
  CLIP_X
  if (IF_CLIP_Y && w > 0) {
    drawClippedPixelRec(x, y, w, 1, color);
  }
}

void TSD_GFX::drawFastVLine(clip_t& clip, int16_t x, int16_t y, int16_t h, rgb_t color)
{
  CLIP_Y
  if (IF_CLIP_Y && h > 0) {
    drawClippedPixelRec(x, y, 1, h, color);
  }
}

void TSD_GFX::drawRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color)
{
  startWrite();

  drawFastHLine(clip, x, y, w, color);
  drawFastHLine(clip, x, y + h - 1, w, color);
  // Avoid drawing corner pixels twice
  drawFastVLine(clip, x, y+1, h-2, color);
  drawFastVLine(clip, x + w - 1, y+1, h-2, color);

  endWrite();
}

void TSD_GFX::fillRectHelper(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color)
{
  CLIP_X
  CLIP_Y
  if (w > 0 && h > 0) {
    drawClippedPixelRec(x, y, w, h, color);
  }
}

void TSD_GFX::fillRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color)
{
  startWrite();
  fillRectHelper(clip, x, y, w, h, color);
  endWrite();
}

void TSD_GFX::drawRoundRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, rgb_t color)
{
  startWrite();

  // smarter version
  drawFastHLine(clip, x + r  , y    , w - r - r, color); // Top
  drawFastHLine(clip, x + r  , y + h - 1, w - r - r, color); // Bottom
  drawFastVLine(clip, x    , y + r  , h - r - r, color); // Left
  drawFastVLine(clip, x + w - 1, y + r  , h - r - r, color); // Right
  // draw four corners
  drawCircleHelper(clip, x + r    , y + r    , r, 1, color);
  drawCircleHelper(clip, x + w - r - 1, y + r    , r, 2, color);
  drawCircleHelper(clip, x + w - r - 1, y + h - r - 1, r, 4, color);
  drawCircleHelper(clip, x + r    , y + h - r - 1, r, 8, color);

  endWrite();
}

void TSD_GFX::fillRoundRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, rgb_t color)
{
  startWrite();

  // smarter version
  fillRectHelper(clip, x, y + r, w, h - r - r, color);

  // draw four corners
  fillCircleHelper(clip, x + r, y + h - r - 1, r, 1, w - r - r - 1, color);
  fillCircleHelper(clip, x + r    , y + r, r, 2, w - r - r - 1, color);

  endWrite();
}


// Bresenham's algorithm - thx wikipedia - speed enhanced by Bodmer to use
// an efficient FastH/V Line draw routine for line segments of 2 pixels or more
void TSD_GFX::drawLine(clip_t& clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, rgb_t color)
{
  startWrite();

  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    SWAP_INT(x0, y0);
    SWAP_INT(x1, y1);
  }

  if (x0 > x1) {
    SWAP_INT(x0, x1);
    SWAP_INT(y0, y1);
  }

  int16_t dx = x1 - x0;
  int16_t dy = abs(y1 - y0);

  int16_t err = dx >> 1;
  int16_t xs = x0;
  int16_t dlen = 0;
  int16_t ystep = y0 < y1 ? 1 : -1;

  // Split into steep and not steep for FastH/V separation
  if (steep) {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        if (dlen == 1)
          drawPixel(clip, y0, xs, color);
        else
          drawFastVLine(clip, y0, xs, dlen, color);
        dlen = 0;
        y0 += ystep; xs = x0 + 1;
        err += dx;
      }
    }
    if (dlen) drawFastVLine(clip, y0, xs, dlen, color);
  }
  else
  {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        if (dlen == 1)
          drawPixel(clip, xs, y0, color);
        else
          drawFastHLine(clip, xs, y0, dlen, color);
        dlen = 0;
        y0 += ystep; xs = x0 + 1;
        err += dx;
      }
    }
    if (dlen) drawFastHLine(clip, xs, y0, dlen, color);
  }
  endWrite();
}

void TSD_GFX::drawEllipse(clip_t& clip, int16_t x0, int16_t y0, int16_t rx, int16_t ry, rgb_t color)
{
  startWrite();

  int a2 = rx * rx;
  int b2 = ry * ry;
  int a2b2 = a2 * b2;
  int dx = 0;

  drawPixel(clip, x0 - rx, y0, color);
  drawPixel(clip, x0 + rx, y0, color);

  int px = rx;
  for (int x = rx, y = 1; y <= ry; y++) {
    int x1 = x - (dx - 1);  // try slopes of dx - 1 or more
    for ( ; x1 > 0; x1--)
      if (x1*x1*b2 + a2*y*y <= a2b2)
        break;
    dx = x - x1;
    x = x1;
    if (y < ry) {
      drawFastHLine(clip, x0 - px, y0 - y, px - x, color);
      drawFastHLine(clip, x0 + x, y0 - y, px - x, color);
      drawFastHLine(clip, x0 - px, y0 + y, px - x, color);
      drawFastHLine(clip, x0 + x, y0 + y, px - x, color);
    }
    else {
      drawFastHLine(clip, x0 - px, y0 - y, 2*(px-x), color);
      drawFastHLine(clip, x0 - px, y0 + y, 2*(px-x), color);
    }
    px = x;
  }
  endWrite();
}

void TSD_GFX::fillEllipse(clip_t& clip, int16_t x0, int16_t y0, int16_t rx, int16_t ry, rgb_t color)
{
  startWrite();

  int a2 = rx * rx;
  int b2 = ry * ry;
  int a2b2 = a2 * b2;
  int x = rx;
  int dx = 0;

  drawFastHLine(clip, x0 - x, y0, 2*x, color);

  for (int y = 1; y <= ry; y++) {
    int x1 = x - (dx - 1);  // try slopes of dx - 1 or more
    for ( ; x1 > 0; x1--)
      if (x1*x1*b2 + a2*y*y <= a2b2)
        break;
    dx = x - x1;
    x = x1;
    drawFastHLine(clip, x0 - x, y0 - y, 2*x, color);
    drawFastHLine(clip, x0 - x, y0 + y, 2*x, color);
  }
  endWrite();
}

// Optimised midpoint circle algorithm
void TSD_GFX::drawCircle(clip_t& clip, int16_t x0, int16_t y0, int16_t r, rgb_t color)
{
  if ( r <= 0 ) return;

    startWrite();

    int16_t f     = 1 - r;
    int16_t ddF_y = -2 * r;
    int16_t ddF_x = 1;
    int16_t xs    = -1;
    int16_t xe    = 0;
    int16_t len   = 0;

    bool first = true;
    do {
      while (f < 0) {
        ++xe;
        f += (ddF_x += 2);
      }
      f += (ddF_y += 2);

      if (xe-xs>1) {
        if (first) {
          len = 2*(xe - xs)-1;
          drawFastHLine(clip, x0 - xe, y0 + r, len, color);
          drawFastHLine(clip, x0 - xe, y0 - r, len, color);
          drawFastVLine(clip, x0 + r, y0 - xe, len, color);
          drawFastVLine(clip, x0 - r, y0 - xe, len, color);
          first = false;
        }
        else {
          len = xe - xs++;
          drawFastHLine(clip, x0 - xe, y0 + r, len, color);
          drawFastHLine(clip, x0 - xe, y0 - r, len, color);
          drawFastHLine(clip, x0 + xs, y0 - r, len, color);
          drawFastHLine(clip, x0 + xs, y0 + r, len, color);

          drawFastVLine(clip, x0 + r, y0 + xs, len, color);
          drawFastVLine(clip, x0 + r, y0 - xe, len, color);
          drawFastVLine(clip, x0 - r, y0 - xe, len, color);
          drawFastVLine(clip, x0 - r, y0 + xs, len, color);
        }
      }
      else {
        ++xs;
        drawPixel(clip, x0 - xe, y0 + r, color);
        drawPixel(clip, x0 - xe, y0 - r, color);
        drawPixel(clip, x0 + xs, y0 - r, color);
        drawPixel(clip, x0 + xs, y0 + r, color);

        drawPixel(clip, x0 + r, y0 + xs, color);
        drawPixel(clip, x0 + r, y0 - xe, color);
        drawPixel(clip, x0 - r, y0 - xe, color);
        drawPixel(clip, x0 - r, y0 + xs, color);
      }
      xs = xe;
    } while (xe < --r);

    endWrite();
}

void TSD_GFX::drawCircleHelper(clip_t& clip, int16_t x0, int16_t y0, int16_t rr, uint8_t corners, rgb_t color)
{
  if (rr <= 0) return;

  int16_t f     = 1 - rr;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * rr;
  int16_t xe    = 0;
  int16_t xs    = 0;
  int16_t len   = 0;

  while (xe < rr--)
  {
    while (f < 0) {
      ++xe;
      f += (ddF_x += 2);
    }
    f += (ddF_y += 2);

    if (xe-xs==1) {
      if (corners & 0x1) { // left top
        drawPixel(clip, x0 - xe, y0 - rr, color);
        drawPixel(clip, x0 - rr, y0 - xe, color);
      }
      if (corners & 0x2) { // right top
        drawPixel(clip, x0 + rr    , y0 - xe, color);
        drawPixel(clip, x0 + xs + 1, y0 - rr, color);
      }
      if (corners & 0x4) { // right bottom
        drawPixel(clip, x0 + xs + 1, y0 + rr    , color);
        drawPixel(clip, x0 + rr, y0 + xs + 1, color);
      }
      if (corners & 0x8) { // left bottom
        drawPixel(clip, x0 - rr, y0 + xs + 1, color);
        drawPixel(clip, x0 - xe, y0 + rr    , color);
      }
    }
    else {
      len = xe - xs++;
      if (corners & 0x1) { // left top
        drawFastHLine(clip, x0 - xe, y0 - rr, len, color);
        drawFastVLine(clip, x0 - rr, y0 - xe, len, color);
      }
      if (corners & 0x2) { // right top
        drawFastVLine(clip, x0 + rr, y0 - xe, len, color);
        drawFastHLine(clip, x0 + xs, y0 - rr, len, color);
      }
      if (corners & 0x4) { // right bottom
        drawFastHLine(clip, x0 + xs, y0 + rr, len, color);
        drawFastVLine(clip, x0 + rr, y0 + xs, len, color);
      }
      if (corners & 0x8) { // left bottom
        drawFastVLine(clip, x0 - rr, y0 + xs, len, color);
        drawFastHLine(clip, x0 - xe, y0 + rr, len, color);
      }
    }
    xs = xe;
  }
}

void TSD_GFX::drawCircleFragment(clip_t& clip, int16_t x0, int16_t y0, int16_t rr, uint8_t corners, rgb_t color)
{
  startWrite();
  drawCircleHelper(clip, x0, y0, rr, corners, color);
  endWrite();
}

// Improved algorithm avoids repetition of lines
void TSD_GFX::fillCircle(clip_t& clip, int16_t x0, int16_t y0, int16_t r, rgb_t color)
{
  startWrite();

  int16_t  x  = 0;
  int16_t  dx = 1;
  int16_t  dy = r+r;
  int16_t  p  = -(r>>1);

  drawFastHLine(clip, x0 - r, y0, dy+1, color);

  while(x<r){

    if(p>=0) {
      drawFastHLine(clip, x0 - x, y0 + r, dx, color);
      drawFastHLine(clip, x0 - x, y0 - r, dx, color);
      dy-=2;
      p-=dy;
      r--;
    }

    dx+=2;
    p+=dx;
    x++;

    drawFastHLine(clip, x0 - r, y0 + x, dy+1, color);
    drawFastHLine(clip, x0 - r, y0 - x, dy+1, color);

  }
  endWrite();
}

void TSD_GFX::fillCircleHelper(clip_t& clip, int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, rgb_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -r - r;
  int16_t y     = 0;

  delta++;

  while (y < r) {
    if (f >= 0) {
      if (corners & 0x1) drawFastHLine(clip, x0 - y, y0 + r, y + y + delta, color);
      if (corners & 0x2) drawFastHLine(clip, x0 - y, y0 - r, y + y + delta, color);
      r--;
      ddF_y += 2;
      f     += ddF_y;
    }

    y++;
    ddF_x += 2;
    f     += ddF_x;

    if (corners & 0x1) drawFastHLine(clip, x0 - r, y0 + y, r + r + delta, color);
    if (corners & 0x2) drawFastHLine(clip, x0 - r, y0 - y, r + r + delta, color);
  }
}

void TSD_GFX::fillCircleFragment(clip_t& clip, int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, rgb_t color)
{
  startWrite();
  fillCircleHelper(clip, x0, y0, r, corners, delta, color);
  endWrite();
}

void TSD_GFX::drawTriangle(clip_t& clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color)
{
  drawLine(clip, x0, y0, x1, y1, color);
  drawLine(clip, x1, y1, x2, y2, color);
  drawLine(clip, x2, y2, x0, y0, color);
}

// Fill a triangle - original Adafruit function works well and code footprint is small
void TSD_GFX::fillTriangle(clip_t& clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color)
{
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    SWAP_INT(y0, y1);
    SWAP_INT(x0, x1);
  }
  if (y1 > y2) {
    SWAP_INT(y2, y1);
    SWAP_INT(x2, x1);
  }
  if (y0 > y1) {
    SWAP_INT(y0, y1);
    SWAP_INT(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)      a = x1;
    else if (x1 > b) b = x1;
    if (x2 < a)      a = x2;
    else if (x2 > b) b = x2;

    startWrite();
    drawFastHLine(clip, a, y0, b - a + 1, color);
    endWrite();

    return;
  }

  startWrite();

  int16_t
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1,
  sa   = 0,
  sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2) last = y1;  // Include y1 scanline
  else         last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if (a > b) SWAP_INT(a, b);
    drawFastHLine(clip, a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for (; y <= y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if (a > b) SWAP_INT(a, b);
    drawFastHLine(clip, a, y, b - a + 1, color);
  }

  endWrite();
}

// BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------

void TSD_GFX::drawBitmap(clip_t& clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color)
{
  startWrite();

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = bitmap[j * byteWidth + i / 8];
      if (b & 0x80)
        drawPixel(clip, x + i, y, color);
    }
  }

  endWrite();
}

void TSD_GFX::drawBitmap(clip_t& clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color, rgb_t bg)
{
  startWrite();

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = bitmap[j * byteWidth + i / 8];
      drawPixel(clip, x + i, y, (b & 0x80) ? color : bg);
    }
  }

  endWrite();
}

void TSD_GFX::drawGrayscaleBitmap(clip_t& clip, int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h)
{
  startWrite();

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      drawPixel(clip, x + i, y, bitmap[j * w + i] ? WHITE : BLACK);
    }
  }
  endWrite();
}

void TSD_GFX::drawGrayscaleBitmap(clip_t& clip, int16_t x, int16_t y, const uint8_t* bitmap, const uint8_t* mask, int16_t w, int16_t h)
{
  startWrite();

  int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t b = 0;
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = mask[j * bw + i / 8];
      if (b & 0x80) {
        drawPixel(clip, x + i, y, bitmap[j * w + i] ? WHITE : BLACK);
      }
    }
  }
  endWrite();
}

void TSD_GFX::drawRGBBitmap(clip_t& clip, int16_t x, int16_t y, const uint16_t* bitmap, int16_t w, int16_t h)
{
  int dx, dy, dw, dh;

  if (x >= clip.x1) {dx = 0; dw = w; } else { dx = clip.x1 - x; dw = w - dx; x = clip.x1; }
  if (y >= clip.y1) {dy = 0; dh = h; } else { dy = clip.y1 - y; dh = h - dy; y = clip.y1; }

  if (x + dw > clip.x2) { dw = clip.x2 - x; }
  if (y + dh > clip.y2) { dh = clip.y2 - y; }

  if (dw < 1 || dh < 1) return;

  startWrite();

  writeAddrWindow(x, y, dw, dh);

  if (dx != 0) {
    const uint16_t* p = bitmap + dw * dy + dx;
    if (MDT_SIZE == 2) {
      for (int j = 0; j < dh; ++j) {
        writeMDTBuffer((const uint8_t*)p, dw);
        p += w;
      }
    }
    else {   // translate 565 to 666
      const uint16_t* p = bitmap + dw * dy + dx;
      const uint8_t* buff = (const uint8_t*)malloc(dw * MDT_SIZE + 3);
      uint8_t* q = (uint8_t*)buff;
      for (int j = 0; j < dh; ++j) {
        for (int i = 0; i < dw; ++i) {
          *(rgb_t*)q = rgb(*p++);
          q += 3;
        }
        writeMDTBuffer((const uint8_t*)buff, dw);
        p += w;
      }
      free((void*)buff);
    }
  }
  else {
    writeMDTBuffer((const uint8_t*)(bitmap + w * dy), w * dh);
  }

  endWrite();
}

void TSD_GFX::drawRGBBitmap(clip_t& clip, int16_t x, int16_t y, const uint16_t* bitmap, const uint8_t* mask, int16_t w, int16_t h)
{
  startWrite();

  int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t b = 0;
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = mask[j * bw + i / 8];
      if (b & 0x80) {
        drawPixel(clip, x + i, y, rgb(bitmap[j * w + i]));
      }
    }
  }
  endWrite();
}

void TSD_GFX::drawRGBBitmap(clip_t& clip, int16_t x, int16_t y, const uint32_t* bitmap, int16_t w, int16_t h)
{
  startWrite();

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      drawPixel(clip, x + i, y, bitmap[j * w + i] | 0xFF000000);
    }
  }
  endWrite();
}

void TSD_GFX::drawRGBBitmap(clip_t& clip, int16_t x, int16_t y, const uint32_t* bitmap, const uint8_t* mask, int16_t w, int16_t h)
{
  startWrite();

  int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t b = 0;
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = mask[j * bw + i / 8];
      if (b & 0x80) {
        drawPixel(clip, x + i, y, bitmap[j * w + i] | 0xFF000000);
      }
    }
  }
  endWrite();
}



void TSD_GFX::pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data)
{
  PI_CLIP;

  startWrite();
  writeAddrWindow(x, y, dw, dh);

  data += dx + dy * w;

  // Check if whole image can be pushed
  if (dw == w) {
    writeMDTBuffer((const uint8_t*)data, dw * dh);
  }
  else {
    // Push line segments to crop image
    while (dh--) {
      writeMDTBuffer((const uint8_t*)data, dw);
      data += w;
    }
  }

  endWrite();
}


void TSD_GFX::pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint16_t transp)
{
  PI_CLIP;

  startWrite();

  data += dx + dy * w;


  uint16_t  lineBuf[dw]; // Use buffer to minimise setWindow call count

  // The little endian transp color must be byte swapped if the image is big endian
//  if (!_swapBytes) transp = transp >> 8 | transp << 8;

  while (dh--)
  {
    int32_t len = dw;
    uint16_t* ptr = data;
    int32_t px = x, sx = x;
    bool move = true;
    uint16_t np = 0;

    while (len--)
    {
      if (transp != *ptr)
      {
        if (move) { move = false; sx = px; }
        lineBuf[np] = *ptr;
        np++;
      }
      else
      {
        move = true;
        if (np)
        {
          writeAddrWindow(sx, y, np, 1);
          writeMDTBuffer((const uint8_t*)lineBuf, np);
          np = 0;
        }
      }
      px++;
      ptr++;
    }
    if (np) {
      writeAddrWindow(sx, y, np, 1);
      writeMDTBuffer((const uint8_t*)lineBuf, np);
    }

    y++;
    data += w;
  }

  endWrite();
}














// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

bool ifdrawbgif(rgb_t color, rgb_t bg)
{
#ifdef OVERLAID
  // see TSD_ILI9341.cpp for explanation
  if ((color & 0xFF000000) != 0xFF000000) {
    over_t* t = (over_t*)color;
    return t->color != bg;
  }
  else
#endif
  {
    return color != bg;
  }
}

rgb_t alphaBlend(uint8_t alpha, rgb_t ch, rgb_t cl)
{
  // For speed use fixed point maths and rounding to permit a power of 2 division
  uint16_t rh = ((ch >> 15) & 0x1FE) + 1;
  uint16_t gh = ((ch >>  7) & 0x1FE) + 1;
  uint16_t bh = ((ch <<  1) & 0x1FE) + 1;

  uint16_t rl = ((cl >> 15) & 0x1FE) + 1;
  uint16_t gl = ((cl >>  7) & 0x1FE) + 1;
  uint16_t bl = ((cl <<  1) & 0x1FE) + 1;

  // Shift right 1 to drop rounding bit and shift right 8 to divide by 256
  uint8_t r = (rh * alpha + rl * (255 - alpha)) >> 9;
  uint8_t g = (gh * alpha + gl * (255 - alpha)) >> 9;
  uint8_t b = (bh * alpha + bl * (255 - alpha)) >> 9;

  // Combine RGB colours into 24 bits
  return RGB(r,g,b);
}

rgb_t alphaBlend(uint8_t bpp, uint8_t bits, uint8_t b80, rgb_t colorh, rgb_t colorl)
{
  uint8_t alpha = (bits & b80) + (0x100 - b80 - 1);
  if (bpp == 0 || colorh == colorl || alpha == 255) {
    return colorh;
  }
  return alphaBlend(alpha, colorh, colorl);
}

// utf-8

// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    cursor top left corner x, y coordinate
    @param    font  gfxFont with fontSizeX and fontSizeY
    @param    c
    @param    color
*/
/**************************************************************************/
void TSD_GFX::drawChar(clip_t& clip, cursor_t& cursor, tsd_font_t& font, char** c, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  startWrite();

  uint8_t size_x = font.fontSizeX;
  uint8_t size_y = font.fontSizeY;

  int16_t x = cursor.x;
  int16_t y = cursor.y;

  int16_t lw;
  int16_t lhz;
  TSD_GFXfont* gfxFont;
  const TSD_GFXglyph* glyph = font.getCharGlyph(&gfxFont, c);
  font.cursorAdjust(gfxFont, &x, &y);

  bool drawbg = ifdrawbgif(colorh, bg);

  if (!glyph) { // 'Classic' built-in font
    lw = 6;
    lhz = 8 * size_y;
    if (drawbg && x - cursor.x > 0) {
      fillRectHelper(clip, cursor.x, cursor.y, x - cursor.x, lhz, bg);
    }
    if (drawbg && y - cursor.y > 0) {
      fillRectHelper(clip, cursor.x, cursor.y, lw * size_x, y - cursor.y, bg);
    }
    for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
      uint8_t line = default_font[**c * 5 + i];
      for (int8_t j = 0; j < 8; j++, line >>= 1) {
        if (line & 1) {
          if (size_x == 1 && size_y == 1)
            drawPixel(clip, x + i, y + j, colorh);
          else
            fillRectHelper(clip, x + i * size_x, y + j * size_y, size_x, size_y, colorh);
        } else if (drawbg) {
          if (size_x == 1 && size_y == 1)
            drawPixel(clip, x + i, y + j, bg);
          else
            fillRectHelper(clip, x + i * size_x, y + j * size_y, size_x, size_y, bg);
        }
      }
    }
    if (drawbg) { // If opaque, draw vertical line for last column
      if (size_x == 1 && size_y == 1)
        drawFastVLine(clip, x + 5, y, 8, bg);
      else
        fillRectHelper(clip, x + 5 * size_x, y, size_x, 8 * size_y, bg);
    }
    *c += 1;
  }
  else { // Custom font
    lw = glyph->xAdvance;
    lhz = font.yAdvHeight(gfxFont->yAdvance);

    const uint8_t* bp = (const uint8_t*)(glyph + 1);
    uint16_t w = glyph->width, h = glyph->height;
    int16_t xo = glyph->xOffset,
       yo = 1 - glyph->yOffset; // yOffset is positive

    if (drawbg && x - cursor.x + xo * size_x > 0) {
      fillRectHelper(clip, cursor.x, cursor.y, x - cursor.x + xo * size_x, lhz, bg);
    }
    if (drawbg && y - cursor.y + yo * size_y > 0) {
      fillRectHelper(clip, cursor.x, cursor.y, lw * size_x, y - cursor.y + yo * size_y, bg);
    }

    uint8_t xx, yy, bits = 0, bit = 0;

    if (size_x > 1 || size_y > 1) {
      xo = (xo + 1) >> 1;
    }

    uint8_t bpp = glyph->bpp;
    uint8_t shu, b80;
    switch (bpp) {
      case 0: b80 = 0x80; shu = 1; break;
      case 1: b80 = 0xc0; shu = 2; break;
      case 2: b80 = 0xf0; shu = 4; break;
      case 3:
      default: b80 = 0xff; shu = 8;
    }    
    for (yy = 0; yy < h; yy++) {
      for (xx = 0; xx < w; xx++) {
        if (!(bit & 7)) {
          bits = *bp++;
          bit = 0;
        }
        bit += shu;
        uint8_t b = bits & b80;
        rgb_t color = b ? alphaBlend(bpp, bits, b80, colorh, colorl) : bg;
        if (b || drawbg) {
          if (size_x == 1 && size_y == 1) {
            drawPixel(clip, x + xo + xx, y + yo + yy, color);
          }
          else {
            fillRectHelper(clip, x + (xo + xx) * size_x, y + (yo + yy) * size_y, size_x, size_y, color);
          }
        }
        bits <<= shu;
      }
      if (glyph->ver > 0) {  // not compressed
        bit = 0;  // go to next bitmap byte
      }
    }
    int16_t y2 = y + (yo + h) * size_y;
    int16_t h2 = cursor.y + lhz - y2;
    if (drawbg && h2 > 0) {
      fillRectHelper(clip, cursor.x, y2, lw * size_x, h2, bg);
    }
    int16_t w2 = lw - (xo + w);
    if (drawbg && w2 > 0) {
      fillRectHelper(clip, cursor.x + (xo + w) * size_x, cursor.y, w2 * size_x, lhz, bg);
    }
  } // End classic vs custom font
  if (drawbg && spacing > 0) {
    fillRectHelper(clip, cursor.x + lw * size_x, cursor.y, spacing * size_x, lhz, bg);
  }
  cursor.x += (lw + spacing) * size_x;

  endWrite();
}


// Draw text line
/**************************************************************************/
/*!
   @brief   Draw text line to the \r or \n character or to the end of the text
    @param    cursor top left corner x, y coordinate
    @param    font  gfxFont with fontSizeX and fontSizeY
    @param    text   text line to be drawn
    @param    color
  @param    spacing extra horizontal spacing for letters
*/
/**************************************************************************/
const char*  TSD_GFX::drawTextLine(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const char* text, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  if (text) {
    char* p = (char *)text;
    char c;
    while ((c = *p) && c != '\r' && c != '\n') {
      drawChar(clip, cursor, font, &p, colorh, bg, colorl, spacing);
    }
    return (const char*) p;
  }
  return NULL;
}

// unicode

void TSD_GFX::drawChar(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const uint16_t uchar, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  char buf[8];
  toUtf8(buf, uchar);
  char* bp = buf;
  drawChar(clip, cursor, font, &bp, colorh, bg, colorl, spacing);
}

const uint16_t* TSD_GFX::drawTextLine(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const uint16_t* utext, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  if (utext) {
    const uint16_t* p = utext;
    uint16_t u;
    while ((u = *p) && u != '\r' && u != '\n') {
      char buf[8];
      toUtf8(buf, u);
      char* bp = buf;
      drawChar(clip, cursor, font, &bp, colorh, bg, colorl, spacing);
      ++p;
    }
    return p;
  }
  return NULL;
}





// color565toRGB14		- converts 16 bit 565 format color to 14 bit RGB (2 bits clear for math and sign)
// returns 00rrrrr000000000,00gggggg00000000,00bbbbb000000000
// thus not overloading sign, and allowing up to double for additions for fixed point delta

struct grec_t_ {
  int16_t r, g, b;
  void RGB14fromColor(const rgb_t color);
  rgb_t RGB14toColor();
};

typedef grec_t_  grec_t;

void grec_t_::RGB14fromColor(const rgb_t color)
{
//#ifdef COLOR_565
//  r = (color >> 2) & 0x3E00;
//  g = (color << 3) & 0x3F00;
//  b = (color << 9) & 0x3E00;
//#else
  r = (color >> 10) & 0x3F00;
  g = (color >>  2) & 0x3F00;
  b = (color <<  6) & 0x3F00;
//#endif
}

// RGB14tocolor565		- converts 14 bit RGB back to 16 bit 565 format color
rgb_t grec_t_::RGB14toColor()
{
//#ifdef COLOR_565
//  return (((r & 0x3E00) << 2) | ((g & 0x3F00) >> 3) | ((b & 0x3E00) >> 9));
//#else
  return (((r & 0x3F00) << 10) | ((g & 0x3F00) << 2) | ((b & 0x3F00) >> 6));
//#endif
}

static void nextGradHue(int8_t prc, grec_t& cc, const grec_t c1, const grec_t c2, const int16_t j, const int16_t h)
{
    int adj = prc - 50;
    if (adj > 45)
      adj = 45;
    else
    if (adj < -45)
      adj = -45;

    int16_t p = prc + (50 - prc + adj) * j * 2 / h;
    int16_t dr = (int)(c2.r - c1.r) * 50 / (p * h);
    int16_t dg = (int)(c2.g - c1.g) * 50 / (p * h);
    int16_t db = (int)(c2.b - c1.b) * 50 / (p * h);
    int q;
    q = cc.r + dr;
    if ((c2.r >= c1.r && q <= c2.r && q >= c1.r) || (c1.r >= c2.r && q <= c1.r && q >= c2.r)) cc.r = q; else
      q = 0;
    q = cc.g + dg;
    if ((c2.g >= c1.g && q <= c2.g && q >= c1.g) || (c1.g >= c2.g && q <= c1.g && q >= c2.g)) cc.g = q; else
      q = 0;
    q = cc.b + db;
    if ((c2.b >= c1.b && q <= c2.b && q >= c1.b) || (c1.b >= c2.b && q <= c1.b && q >= c2.b)) cc.b = q; else
      q = 0;
}

int ixi = 0;

void TSD_GFX::fillRectGradient(clip_t& clip, const int16_t x, const int16_t y, const int16_t w, const int16_t h, gradient_t& z)
{
  int x1 = x;
  int dx1 = 0;
  if (x1 < clip.x1) { dx1 = clip.x1 - x1; x1 = clip.x1; }
  int dx2 = x + w - clip.x2; dx2 = dx2 >= 0 ? dx2 : 0;
  int y1 = y;
  int dy1 = 0;
  if (y1 < clip.y1) { dy1 = clip.y1 - y1; y1 = clip.y1; }
  int dy2 = y + h - clip.y2; dy2 = dy2 >= 0 ? dy2 : 0;

  int w12 = w - dx1 - dx2;
  int h12 = h - dy1 - dy2;
  if (w12 <= 0 || h12 <= 0) return;

  startWrite();

  grec_t c1, c2;
  c1.RGB14fromColor(z.color1);
  c2.RGB14fromColor(z.color2);
  grec_t cc = c1;

  int8_t prc = z.percent;
  if (prc < 0) {
    prc = 0;
  }
  if (prc > 100) {
    prc = 100;
  }
  if (z.deg & 1) {
    for (int i = 0; i < w; ++i) {
//      if (i >= clip.x1 && i < clip.x2) {
        writeAddrWindow(z.deg == 3 ? w - 1 - i : i, y1, 1, h12);
        sendMDTColor(mdt_color(cc.RGB14toColor()), h12);
//      }
      nextGradHue(prc, cc, c1, c2, i, w);
    }
  }
  else {
    for (int j = 0; j < h; ++j) {
//      if (j >= clip.y1 && j < clip.y2) {
        writeAddrWindow(x1, z.deg == 2 ? h - j - 1 : j, w12, 1);
        sendMDTColor(mdt_color(cc.RGB14toColor()), w12);
//      }
      nextGradHue(prc, cc, c1, c2, j, h);
    }
  }

  endWrite();
}

void TSD_GFX::sendMDTColor(const mdt_t c, int32_t len)
{
  while (--len >= 0) {
    sendMDTColor1(c);
  }
}

void TSD_GFX::drawClippedPixel(const int16_t x, const int16_t y, const rgb_t color)
{
  writeAddrWindow(x, y, 1, 1);
  sendMDTColor1(mdt_color(color));
}

void TSD_GFX::drawClippedPixelRec(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color)
{
  writeAddrWindow(x, y, w, h);
  sendMDTColor(mdt_color(color), w * h);
}
