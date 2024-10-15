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
  fillRect(clip, clip.x1, clip.y1, clip.width(), clip.height(), color);
}

void TSD_SCREEN::drawPixel(int16_t x, int16_t y, rgb_t color)
{
  drawPixel(clip, x, y, color);
}

void TSD_SCREEN::drawFastVLine(int16_t x, int16_t y, int16_t h, rgb_t color)
{
  drawFastVLine(clip, x, y, h, color);
}

void TSD_SCREEN::drawFastHLine(int16_t x, int16_t y, int16_t w, rgb_t color)
{
  drawFastHLine(clip, x, y, w, color);
}

void TSD_SCREEN::drawLine(clip_t& clip, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts, uint8_t mode)
{
  if (ts > 1) {
    int16_t u = ts / 2;
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
        int16_t d = u - ts;
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
        int16_t d = u - ts;
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

void TSD_SCREEN::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts, uint8_t mode)
{
  drawLine(clip, x1, y1, x2, y2, color, ts, mode);
}

void TSD_SCREEN::drawRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color, int16_t ts)
{
  if (ts > 1) {
    int16_t d = ts / 2;
    fillRect(clip, x - d, y - d, w + ts, ts, color);     // Top
    fillRect(clip, x - d, y + h - d, w + ts, ts, color); // Bottom
    fillRect(clip, x - d, y + d, ts, h - ts, color);     // Left
    fillRect(clip, x + w - d, y + d, ts, h - ts, color); // Right
  }
  else {
    drawRect(clip, x, y, w, h, color);
  }
}

void TSD_SCREEN::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color, int16_t ts)
{
  drawRect(clip, x, y, w, h, color, ts);
}

void TSD_SCREEN::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color)
{
  fillRect(clip, x, y, w, h, color);
}

void TSD_SCREEN::fillRectGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t& z)
{
  fillRectGradient(clip, x, y, w, h, z);
}

void TSD_SCREEN::drawRoundRect(clip_t& clip, int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, rgb_t color, int16_t ts)
{
  if (ts > 1) {
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    int16_t r = radius;
    if (r > max_radius) {
      r = max_radius;
    }
    r += ts;
    // smarter version
    int16_t u = ts / 2;
    fillRect(clip, x + r, y - u, w - 2 * r, ts, color);     // Top
    fillRect(clip, x + r, y + h - u, w - 2 * r, ts, color); // Bottom
    fillRect(clip, x - u, y + r, ts, h - 2 * r, color);     // Left
    fillRect(clip, x + w - u, y + r, ts, h - 2 * r, color); // Right
    // dodraw four corners
    int16_t d = u - ts;
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

void TSD_SCREEN::drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, rgb_t color, int16_t ts)
{
  drawRoundRect(clip, x0, y0, w, h, radius, color, ts);
}

void TSD_SCREEN::fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, rgb_t color)
{
  fillRoundRect(clip, x0, y0, w, h, radius, color);
}

void TSD_SCREEN::drawCircle(clip_t& clip, int16_t x0, int16_t y0, int16_t r, rgb_t color, int16_t ts)
{
  int16_t u = ts / 2;
  int16_t d = u - ts;
  while (++d <= u) {
    drawCircle(clip, x0, y0, r + d, color);
  }
}

void TSD_SCREEN::drawCircle(int16_t x0, int16_t y0, int16_t r, rgb_t color, int16_t ts)
{
  drawCircle(clip, x0, y0, r, color, ts);
}

void TSD_SCREEN::drawCircleFragment(clip_t& clip, int16_t x0, int16_t y0, int16_t r, uint8_t corners, rgb_t color, int16_t ts)
{
  int16_t u = ts / 2;
  int16_t d = u - ts;
  while (++d <= u) {
    drawCircleFragment(clip, x0, y0, r + d, corners, color);
  }
}

void TSD_SCREEN::drawCircleFragment(int16_t x, int16_t y, int16_t r, uint8_t corners, rgb_t color, int16_t ts)
{
  drawCircleFragment(clip, x, y, r, corners, color, ts);
}

void TSD_SCREEN::fillCircle(int16_t x0, int16_t y0, int16_t r, rgb_t color)
{
  fillCircle(clip, x0, y0, r, color);
}

void TSD_SCREEN::fillCircleFragment(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, rgb_t color)
{
  fillCircleFragment(clip, x0, y0, r, corners, delta, color);
}

void TSD_SCREEN::drawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, rgb_t color)
{
  drawEllipse(clip, x0, y0, rx, ry, color);
}

void TSD_SCREEN::fillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, rgb_t color)
{
  fillEllipse(clip, x0, y0, rx, ry, color);
}

void TSD_SCREEN::drawTriangle(clip_t& clip, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts, uint8_t mode)
{
  drawLine(clip, x0, y0, x1, y1, color, ts, mode);
  drawLine(clip, x1, y1, x2, y2, color, ts, mode);
  drawLine(clip, x2, y2, x0, y0, color, ts, mode);
}

