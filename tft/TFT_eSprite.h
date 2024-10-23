/*
  TFT_eSprite

  Copyright (c) 2024, rspber (https://github.com/rspber)

  https://github.com/Bodmer/TFT_eSPI/Extensions/Sprite.h

  Bodmer's license.txt in file /licenses/Bodmer_license.txt

*/

#pragma once

#include <BufferedDisplay.h>
#include "TFT_eSPI.h"

struct bounds_t {
  int16_t min_x, min_y, max_x, max_y;
  int32_t cosra, sinra;
  void getRotatedBounds(int16_t angle, int16_t disp_w, int16_t disp_h, int16_t xPivot, int16_t yPivot);
};

class TFT_eSprite : public BufferedDisplay {
public:
  explicit TFT_eSprite() : BufferedDisplay(0, 0, 1, 1) {}
  TFT_eSprite(int16_t w, int16_t h) : BufferedDisplay(0, 0, w, h) {}

  void createSprite(int16_t w, int16_t h);
  void deleteSprite();

           // Push a rotated copy of Sprite to TFT with optional transparent colour
  bool     pushRotated(TSD_SCREEN& tft, int16_t angle, rgb_t transp = WHITE);

           // Get the destination Sprite bounding box for a rotated copy of this Sprite
  bool     getRotatedBounds(int16_t angle, bounds_t& bds, int16_t dist_w, int16_t disp_h);

  void pushSprite(TSD_SCREEN& display, const int16_t x, const int16_t y);
  void pushSprite(TSD_SCREEN& display, const int16_t x, const int16_t y, const rgb_t transp);

  void fillSprite(const rgb_t color);

};
