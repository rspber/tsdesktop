/*
  TSD_SCREEN for TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

  Smooth graphics comes from Bodmer's TFT_eSPI

*/

#include "TSD_SCREEN.h"
#include <math.h>

#ifndef abs
#define abs(x) ((x)>=0?(x):-(x))
#endif

/// @TSD_SCREEN

void TSD_SCREEN::fillScreen(const rgb_t color)
{
  fillRect(_clip, _clip.x1, _clip.y1, _clip.width(), _clip.height(), color);
}

void TSD_SCREEN::drawPixel(int32_t x, int32_t y, rgb_t color)
{
  drawPixel(_clip, x, y, color);
}

void TSD_SCREEN::drawFastVLine(int32_t x, int32_t y, int32_t h, rgb_t color)
{
  drawFastVLine(_clip, x, y, h, color);
}

void TSD_SCREEN::drawFastHLine(int32_t x, int32_t y, int32_t w, rgb_t color)
{
  drawFastHLine(_clip, x, y, w, color);
}

void TSD_SCREEN::drawLine(clip_t& clip, int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgb_t color, int32_t ts, uint8_t mode)
{
  if (ts > 1) {
    int32_t u = ts / 2;
    if (x1 == x2) {
      fillRect(clip, x1 - u, y1, ts, y2 - y1, color);
      return;
    }
    if (y1 == y2)  {
      fillRect(clip, x1, y1 - u, x2 - x1, ts, color);
      return;
    }
    switch (mode) {
      case 1:		// cut
      {
        ts = ts * 5 / 4;
        u = ts / 2;
        int32_t d = u - ts;
        while (++d <= 0) {
          drawLine(clip, x1, y1 - d, x2 + d, y2, color);
        }
        --d;
        while (++d <= u) {
          drawLine(clip, x1 + d, y1, x2, y2 - d, color);
        }
        break;
      }
      default:    // flat
      {
        int32_t d = u - ts;
        if (abs(x2 - x1) > abs(y2 - y1)) {
          while (++d <= u) {
            drawLine(clip, x1, y1 + d, x2, y2 + d, color);
          }
        }
        else {
          while (++d <= u) {
            drawLine(clip, x1 + d, y1, x2 + d, y2, color);
          }
        }
      }
    }
  }
  else {
    drawLine(clip, x1, y1, x2, y2, color);
  }
}

void TSD_SCREEN::drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgb_t color, int32_t ts, uint8_t mode)
{
  drawLine(_clip, x1, y1, x2, y2, color, ts, mode);
}

void TSD_SCREEN::drawRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color, int32_t ts)
{
  if (ts > 1) {
    int32_t d = ts / 2;
    fillRect(clip, x - d, y - d, w + ts, ts, color);     // Top
    fillRect(clip, x - d, y + h - d, w + ts, ts, color); // Bottom
    fillRect(clip, x - d, y + d, ts, h - ts, color);     // Left
    fillRect(clip, x + w - d, y + d, ts, h - ts, color); // Right
  }
  else {
    drawRect(clip, x, y, w, h, color);
  }
}

void TSD_SCREEN::drawRect(int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color, int32_t ts)
{
  drawRect(_clip, x, y, w, h, color, ts);
}

void TSD_SCREEN::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color)
{
  fillRect(_clip, x, y, w, h, color);
}

void TSD_SCREEN::fillRectGradient(int32_t x, int32_t y, int32_t w, int32_t h, gradient_t& z)
{
  fillRectGradient(_clip, x, y, w, h, z);
}

void TSD_SCREEN::drawRoundRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color, int32_t ts)
{
  if (ts > 1) {
    int32_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    int32_t r = radius;
    if (r > max_radius) {
      r = max_radius;
    }
    r += ts;
    // smarter version
    int32_t u = ts / 2;
    fillRect(clip, x + r, y - u, w - 2 * r, ts, color);     // Top
    fillRect(clip, x + r, y + h - u, w - 2 * r, ts, color); // Bottom
    fillRect(clip, x - u, y + r, ts, h - 2 * r, color);     // Left
    fillRect(clip, x + w - u, y + r, ts, h - 2 * r, color); // Right
    // dodraw four corners
    int32_t d = u - ts;
    while (++d <= u ) {
      drawCircleFragment(clip, x + r, y + r, r + d, 1, color);
    }
    d = u - ts;
    while (++d <= u ) {
      drawCircleFragment(clip, x + w - r - 1, y + r, r + d, 2, color);
    }
    d = u - ts;
    while (++d <= u ) {
      drawCircleFragment(clip, x + w - r - 1, y + h - r - 1, r + d, 4, color);
    }
    d = u - ts;
    while (++d <= u ) {
      drawCircleFragment(clip, x + r, y + h - r - 1, r + d, 8, color);
    }
  }
  else {
    drawRoundRect(clip, x, y, w, h, radius, color);
  }
}