void TSD_SCREEN::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color, int16_t ts, uint8_t mode)
{
  drawTriangle(clip, x0, y0, x1, y1, x2, y2, color, ts, mode);
}

void TSD_SCREEN::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, rgb_t color)
{
  fillTriangle(clip, x0, y0, x1, y1, x2, y2, color);
}



/***************************************************************************************
** Function name:           alphaBlend
** Description:             Blend 16bit foreground and background
*************************************************************************************x*/
uint16_t TSD_SCREEN::alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc)
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

/***************************************************************************************
** Function name:           alphaBlend
** Description:             Blend 16bit foreground and background with dither
*************************************************************************************x*/
uint16_t TSD_SCREEN::alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither)
{
  if (dither) {
    int16_t alphaDither = (int16_t)alpha - dither + random() % (2*dither+1); // +/-4 randomised
    alpha = (uint8_t)alphaDither;
    if (alphaDither <  0) alpha = 0;
    if (alphaDither >255) alpha = 255;
  }

  return alphaBlend(alpha, fgc, bgc);
}

/***************************************************************************************
** Function name:           alphaBlend
** Description:             Blend 24bit foreground and background with optional dither
*************************************************************************************x*/
uint32_t TSD_SCREEN::alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither)
{

  if (dither) {
    int16_t alphaDither = (int16_t)alpha - dither + random() % (2*dither+1); // +/-dither randomised
    alpha = (uint8_t)alphaDither;
    if (alphaDither <  0) alpha = 0;
    if (alphaDither >255) alpha = 255;
  }

  uint32_t rxx = bgc & 0xFF0000;
  rxx += ((fgc & 0xFF0000) - rxx) * alpha >> 8;
  uint32_t xgx = bgc & 0x00FF00;
  xgx += ((fgc & 0x00FF00) - xgx) * alpha >> 8;
  uint32_t xxb = bgc & 0x0000FF;
  xxb += ((fgc & 0x0000FF) - xxb) * alpha >> 8;
  return (rxx & 0xFF0000) | (xgx & 0x00FF00) | (xxb & 0x0000FF);
}

/***************************************************************************************
** Description:  Constants for anti-aliased line drawing on TFT and in Sprites
***************************************************************************************/
constexpr float PixelAlphaGain   = 255.0;
constexpr float LoAlphaTheshold  = 1.0/32.0;
constexpr float HiAlphaTheshold  = 1.0 - LoAlphaTheshold;
constexpr float deg2rad      = 3.14159265359/180.0;


/***************************************************************************************
** Function name:           drawPixel (alpha blended)
** Description:             Draw a pixel blended with the screen or bg pixel colour
***************************************************************************************/
uint16_t TSD_SCREEN::drawPixel(clip_t& clip, int32_t x, int32_t y, uint32_t color, uint8_t alpha, uint32_t bg_color)
{
  if (bg_color == 0x00FFFFFF) bg_color = readPixel(clip, x, y);
  color = fastBlend(alpha, color, bg_color);
  drawPixel(clip, x, y, color);
  return color;
}


/***************************************************************************************
** Function name:           drawSmoothArc
** Description:             Draw a smooth arc clockwise from 6 o'clock
***************************************************************************************/
void TSD_SCREEN::drawSmoothArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool roundEnds)
// Centre at x,y
// r = arc outer radius, ir = arc inner radius. Inclusive so arc thickness = r - ir + 1
// Angles in range 0-360
// Arc foreground colour anti-aliased with background colour at edges
// anti-aliased roundEnd is optional, default is anti-aliased straight end
// Note: rounded ends extend the arc angle so can overlap, user sketch to manage this.
{
//???  inTransaction = true;
  startWrite();

  if (endAngle != startAngle && (startAngle != 0 || endAngle != 360))
  {
    float sx = -sinf(startAngle * deg2rad);
    float sy = +cosf(startAngle * deg2rad);
    float ex = -sinf(  endAngle * deg2rad);
    float ey = +cosf(  endAngle * deg2rad);

    if (roundEnds)
    { // Round ends
      sx = sx * (r + ir)/2.0 + x;
      sy = sy * (r + ir)/2.0 + y;
      drawSpot(sx, sy, (r - ir)/2.0, fg_color, bg_color);

      ex = ex * (r + ir)/2.0 + x;
      ey = ey * (r + ir)/2.0 + y;
      drawSpot(ex, ey, (r - ir)/2.0, fg_color, bg_color);
    }
    else
    { // Square ends
      float asx = sx * ir + x;
      float asy = sy * ir + y;
      float aex = sx *  r + x;
      float aey = sy *  r + y;
      drawWedgeLine(asx, asy, aex, aey, 0.3, 0.3, fg_color, bg_color);

      asx = ex * ir + x;
      asy = ey * ir + y;
      aex = ex *  r + x;
      aey = ey *  r + y;
      drawWedgeLine(asx, asy, aex, aey, 0.3, 0.3, fg_color, bg_color);
    }

    // Draw arc
    drawArc(x, y, r, ir, startAngle, endAngle, fg_color, bg_color);

  }
  else // Draw full 360
  {
    drawArc(x, y, r, ir, 0, 360, fg_color, bg_color);
  }

//???  inTransaction = lockTransaction;
//???  end_tft_write();
  endWrite();
}


