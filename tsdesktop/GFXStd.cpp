/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "GFXStd.h"
#include "Display.h"

/// @GFXPixel

void GFXPixel::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  display.drawPixel(clip, x + x1, y + y1, color);
}


/// @GFXLine

void drawLine(clip_t* clip, int16_t x, int16_t y, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts)
{
  x1 += x;
  y1 += y;
  x2 += x;
  y2 += y;
  if (ts > 1) {
    int16_t u = ts / 2;
    if (x1 == x2) {
      display.fillRect(clip, x1 - u, y1, ts, y2 - y1, color);
      return;
    }
    if (y1 == y2)  {
      display.fillRect(clip, x1, y1 - u, x2 - x1, ts, color);
      return;
    }
    if (abs(x2 - x1) > abs(y2 - y1)) {
      int16_t d = u - ts;
      while (++d <= u) {
        display.drawLine(clip, x1, y1 + d, x2, y2 + d, color);
      }
    }
    else {
      int16_t d = u - ts;
      while (++d <= u) {
        display.drawLine(clip, x1 + d, y1, x2 + d, y2, color);
      }
    }
  }
  else {
    display.drawLine(clip, x1, y1, x2, y2, color);
  }
}

void GFXLine::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  drawLine(clip, x, y, x1, y1, x2, y2, color, ts);
}


/// @GFXRect

void GFXRect::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  x += x1;
  y += y1;
  if (ts > 1) {
    int16_t d = ts / 2;
    display.fillRect(clip, x - d, y - d, w + ts, ts, color);     // Top
    display.fillRect(clip, x - d, y + h - d, w + ts, ts, color); // Bottom
    display.fillRect(clip, x - d, y + d, ts, h - ts, color);     // Left
    display.fillRect(clip, x + w - d, y + d, ts, h - ts, color); // Right
  }
  else {
    display.drawRect(clip, x, y, w, h, color);
  }
}


/// @GFXRoundRect

void GFXRoundRect::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  x += x1;
  y += y1;
  if (ts > 1) {
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    int16_t r = radius;
    if (r > max_radius) {
      r = max_radius;
    }
    // smarter version
    int16_t u = ts / 2;
    display.fillRect(clip, x + r, y - u, w - 2 * r, ts, color);     // Top
    display.fillRect(clip, x + r, y + h - u, w - 2 * r, ts, color); // Bottom
    display.fillRect(clip, x - u, y + r, ts, h - 2 * r, color);     // Left
    display.fillRect(clip, x + w - u, y + r, ts, h - 2 * r, color); // Right
    // dodraw four corners
    display.startWrite();
    int16_t d = u - ts;
    while (++d <= u ) {
      display.drawCircleHelper(clip, x + r, y + r, r + d, 1, color);
    }
    d = u - ts;
    while (++d <= u ) {
      display.drawCircleHelper(clip, x + w - r - 1, y + r, r + d, 2, color);
    }
    d = u - ts;
    while (++d <= u ) {
      display.drawCircleHelper(clip, x + w - r - 1, y + h - r - 1, r + d, 4, color);
    }
    d = u - ts;
    while (++d <= u ) {
      display.drawCircleHelper(clip, x + r, y + h - r - 1, r + d, 8, color);
    }
    display.endWrite();
  }
  else {
    display.drawRoundRect(clip, x, y, w, h, radius, color);
  }
}


/// @GFXFillRect

void GFXFillRect::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  display.fillRect(clip, x + x1, y + y1, w, h, color);
}


/// @GFXFillRoundRect

void GFXFillRoundRect::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  display.fillRoundRect(clip, x + x1, y + y1, w, h, radius, color);
}


/// @GFXCircle

void GFXCircle::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  int16_t u = ts / 2;
  int16_t d = u - ts;
  while (++d <= u) {
    display.drawCircle(clip, x + x0, y + y0, r + d, color);
  }
}


/// @GFXCircleFragment

void GFXCircleFragment::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  display.startWrite();
  int16_t u = ts / 2;
  int16_t d = u - ts;
  while (++d <= u) {
    display.drawCircleHelper(clip, x + x0, y + y0, r + d, fragment, color);
  }
  display.endWrite();
}


/// @GFXFillCircle

void GFXFillCircle::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  display.fillCircle(clip, x + x0, y + y0, r, color);
}


/// @GFXFillCircleFragment

void GFXFillCircleFragment::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  display.startWrite();
  display.fillCircleHelper(clip, x + x0, y + y0, r, fragment, delta, color);
  display.endWrite();
}


/// @GFXTriangle

void GFXTriangle::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  drawLine(clip, x, y, x1, y1, x2, y2, color, ts);
  drawLine(clip, x, y, x2, y2, x3, y3, color, ts);
  drawLine(clip, x, y, x3, y3, x1, y1, color, ts);
}


/// @GFXFillTriangle

void GFXFillTriangle::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  display.fillTriangle(clip, x + x1, y + y1, x + x2, y + y2, x + x3, y + y3, color);
}


/// @GFXGrayscaleBitmap

void GFXGrayscaleBitmap::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  if (mask) {
    display.drawGrayscaleBitmap(clip, x + x1, y + y1, bitmap, mask, w, h);
  }
  else {
    display.drawGrayscaleBitmap(clip, x + x1, y + y1, bitmap, w, h);
  }
}


/// @GFXBitmap

void GFXBitmap::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  if (bg != 1) {
    display.drawBitmap(clip, x + x1, y + y1, bitmap, w, h, color, bg);
  }
  else {
    display.drawBitmap(clip, x + x1, y + y1, bitmap, w, h, color);
  }
}


/// @GFXRGBBitmap

void GFXRGBBitmap::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  if (mask) {
    display.drawRGBBitmap(clip, x + x1, y + y1, bitmap, mask, w, h);
  }
  else {
    display.drawRGBBitmap(clip, x + x1, y + y1, bitmap, w, h);
  }
}


/// @GFXChar

void GFXChar::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  cursor_t cursor{(int16_t)(x + x1), (int16_t)(y + y1)};
  display.drawChar(clip, &cursor, &font, c, color);
}

/// @GFXTextLine

void GFXTextLine::dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color)
{
  cursor_t cursor{(int16_t)(x + x1), (int16_t)(y + y1)};
  display.drawTextLine(clip, &cursor, &font, text, color);
}