void TSD_SCREEN::drawRoundRect(int32_t x0, int32_t y0, int32_t w, int32_t h, int32_t radius, rgb_t color, int32_t ts)
{
  drawRoundRect(_clip, x0, y0, w, h, radius, color, ts);
}

void TSD_SCREEN::fillRoundRect(int32_t x0, int32_t y0, int32_t w, int32_t h, int32_t radius, rgb_t color)
{
  fillRoundRect(_clip, x0, y0, w, h, radius, color);
}

void TSD_SCREEN::drawCircle(clip_t& clip, int32_t x0, int32_t y0, int32_t r, rgb_t color, int32_t ts)
{
  int32_t u = ts / 2;
  int32_t d = u - ts;
  while (++d <= u) {
    drawCircle(clip, x0, y0, r + d, color);
  }
}

void TSD_SCREEN::drawCircle(int32_t x0, int32_t y0, int32_t r, rgb_t color, int32_t ts)
{
  drawCircle(_clip, x0, y0, r, color, ts);
}

void TSD_SCREEN::drawCircleFragment(clip_t& clip, int32_t x0, int32_t y0, int32_t r, uint8_t corners, rgb_t color, int32_t ts)
{
  int32_t u = ts / 2;
  int32_t d = u - ts;
  while (++d <= u) {
    drawCircleFragment(clip, x0, y0, r + d, corners, color);
  }
}

void TSD_SCREEN::drawCircleFragment(int32_t x, int32_t y, int32_t r, uint8_t corners, rgb_t color, int32_t ts)
{
  drawCircleFragment(_clip, x, y, r, corners, color, ts);
}

void TSD_SCREEN::fillCircle(int32_t x0, int32_t y0, int32_t r, rgb_t color)
{
  fillCircle(_clip, x0, y0, r, color);
}

void TSD_SCREEN::fillCircleFragment(int32_t x0, int32_t y0, int32_t r, uint8_t corners, int32_t delta, rgb_t color)
{
  fillCircleFragment(_clip, x0, y0, r, corners, delta, color);
}

void TSD_SCREEN::drawEllipse(int32_t x0, int32_t y0, int32_t rx, int32_t ry, rgb_t color)
{
  drawEllipse(_clip, x0, y0, rx, ry, color);
}

void TSD_SCREEN::fillEllipse(int32_t x0, int32_t y0, int32_t rx, int32_t ry, rgb_t color)
{
  fillEllipse(_clip, x0, y0, rx, ry, color);
}

void TSD_SCREEN::drawTriangle(clip_t& clip, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgb_t color, int32_t ts, uint8_t mode)
{
  drawLine(clip, x0, y0, x1, y1, color, ts, mode);
  drawLine(clip, x1, y1, x2, y2, color, ts, mode);
  drawLine(clip, x2, y2, x0, y0, color, ts, mode);
}

void TSD_SCREEN::drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgb_t color, int32_t ts, uint8_t mode)
{
  drawTriangle(_clip, x0, y0, x1, y1, x2, y2, color, ts, mode);
}

void TSD_SCREEN::fillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgb_t color)
{
  fillTriangle(_clip, x0, y0, x1, y1, x2, y2, color);
}






/***************************************************************************************
** Function name:           drawSmoothArc
** Description:             Draw a smooth arc clockwise from 6 o'clock
***************************************************************************************/
void TSD_SCREEN::drawSmoothArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool roundEnds)
{
  drawSmoothArc(_clip, x, y, r, ir, startAngle, endAngle, fg_color, bg_color, roundEnds);
}


/***************************************************************************************
** Function name:           drawArc
** Description:             Draw an arc clockwise from 6 o'clock position
***************************************************************************************/
void TSD_SCREEN::drawArc(int32_t x, int32_t y, int32_t r, int32_t ir,
                       uint32_t startAngle, uint32_t endAngle,
                       rgb_t fg_color, rgb_t bg_color,
                       bool smooth)
{
  drawArc(_clip, x, y, r, ir, startAngle, endAngle, fg_color, bg_color, smooth);
}


/***************************************************************************************
** Function name:           drawSmoothCircle
** Description:             Draw a smooth circle
***************************************************************************************/
void TSD_SCREEN::drawSmoothCircle(int32_t x, int32_t y, int32_t r, rgb_t fg_color, rgb_t bg_color)
{
  drawSmoothRoundRect(x-r, y-r, r, r-1, 0, 0, fg_color, bg_color);
}


/***************************************************************************************
** Function name:           fillSmoothCircle
** Description:             Draw a filled anti-aliased circle
***************************************************************************************/
void TSD_SCREEN::fillSmoothCircle(int32_t x, int32_t y, int32_t r, rgb_t color, rgb_t bg_color)
{
  fillSmoothCircle(_clip, x, y, r, color, bg_color);
}