/***************************************************************************************
** Function name:           sqrt_fraction (private function)
** Description:             Smooth graphics support function for alpha derivation
***************************************************************************************/
// Compute the fixed point square root of an integer and
// return the 8 MS bits of fractional part.
// Quicker than sqrt() for processors that do not have an FPU (e.g. RP2040)
inline uint8_t TSD_SCREEN::sqrt_fraction(uint32_t num) {
  if (num > (0x40000000)) return 0;
  uint32_t bsh = 0x00004000;
  uint32_t fpr = 0;
  uint32_t osh = 0;

  // Auto adjust from U8:8 up to U15:16
  while (num>bsh) {bsh <<= 2; osh++;}

  do {
    uint32_t bod = bsh + fpr;
    if(num >= bod)
    {
      num -= bod;
      fpr = bsh + bod;
    }
    num <<= 1;
  } while(bsh >>= 1);

  return fpr>>osh;
}

/***************************************************************************************
** Function name:           drawArc
** Description:             Draw an arc clockwise from 6 o'clock position
***************************************************************************************/
// Centre at x,y
// r = arc outer radius, ir = arc inner radius. Inclusive, so arc thickness = r-ir+1
// Angles MUST be in range 0-360
// Arc foreground fg_color anti-aliased with background colour along sides
// smooth is optional, default is true, smooth=false means no antialiasing
// Note: Arc ends are not anti-aliased (use drawSmoothArc instead for that)
void TSD_SCREEN::drawArc(int32_t x, int32_t y, int32_t r, int32_t ir,
                       uint32_t startAngle, uint32_t endAngle,
                       uint32_t fg_color, uint32_t bg_color,
                       bool smooth)
{
  if (endAngle   > 360)   endAngle = 360;
  if (startAngle > 360) startAngle = 360;
  if (startAngle == endAngle) return;
  if (r < ir) transpose(r, ir);  // Required that r > ir
  if (r <= 0 || ir < 0) return;  // Invalid r, ir can be zero (circle sector)

  if (endAngle < startAngle) {
    // Arc sweeps through 6 o'clock so draw in two parts
    if (startAngle < 360) drawArc(x, y, r, ir, startAngle, 360, fg_color, bg_color, smooth);
    if (endAngle == 0) return;
    startAngle = 0;
  }
// inTransaction = true;
  startWrite();

  int32_t xs = 0;        // x start position for quadrant scan
  uint8_t alpha = 0;     // alpha value for blending pixels

  uint32_t r2 = r * r;   // Outer arc radius^2
  if (smooth) r++;       // Outer AA zone radius
  uint32_t r1 = r * r;   // Outer AA radius^2
  int16_t w  = r - ir;   // Width of arc (r - ir + 1)
  uint32_t r3 = ir * ir; // Inner arc radius^2
  if (smooth) ir--;      // Inner AA zone radius
  uint32_t r4 = ir * ir; // Inner AA radius^2

  //     1 | 2
  //    ---¦---    Arc quadrant index
  //     0 | 3
  // Fixed point U16.16 slope table for arc start/end in each quadrant
  uint32_t startSlope[4] = {0, 0, 0xFFFFFFFF, 0};
  uint32_t   endSlope[4] = {0, 0xFFFFFFFF, 0, 0};

  // Ensure maximum U16.16 slope of arc ends is ~ 0x8000 0000
  constexpr float minDivisor = 1.0f/0x8000;

  // Fill in start slope table and empty quadrants
  float fabscos = fabsf(cosf(startAngle * deg2rad));
  float fabssin = fabsf(sinf(startAngle * deg2rad));

  // U16.16 slope of arc start
  uint32_t slope = (fabscos/(fabssin + minDivisor)) * (float)(1UL<<16);

  // Update slope table, add slope for arc start
  if (startAngle <= 90) {
    startSlope[0] =  slope;
  }
  else if (startAngle <= 180) {
    startSlope[1] =  slope;
  }
  else if (startAngle <= 270) {
    startSlope[1] = 0xFFFFFFFF;
    startSlope[2] = slope;
  }
  else {
    startSlope[1] = 0xFFFFFFFF;
    startSlope[2] =  0;
    startSlope[3] = slope;
  }

  // Fill in end slope table and empty quadrants
  fabscos  = fabsf(cosf(endAngle * deg2rad));
  fabssin  = fabsf(sinf(endAngle * deg2rad));

  // U16.16 slope of arc end
  slope   = (uint32_t)((fabscos/(fabssin + minDivisor)) * (float)(1UL<<16));

  // Work out which quadrants will need to be drawn and add slope for arc end
  if (endAngle <= 90) {
    endSlope[0] = slope;
    endSlope[1] =  0;
    startSlope[2] =  0;
  }
  else if (endAngle <= 180) {
    endSlope[1] = slope;
    startSlope[2] =  0;
  }
  else if (endAngle <= 270) {
    endSlope[2] =  slope;
  }
  else {
    endSlope[3] =  slope;
  }

  // Scan quadrant
  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    uint32_t len[4] = { 0,  0,  0,  0}; // Pixel run length
    int32_t  xst[4] = {-1, -1, -1, -1}; // Pixel run x start
    uint32_t dy2 = (r - cy) * (r - cy);

    // Find and track arc zone start point
    while ((r - xs) * (r - xs) + dy2 >= r1) xs++;

    for (int32_t cx = xs; cx < r; cx++)
    {
      // Calculate radius^2
      uint32_t hyp = (r - cx) * (r - cx) + dy2;

      // If in outer zone calculate alpha
      if (hyp > r2) {
        alpha = ~sqrt_fraction(hyp); // Outer AA zone
      }
      // If within arc fill zone, get line start and lengths for each quadrant
      else if (hyp >= r3) {
        // Calculate U16.16 slope
        slope = ((r - cy) << 16)/(r - cx);
        if (slope <= startSlope[0] && slope >= endSlope[0]) { // slope hi -> lo
          xst[0] = cx; // Bottom left line end
          len[0]++;
        }
        if (slope >= startSlope[1] && slope <= endSlope[1]) { // slope lo -> hi
          xst[1] = cx; // Top left line end
          len[1]++;
        }
        if (slope <= startSlope[2] && slope >= endSlope[2]) { // slope hi -> lo
          xst[2] = cx; // Bottom right line start
          len[2]++;
        }
        if (slope <= endSlope[3] && slope >= startSlope[3]) { // slope lo -> hi
          xst[3] = cx; // Top right line start
          len[3]++;
        }
        continue; // Next x
      }
      else {
        if (hyp <= r4) break;  // Skip inner pixels
        alpha = sqrt_fraction(hyp); // Inner AA zone
      }

      if (alpha < 16) continue;  // Skip low alpha pixels

      // If background is read it must be done in each quadrant
      uint16_t pcol = fastBlend(alpha, fg_color, bg_color);
      // Check if an AA pixels need to be drawn
      slope = ((r - cy)<<16)/(r - cx);
      if (slope <= startSlope[0] && slope >= endSlope[0]) // BL
        drawPixel(clip, x + cx - r, y - cy + r, pcol);
      if (slope >= startSlope[1] && slope <= endSlope[1]) // TL
        drawPixel(clip, x + cx - r, y + cy - r, pcol);
      if (slope <= startSlope[2] && slope >= endSlope[2]) // TR
        drawPixel(clip, x - cx + r, y + cy - r, pcol);
      if (slope <= endSlope[3] && slope >= startSlope[3]) // BR
        drawPixel(clip, x - cx + r, y - cy + r, pcol);
    }
    // Add line in inner zone
    if (len[0]) drawFastHLine(clip, x + xst[0] - len[0] + 1 - r, y - cy + r, len[0], fg_color); // BL
    if (len[1]) drawFastHLine(clip, x + xst[1] - len[1] + 1 - r, y + cy - r, len[1], fg_color); // TL
    if (len[2]) drawFastHLine(clip, x - xst[2] + r, y + cy - r, len[2], fg_color); // TR
    if (len[3]) drawFastHLine(clip, x - xst[3] + r, y - cy + r, len[3], fg_color); // BR
  }

  // Fill in centre lines
  if (startAngle ==   0 || endAngle == 360) drawFastVLine(clip, x, y + r - w, w, fg_color); // Bottom
  if (startAngle <=  90 && endAngle >=  90) drawFastHLine(clip, x - r + 1, y, w, fg_color); // Left
  if (startAngle <= 180 && endAngle >= 180) drawFastVLine(clip, x, y - r + 1, w, fg_color); // Top
  if (startAngle <= 270 && endAngle >= 270) drawFastHLine(clip, x + r - w, y, w, fg_color); // Right

