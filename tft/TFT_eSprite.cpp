/*
  TFT_eSprite

  Copyright (c) 2024, rspber (https://github.com/rspber)

  https://github.com/Bodmer/TFT_eSPI/Extensions/Sprite.cpp

  Bodmer's license.txt in file /licenses/Bodmer_license.txt

*/

#include "TFT_eSprite.h"
#include "SerialUSB.h"
#include "Arduino.h"

/***************************************************************************************
** Function name:           createSprite
** Description:             Create a sprite (bitmap) of defined width and height
***************************************************************************************/
void TFT_eSprite::createSprite(int16_t w, int16_t h)
{
  clip_t ac = {0, 0, w, h};
  adjust(ac);
}

/***************************************************************************************
** Function name:           deleteSprite
** Description:             Delete the sprite to free up memory (RAM)
***************************************************************************************/
void TFT_eSprite::deleteSprite() {
  createSprite(1,1);
}


/***************************************************************************************
** Function name:           pushRotated - Fast fixed point integer maths version
** Description:             Push rotated Sprite to TFT screen
***************************************************************************************/
#define FP_SCALE 10
bool TFT_eSprite::pushRotated(TSD_SCREEN *tft, int16_t angle, rgb_t transp)
{
  // Bounding box parameters
  bounds_t bds;

  // Get the bounding box of this rotated source Sprite relative to Sprite pivot
  bds.getRotatedBounds(angle, tft->width(), tft->height(), tft->getPivotX(), tft->getPivotY());

  uint8_t sline_buffer[(bds.max_x - bds.min_x + 1) * MDT_SIZE];

  int32_t xt = bds.min_x - tft->getPivotX();
  int32_t yt = bds.min_y - tft->getPivotY();
  uint32_t xe = width() << FP_SCALE;
  uint32_t ye = height() << FP_SCALE;
  rgb_t tpcolor = transp;

  tft->startWrite();

  if( bds.max_x < tft->clip.x2 ) {
    bds.max_x = tft->clip.x2;
  }

  if( bds.max_y < tft->clip.y2 ) {
    bds.max_y = tft->clip.y2;
  }

  // Scan destination bounding box and fetch transformed pixels from source Sprite
  for (int32_t y = bds.min_y; y <= bds.max_y; y++, yt++) {
    int32_t x = bds.min_x;
    uint32_t xs = (bds.cosra * xt - (bds.sinra * yt - (getPivotX() << FP_SCALE)) + (1 << (FP_SCALE - 1)));
    uint32_t ys = (bds.sinra * xt + (bds.cosra * yt + (getPivotY() << FP_SCALE)) + (1 << (FP_SCALE - 1)));

    while ((xs >= xe || ys >= ye) && x < bds.max_x) {
       x++; 
       xs += bds.cosra; 
       ys += bds.sinra;
    }
    if (x == bds.max_x) {
      continue;
    }

    int pixel_count = 0;
    do {
      int32_t xp = xs >> FP_SCALE;
      int32_t yp = ys >> FP_SCALE;
      rgb_t rp = readPixel(clip, xp, yp);

      if (transp != WHITE && tpcolor == rp) {
        if (pixel_count) {
          // TFT window is already clipped, so this is faster than pushImage()
          tft->writeAddrWindow(x - pixel_count, y, pixel_count, 1);
          tft->writeMDTBuffer((const uint8_t*)sline_buffer, pixel_count);
          pixel_count = 0;
        }
      }
      else {
        *(mdt_t*)&sline_buffer[pixel_count++ * MDT_SIZE] = mdt_color(rp);
      }
    } while (++x < bds.max_x && (xs += bds.cosra) < xe && (ys += bds.sinra) < ye);
    if (pixel_count) {
      // TFT window is already clipped, so this is faster than pushImage()
      tft->writeAddrWindow(x - pixel_count, y, pixel_count, 1);
      tft->writeMDTBuffer((const uint8_t*)sline_buffer, pixel_count);
    }
  }

  tft->endWrite();

  return true;
}


           // Push a rotated copy of Sprite to TFT with optional transparent colour
bool TFT_eSprite::pushRotated(int16_t angle, rgb_t transp)
{
  return pushRotated(_tft, angle, transp);
}


/***************************************************************************************
** Function name:           rotatedBounds
** Description:             Get bounding box of a rotated Sprite wrt pivot
***************************************************************************************/
void bounds_t::getRotatedBounds(int16_t angle, int16_t disp_w, int16_t disp_h, int16_t xPivot, int16_t yPivot)
{
  // Trig values for the rotation
  float radAngle = -angle * 0.0174532925; // Convert degrees to radians
  float sina = sin(radAngle);
  float cosa = cos(radAngle);

  disp_w -= xPivot; // w is now right edge coordinate relative to xPivot
  disp_h -= yPivot; // h is now bottom edge coordinate relative to yPivot

  // Calculate new corner coordinates
  int16_t x0 = -xPivot * cosa - yPivot * sina;
  int16_t y0 =  xPivot * sina - yPivot * cosa;

  int16_t x1 =  disp_w * cosa - yPivot * sina;
  int16_t y1 = -disp_w * sina - yPivot * cosa;

  int16_t x2 =  disp_h * sina + disp_w * cosa;
  int16_t y2 =  disp_h * cosa - disp_w * sina;

  int16_t x3 =  disp_h * sina - xPivot * cosa;
  int16_t y3 =  disp_h * cosa + xPivot * sina;

  // Find bounding box extremes, enlarge box to accomodate rounding errors
  min_x = x0-2;
  if (x1 < min_x) min_x = x1-2;
  if (x2 < min_x) min_x = x2-2;
  if (x3 < min_x) min_x = x3-2;

  max_x = x0+2;
  if (x1 > max_x) max_x = x1+2;
  if (x2 > max_x) max_x = x2+2;
  if (x3 > max_x) max_x = x3+2;

  min_y = y0-2;
  if (y1 < min_y) min_y = y1-2;
  if (y2 < min_y) min_y = y2-2;
  if (y3 < min_y) min_y = y3-2;

  max_y = y0+2;
  if (y1 > max_y) max_y = y1+2;
  if (y2 > max_y) max_y = y2+2;
  if (y3 > max_y) max_y = y3+2;

  sinra = round(sina * (1<<FP_SCALE));
  cosra = round(cosa * (1<<FP_SCALE));
}


/***************************************************************************************
** Function name:           pushSprite
** Description:             Push the sprite to the TFT at x, y
***************************************************************************************/
void TFT_eSprite::pushSprite(const int16_t x, const int16_t y) {
    clip_t old = clip;
    setPos(x, y);
    push(_tft);
    clip = old;
}


/***************************************************************************************
** Function name:           pushSprite
** Description:             Push the sprite to the TFT at x, y with transparent colour
***************************************************************************************/
void TFT_eSprite::pushSprite(const int16_t x, const int16_t y, const rgb_t transp) {
    clip_t old = clip;
    setPos(x, y);
    pushTransp(_tft, transp);
    clip = old;
}


/***************************************************************************************
** Function name:           fillSprite
** Description:             Fill the whole sprite with defined colour
***************************************************************************************/
void TFT_eSprite::fillSprite(const rgb_t color) {
    clear(color);
}