/***************************************************************************************
** Function name:           drawSmoothRoundRect
** Description:             Draw a rounded rectangle
***************************************************************************************/
void TSD_SCREEN::drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, rgb_t fg_color, rgb_t bg_color, uint8_t quadrants)
{
  drawSmoothRoundRect(_clip, x, y, r, ir, w, h, fg_color, bg_color, quadrants);
}


/***************************************************************************************
** Function name:           fillSmoothRoundRect
** Description:             Draw a filled anti-aliased rounded corner rectangle
***************************************************************************************/
void TSD_SCREEN::fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, rgb_t color, rgb_t bg_color)
{
  fillSmoothRoundRect(_clip, x, y, w, h, r, color, bg_color);
}


/***************************************************************************************
** Function name:           drawSpot - maths intensive, so for small filled circles
** Description:             Draw an anti-aliased filled circle at ax,ay with radius r
***************************************************************************************/
void TSD_SCREEN::drawSpot(float ax, float ay, float r, rgb_t fg_color, rgb_t bg_color)
{
  drawWedgeLine(ax, ay, ax, ay, r, r, fg_color, bg_color);
}


/***************************************************************************************
** Function name:           drawWideLine - background colour specified or pixel read
** Description:             draw an anti-aliased line with rounded ends, width wd
***************************************************************************************/
void TSD_SCREEN::drawWideLine(float ax, float ay, float bx, float by, float wd, rgb_t fg_color, rgb_t bg_color)
{
  drawWedgeLine(ax, ay, bx, by, wd/2.0, wd/2.0, fg_color, bg_color);
}

/*
void TSD_SCREEN::drawWedgeLine(float ax, float ay, float bx, float by, float ar, float br, uint32_t fg_color, uint32_t bg_color)
{
  drawWedgeLine(ax, ay, bx, by, ar, br, fg_color, bg_color);
}
*/

/***************************************************************************************
** Function name:           drawWedgeLine - background colour specified or pixel read
** Description:             draw an anti-aliased line with different width radiused ends
***************************************************************************************/
void TSD_SCREEN::drawWedgeLine(float ax, float ay, float bx, float by, float ar, float br, rgb_t fg_color, rgb_t bg_color)
{
  drawWedgeLine(_clip, ax, ay, bx, by, ar, br, fg_color, bg_color);
}





void TSD_SCREEN::drawBitmap(int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h, rgb_t color)
{
  drawBitmap(_clip, x, y, bitmap, w, h, color);
}

void TSD_SCREEN::drawBitmap(int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h, rgb_t color, rgb_t bg)
{
  drawBitmap(_clip, x, y, bitmap, w, h, color, bg);
}

void TSD_SCREEN::drawGrayscaleBitmap(int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h)
{
  drawGrayscaleBitmap(_clip, x, y, bitmap, w, h);
}

void TSD_SCREEN::drawGrayscaleBitmap(int32_t x, int32_t y, const uint8_t* bitmap, const uint8_t* mask, int32_t w, int32_t h)
{
  drawGrayscaleBitmap(_clip, x, y, bitmap, mask, w, h);
}

void TSD_SCREEN::drawRGBBitmap(int32_t x, int32_t y, const rgb_t* bitmap, int32_t w, int32_t h)
{
  drawRGBBitmap(_clip, x, y, bitmap, w, h);
}

void TSD_SCREEN::drawRGBBitmap(int32_t x, int32_t y, const rgb_t* bitmap, const uint8_t *mask, int32_t w, int32_t h)
{
  drawRGBBitmap(_clip, x, y, bitmap, mask, w, h);
}




void TSD_SCREEN::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data)
{
  pushImage(_clip, x, y, w, h, data);
}

void TSD_SCREEN::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, rgb_t transparent)
{
  pushImage(_clip, x, y, w, h, data, transparent);
}


//  void TSD_SCREEN::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, rgb_t transparent)
//  void TSD_SCREEN::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data)

void TSD_SCREEN::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, bool bpp8, uint16_t *cmap)
{
  pushImage(_clip, x, y, w, h, data, bpp8, cmap);
}

void TSD_SCREEN::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, uint8_t  transparent, bool bpp8, uint16_t *cmap)
{
  pushImage(_clip, x, y, w, h, data, transparent, bpp8, cmap);
}

//  void TSD_SCREEN::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8,  uint16_t *cmap = nullptr)

void TSD_SCREEN::pushMaskedImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *img, uint8_t *mask)
{
  pushMaskedImage(_clip, x, y, w, h, img, mask);
}




// utf-8

void TSD_SCREEN::drawChar(cursor_t& cursor, tsd_font_t& font, const char* c, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  drawChar(_clip, cursor, font, (char **)&c, colorh, bg, colorl, spacing);
}