//???  inTransaction = lockTransaction;
//???  end_tft_write();
  endWrite();
}

/***************************************************************************************
** Function name:           drawSmoothCircle
** Description:             Draw a smooth circle
***************************************************************************************/
// To have effective anti-aliasing the circle will be 3 pixels thick
void TSD_SCREEN::drawSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t fg_color, uint32_t bg_color)
{
  drawSmoothRoundRect(x-r, y-r, r, r-1, 0, 0, fg_color, bg_color);
}

/***************************************************************************************
** Function name:           fillSmoothCircle
** Description:             Draw a filled anti-aliased circle
***************************************************************************************/
void TSD_SCREEN::fillSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t color, uint32_t bg_color)
{
  if (r <= 0) return;

//  inTransaction = true;
  startWrite();

  drawFastHLine(clip, x - r, y, 2 * r + 1, color);
  int32_t xs = 1;
  int32_t cx = 0;

  int32_t r1 = r * r;
  r++;
  int32_t r2 = r * r;

  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t dy2 = (r - cy) * (r - cy);
    for (cx = xs; cx < r; cx++)
    {
      int32_t hyp2 = (r - cx) * (r - cx) + dy2;
      if (hyp2 <= r1) break;
      if (hyp2 >= r2) continue;

      uint8_t alpha = ~sqrt_fraction(hyp2);
      if (alpha > 246) break;
      xs = cx;
      if (alpha < 9) continue;

      if (bg_color == 0x00FFFFFF) {
        drawPixel(clip, x + cx - r, y + cy - r, color, alpha, bg_color);
        drawPixel(clip, x - cx + r, y + cy - r, color, alpha, bg_color);
        drawPixel(clip, x - cx + r, y - cy + r, color, alpha, bg_color);
        drawPixel(clip, x + cx - r, y - cy + r, color, alpha, bg_color);
      }
      else {
        uint16_t pcol = drawPixel(clip, x + cx - r, y + cy - r, color, alpha, bg_color);
        drawPixel(clip, x - cx + r, y + cy - r, pcol);
        drawPixel(clip, x - cx + r, y - cy + r, pcol);
        drawPixel(clip, x + cx - r, y - cy + r, pcol);
      }
    }
    drawFastHLine(clip, x + cx - r, y + cy - r, 2 * (r - cx) + 1, color);
    drawFastHLine(clip, x + cx - r, y - cy + r, 2 * (r - cx) + 1, color);
  }
//???  inTransaction = lockTransaction;
//???  end_tft_write();
  endWrite();
}


/***************************************************************************************
** Function name:           drawSmoothRoundRect
** Description:             Draw a rounded rectangle
***************************************************************************************/
// x,y is top left corner of bounding box for a complete rounded rectangle
// r = arc outer corner radius, ir = arc inner radius. Arc thickness = r-ir+1
// w and h are width and height of the bounding rectangle
// If w and h are < radius (e.g. 0,0) a circle will be drawn with centre at x+r,y+r
// Arc foreground fg_color anti-aliased with background colour at edges
// A subset of corners can be drawn by specifying a quadrants mask. A bit set in the
// mask means draw that quadrant (all are drawn if parameter missing):
//   0x1 | 0x2
//    ---¦---    Arc quadrant mask select bits (as in drawCircleHelper fn)
//   0x8 | 0x4
void TSD_SCREEN::drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, uint32_t fg_color, uint32_t bg_color, uint8_t quadrants)
{
  if (r < ir) transpose(r, ir); // Required that r > ir
  if (r <= 0 || ir < 0) return; // Invalid

  w -= 2*r;
  h -= 2*r;

  if (w < 0) w = 0;
  if (h < 0) h = 0;

//???  inTransaction = true;
  startWrite();

  x += r;
  y += r;

  uint16_t t = r - ir + 1;
  int32_t xs = 0;
  int32_t cx = 0;

  int32_t r2 = r * r;   // Outer arc radius^2
  r++;
  int32_t r1 = r * r;   // Outer AA zone radius^2

  int32_t r3 = ir * ir; // Inner arc radius^2
  ir--;
  int32_t r4 = ir * ir; // Inner AA zone radius^2

  uint8_t alpha = 0;

  // Scan top left quadrant x y r ir fg_color  bg_color
  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t len = 0;  // Pixel run length
    int32_t lxst = 0; // Left side run x start
    int32_t rxst = 0; // Right side run x start
    int32_t dy2 = (r - cy) * (r - cy);

    // Find and track arc zone start point
    while ((r - xs) * (r - xs) + dy2 >= r1) xs++;

    for (cx = xs; cx < r; cx++)
    {
      // Calculate radius^2
      int32_t hyp = (r - cx) * (r - cx) + dy2;

      // If in outer zone calculate alpha
      if (hyp > r2) {
        alpha = ~sqrt_fraction(hyp); // Outer AA zone
      }
      // If within arc fill zone, get line lengths for each quadrant
      else if (hyp >= r3) {
        rxst = cx; // Right side start
        len++;     // Line segment length
        continue;  // Next x
      }
      else {
        if (hyp <= r4) break;  // Skip inner pixels
        alpha = sqrt_fraction(hyp); // Inner AA zone
      }

      if (alpha < 16) continue;  // Skip low alpha pixels

      // If background is read it must be done in each quadrant - TODO
      uint16_t pcol = fastBlend(alpha, fg_color, bg_color);
      if (quadrants & 0x8) drawPixel(clip, x + cx - r, y - cy + r + h, pcol);     // BL
      if (quadrants & 0x1) drawPixel(clip, x + cx - r, y + cy - r, pcol);         // TL
      if (quadrants & 0x2) drawPixel(clip, x - cx + r + w, y + cy - r, pcol);     // TR
      if (quadrants & 0x4) drawPixel(clip, x - cx + r + w, y - cy + r + h, pcol); // BR
    }
    // Fill arc inner zone in each quadrant
    lxst = rxst - len + 1; // Calculate line segment start for left side
    if (quadrants & 0x8) drawFastHLine(clip, x + lxst - r, y - cy + r + h, len, fg_color);     // BL
    if (quadrants & 0x1) drawFastHLine(clip, x + lxst - r, y + cy - r, len, fg_color);         // TL
    if (quadrants & 0x2) drawFastHLine(clip, x - rxst + r + w, y + cy - r, len, fg_color);     // TR
    if (quadrants & 0x4) drawFastHLine(clip, x - rxst + r + w, y - cy + r + h, len, fg_color); // BR
  }

  // Draw sides
  if ((quadrants & 0xC) == 0xC) fillRect(clip, x, y + r - t + h, w + 1, t, fg_color); // Bottom
  if ((quadrants & 0x9) == 0x9) fillRect(clip, x - r + 1, y, t, h + 1, fg_color);     // Left
  if ((quadrants & 0x3) == 0x3) fillRect(clip, x, y - r + 1, w + 1, t, fg_color);     // Top
  if ((quadrants & 0x6) == 0x6) fillRect(clip, x + r - t + w, y, t, h + 1, fg_color); // Right