void TSD_SCREEN::drawTextLine(cursor_t& cursor, tsd_font_t& font, const char* text, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  drawTextLine(_clip, cursor, font, text, colorh, bg, colorl, spacing);
}

void TSD_SCREEN::drawText(const int32_t x, const int32_t y, const char* text, const TSD_GFXfont** gfxFont, const int8_t fontSize, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  if (text) {
    cursor_t cursor{ x, y };
    tsd_font_t font{gfxFont, fontSize, fontSize};
    const char* p = text;
    while (*p) {
      p = drawTextLine(_clip, cursor, font, p, colorh, bg, colorl, spacing);
      while (*p == '\r') ++p;
      if (*p == '\n') ++p;
      cursor.x = x;
      cursor.y += font.textLineHeight();
    }
  }
}

void TSD_SCREEN::drawText(const int32_t x, const int32_t y, const char* text, const int8_t fontSize, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  drawText(x, y, text, (const TSD_GFXfont**)NULL, fontSize, colorh, bg, colorl, spacing);
}

// unicode

void TSD_SCREEN::drawChar(cursor_t& cursor, tsd_font_t& font, const uint16_t uchar, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  drawChar(_clip, cursor, font, uchar, colorh, bg, colorl, spacing);
}

const uint16_t* TSD_SCREEN::drawTextLine(cursor_t& cursor, tsd_font_t& font, const uint16_t* utext, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  return drawTextLine(_clip, cursor, font, utext, colorh, bg, colorl, spacing);
}

void TSD_SCREEN::drawText(const int32_t x, const int32_t y, const uint16_t* utext, const TSD_GFXfont** gfxFont, const int8_t fontSize, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  if (utext) {
    cursor_t cursor{ x, y };
    tsd_font_t font{gfxFont, fontSize, fontSize};
    const uint16_t* p = utext;
    while (*p) {
      p = drawTextLine(_clip, cursor, font, p, colorh, bg, colorl, spacing);
      while (*p == '\r') ++p;
      if (*p == '\n') ++p;
      cursor.x = x;
      cursor.y += font.textLineHeight();
    }
  }
}

void TSD_SCREEN::drawText(const int32_t x, const int32_t y, const uint16_t* utext, const int8_t fontSize, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  drawText(x, y, utext, (const TSD_GFXfont**)NULL, fontSize, colorh, bg, colorl, spacing);
}


void TSD_SCREEN::pushMDTBuffer(clip_t& window, const uint8_t* buffer)
{
  int i = window.x1;
  int w = window.width();
  int di = 0;
  if (i < 0) {
    di -= i;
    w += i;
    i = 0;
  }
  if (w > 0) {
    if (i + w > _clip.x2) {
      w = _clip.x2 - i;
    }
    if (w > 0) {
      int jhw = di;
      for (int j = window.y1; j < window.y2; ++j ) {
        if (j >= 0 && j < _clip.height()) {
          drawMDTBuffer(_clip.x1 + i, _clip.y1 + j, w, 1, &buffer[jhw * MDT_SIZE]);
        }
        jhw += window.width();
      }
    }
  }
}

void TSD_SCREEN::pushMDTBuffer(clip_t& window, const uint8_t* buffer, const rgb_t transparent)
{
  mdt_t b = mdt_color(transparent);
  int i = window.x1;
  int w = window.width();
  int di = 0;
  if (i < 0) {
    di -= i;
    w += i;
    i = 0;
  }
  if (w > 0) {
    if (i + w > _clip.x2) {
      w = _clip.x2 - i;
    }
    if (w > 0) {
      int jhw = di;
      for (int j = window.y1; j < window.y2; ++j ) {
        if (j >= 0 && j < _clip.height()) {
          const uint8_t* p = &buffer[jhw * MDT_SIZE];
          int k0 = 0;
          int iw = 0;
          for (int k = 0; k < w; ++k ) {
            bool ok = true;
            if (MDT_SIZE > 2) {
              ok = (*p++ == ((b >> 16) & 0xff));
            }
            ok = (*p++ == ((b >> 8) & 0xff)) && ok;
            ok = (*p++ == (b & 0xff)) && ok;
            if (ok) {
              if (iw > 0) {
                drawMDTBuffer(_clip.x1 + k0 + i, _clip.y1 + j, iw, 1, &buffer[(jhw + k0) * MDT_SIZE]);
                iw = 0;
              }
            }
            else {
              if (iw == 0) {
                k0 = k;
              }
              ++iw;
            }
          }
          if (iw > 0) {
            drawMDTBuffer(_clip.x1 + k0 + i, _clip.y1 + j, iw, 1, &buffer[(jhw + k0) * MDT_SIZE]);
          }
        }
        jhw += window.width();
      }
    }
  }
}