//  inTransaction = lockTransaction;
//  end_tft_write();
  endWrite();
}

/***************************************************************************************
** Function name:           fillSmoothRoundRect
** Description:             Draw a filled anti-aliased rounded corner rectangle
***************************************************************************************/
void TSD_SCREEN::fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color, uint32_t bg_color)
{
//  inTransaction = true;
  startWrite();

  int32_t xs = 0;
  int32_t cx = 0;

  // Limit radius to half width or height
  if (r < 0)   r = 0;
  if (r > w/2) r = w/2;
  if (r > h/2) r = h/2;

  y += r;
  h -= 2*r;
  fillRect(clip, x, y, w, h, color);

  h--;
  x += r;
  w -= 2*r+1;

  int32_t r1 = r * r;
  r++;
  int32_t r2 = r * r;

  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t dy2 = (r - cy) * (r - cy);
    for (cx = xs; cx < r; cx++)
    {
      int32_t hyp2 = (r - cx) * (r - cx) + dy2;
      if (hyp2 <= r1) break;
      if (hyp2 >= r2) continue;

      uint8_t alpha = ~sqrt_fraction(hyp2);
      if (alpha > 246) break;
      xs = cx;
      if (alpha < 9) continue;

      drawPixel(clip, x + cx - r, y + cy - r, color, alpha, bg_color);
      drawPixel(clip, x - cx + r + w, y + cy - r, color, alpha, bg_color);
      drawPixel(clip, x - cx + r + w, y - cy + r + h, color, alpha, bg_color);
      drawPixel(clip, x + cx - r, y - cy + r + h, color, alpha, bg_color);
    }
    drawFastHLine(clip, x + cx - r, y + cy - r, 2 * (r - cx) + 1 + w, color);
    drawFastHLine(clip, x + cx - r, y - cy + r + h, 2 * (r - cx) + 1 + w, color);
  }
//???  inTransaction = lockTransaction;
//???  end_tft_write();
  endWrite();
}

/***************************************************************************************
** Function name:           drawWedgeLine - background colour specified or pixel read
** Description:             draw an anti-aliased line with different width radiused ends
***************************************************************************************/
void TSD_SCREEN::drawWedgeLine(float ax, float ay, float bx, float by, float ar, float br, uint32_t fg_color, uint32_t bg_color)
{
  if ( (ar < 0.0) || (br < 0.0) )return;
  if ( (fabsf(ax - bx) < 0.01f) && (fabsf(ay - by) < 0.01f) ) bx += 0.01f;  // Avoid divide by zero

  // Find line bounding box
  int32_t x0 = (int32_t)floorf(fminf(ax-ar, bx-br));
  int32_t x1 = (int32_t) ceilf(fmaxf(ax+ar, bx+br));
  int32_t y0 = (int32_t)floorf(fminf(ay-ar, by-br));
  int32_t y1 = (int32_t) ceilf(fmaxf(ay+ar, by+br));

//???  if (!clip.clipClip(x0, y0, x1, y1)) return;

  // Establish x start and y start
  int32_t ys = ay;
  if ((ax-ar)>(bx-br)) ys = by;

  float rdt = ar - br; // Radius delta
  float alpha = 1.0f;
  ar += 0.5;

  uint16_t bg = bg_color;
  float xpax, ypay, bax = bx - ax, bay = by - ay;

//  begin_nin_write();
//  inTransaction = true;
  startWrite();

  int32_t xs = x0;
  // Scan bounding box from ys down, calculate pixel intensity from distance to line
  for (int32_t yp = ys; yp <= y1; yp++) {
    bool swin = true;  // Flag to start new window area
    bool endX = false; // Flag to skip pixels
    ypay = yp - ay;
    for (int32_t xp = xs; xp <= x1; xp++) {
      if (endX) if (alpha <= LoAlphaTheshold) break;  // Skip right side
      xpax = xp - ax;
      alpha = ar - wedgeLineDistance(xpax, ypay, bax, bay, rdt);
      if (alpha <= LoAlphaTheshold ) continue;
      // Track edge to minimise calculations
      if (!endX) { endX = true; xs = xp; }
      if (alpha > HiAlphaTheshold) {
        #ifdef GC9A01_DRIVER
          drawPixel(clip, xp, yp, fg_color);
        #else
//???          if (swin) { setWindow(xp, yp, x1-xp+1, 1); swin = false; }
          if (swin) { writeAddrWindow(xp, yp, x1-xp+1, 1); swin = false; }
//          pushColor(fg_color);
          sendMDTColor1(fg_color);
        #endif
        continue;
      }
      //Blend color with background and plot
      if (bg_color == 0x00FFFFFF) {
        bg = readPixel(clip, xp, yp); swin = true;
      }
      #ifdef GC9A01_DRIVER
        uint16_t pcol = fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg);
        drawPixel(clip, xp, yp, pcol);
        swin = swin;
      #else
//???        if (swin) { setWindow(xp, yp, x1-xp+1, 1); swin = false; }
        if (swin) { writeAddrWindow(xp, yp, x1-xp+1, 1); swin = false; }
//???        pushColor(fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg));
        sendMDTColor1(fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg));
      #endif
    }
  }

  // Reset x start to left side of box
  xs = x0;
  // Scan bounding box from ys-1 up, calculate pixel intensity from distance to line
  for (int32_t yp = ys-1; yp >= y0; yp--) {
    bool swin = true;  // Flag to start new window area
    bool endX = false; // Flag to skip pixels
    ypay = yp - ay;
    for (int32_t xp = xs; xp <= x1; xp++) {
      if (endX) if (alpha <= LoAlphaTheshold) break;  // Skip right side of drawn line
      xpax = xp - ax;
      alpha = ar - wedgeLineDistance(xpax, ypay, bax, bay, rdt);
      if (alpha <= LoAlphaTheshold ) continue;
      // Track line boundary
      if (!endX) { endX = true; xs = xp; }
      if (alpha > HiAlphaTheshold) {
        #ifdef GC9A01_DRIVER
          drawPixel(clip, xp, yp, fg_color);
        #else
//???          if (swin) { setWindow(xp, yp, x1-xp+1, yp); swin = false; }
          if (swin) { writeAddrWindow(xp, yp, x1-xp+1, yp); swin = false; }
//???          pushColor(fg_color);
          sendMDTColor1(fg_color);
        #endif
        continue;
      }
      //Blend colour with background and plot
      if (bg_color == 0x00FFFFFF) {
        bg = readPixel(clip, xp, yp); swin = true;
      }
      #ifdef GC9A01_DRIVER
        uint16_t pcol = fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg);
        drawPixel(xp, yp, pcol);
        swin = swin;
      #else
//???        if (swin) { setWindow(xp, yp, x1-xp+1, yp); swin = false; }
        if (swin) { writeAddrWindow(xp, yp, x1-xp+1, yp); swin = false; }
//???        pushColor(fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg));
        sendMDTColor1(fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg));
      #endif
    }
  }

//???  inTransaction = lockTransaction;
//???  end_nin_write();
  endWrite();
}


/***************************************************************************************
** Function name:           lineDistance - private helper function for drawWedgeLine
** Description:             returns distance of px,py to closest part of a to b wedge
***************************************************************************************/
inline float TSD_SCREEN::wedgeLineDistance(float xpax, float ypay, float bax, float bay, float dr)
{
  float h = fmaxf(fminf((xpax * bax + ypay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
  float dx = xpax - bax * h, dy = ypay - bay * h;
  return sqrtf(dx * dx + dy * dy) + h * dr;
}


void TSD_SCREEN::drawSpot(float ax, float ay, float r, uint32_t fg_color, uint32_t bg_color)
{
  // Filled circle can be created by the wide line function with zero line length
  drawWedgeLine(ax, ay, ax, ay, r, r, fg_color, bg_color);
}

void TSD_SCREEN::drawWideLine(float ax, float ay, float bx, float by, float wd, uint32_t fg_color, uint32_t bg_color)
{
  drawWedgeLine(ax, ay, bx, by, wd/2.0, wd/2.0, fg_color, bg_color);
}
/*
void TSD_SCREEN::drawWedgeLine(float ax, float ay, float bx, float by, float ar, float br, uint32_t fg_color, uint32_t bg_color)
{
  drawWedgeLine(ax, ay, bx, by, ar, br, fg_color, bg_color);
}
*/



void TSD_SCREEN::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color)
{
  drawBitmap(clip, x, y, bitmap, w, h, color);
}

void TSD_SCREEN::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, rgb_t color, rgb_t bg)
{
  drawBitmap(clip, x, y, bitmap, w, h, color, bg);
}

void TSD_SCREEN::drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h)
{
  drawGrayscaleBitmap(clip, x, y, bitmap, w, h);
}

void TSD_SCREEN::drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t* bitmap, const uint8_t* mask, int16_t w, int16_t h)
{
  drawGrayscaleBitmap(clip, x, y, bitmap, mask, w, h);
}

void TSD_SCREEN::drawRGBBitmap(int16_t x, int16_t y, const rgb_t* bitmap, int16_t w, int16_t h)
{
  drawRGBBitmap(clip, x, y, bitmap, w, h);
}

void TSD_SCREEN::drawRGBBitmap(int16_t x, int16_t y, const rgb_t* bitmap, const uint8_t *mask, int16_t w, int16_t h)
{
  drawRGBBitmap(clip, x, y, bitmap, mask, w, h);
}

// utf-8

void TSD_SCREEN::drawChar(cursor_t& cursor, font_t& font, const char* c, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  drawChar(clip, cursor, font, (char **)&c, colorh, bg, colorl, spacing);
}

void TSD_SCREEN::drawTextLine(cursor_t& cursor, font_t& font, const char* text, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  drawTextLine(clip, cursor, font, text, colorh, bg, colorl, spacing);
}

void TSD_SCREEN::drawText(const int16_t x, const int16_t y, const char* text, const GFXfont** gfxFont, const int8_t fontSize, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  if (text) {
    cursor_t cursor{ x, y };
    font_t font{gfxFont, fontSize, fontSize};
    const char* p = text;
    while (*p) {
      p = drawTextLine(clip, cursor, font, p, colorh, bg, colorl, spacing);
      while (*p == '\r') ++p;
      if (*p == '\n') ++p;
      cursor.x = x;
      cursor.y += font.textLineHeight();
    }
  }
}

void TSD_SCREEN::drawText(const int16_t x, const int16_t y, const char* text, const int8_t fontSize, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  drawText(x, y, text, (const GFXfont**)NULL, fontSize, colorh, bg, colorl, spacing);
}

// unicode

void TSD_SCREEN::drawChar(cursor_t& cursor, font_t& font, const uint16_t uchar, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  drawChar(clip, cursor, font, uchar, colorh, bg, colorl, spacing);
}

const uint16_t* TSD_SCREEN::drawTextLine(cursor_t& cursor, font_t& font, const uint16_t* utext, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  return drawTextLine(clip, cursor, font, utext, colorh, bg, colorl, spacing);
}

void TSD_SCREEN::drawText(const int16_t x, const int16_t y, const uint16_t* utext, const GFXfont** gfxFont, const int8_t fontSize, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  if (utext) {
    cursor_t cursor{ x, y };
    font_t font{gfxFont, fontSize, fontSize};
    const uint16_t* p = utext;
    while (*p) {
      p = drawTextLine(clip, cursor, font, p, colorh, bg, colorl, spacing);
      while (*p == '\r') ++p;
      if (*p == '\n') ++p;
      cursor.x = x;
      cursor.y += font.textLineHeight();
    }
  }
}

void TSD_SCREEN::drawText(const int16_t x, const int16_t y, const uint16_t* utext, const int8_t fontSize, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  drawText(x, y, utext, (const GFXfont**)NULL, fontSize, colorh, bg, colorl, spacing);
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
    if (i + w > clip.x2) {
      w = clip.x2 - i;
    }
    if (w > 0) {
      int jhw = di;
      for (int j = window.y1; j < window.y2; ++j ) {
        if (j >= 0 && j < clip.height()) {
          drawMDTBuffer(clip.x1 + i, clip.y1 + j, w, 1, &buffer[jhw * MDT_SIZE]);
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
    if (i + w > clip.x2) {
      w = clip.x2 - i;
    }
    if (w > 0) {
      int jhw = di;
      for (int j = window.y1; j < window.y2; ++j ) {
        if (j >= 0 && j < clip.height()) {
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
                drawMDTBuffer(clip.x1 + k0 + i, clip.y1 + j, iw, 1, &buffer[(jhw + k0) * MDT_SIZE]);
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
            drawMDTBuffer(clip.x1 + k0 + i, clip.y1 + j, iw, 1, &buffer[(jhw + k0) * MDT_SIZE]);
          }
        }
        jhw += window.width();
      }
    }
  }
}
