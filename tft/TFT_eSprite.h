/*
  TFT_eSprite

  Copyright (c) 2024, rspber (https://github.com/rspber)

  https://github.com/Bodmer/TFT_eSPI/Extensions/Sprite.h

  Bodmer's license.txt in file /licenses/Bodmer_license.txt

*/

#pragma once

#include "BufferedDisplay.h"
#include "TSD_SCREEN.h"

struct bounds_t {
  int16_t min_x, min_y, max_x, max_y;
  int32_t cosra, sinra;
  void getRotatedBounds(int16_t angle, int16_t disp_w, int16_t disp_h, int16_t xPivot, int16_t yPivot);
};

class TFT_eSprite : public BufferedDisplay {
public:
  explicit TFT_eSprite(TSD_SCREEN *tft) : BufferedDisplay(0,0,1,1), _tft(tft) {}

  void createSprite(int16_t w, int16_t h);
  void deleteSprite();

           // Push a rotated copy of Sprite to TFT with optional transparent colour
  bool     pushRotated(TSD_SCREEN *tft, int16_t angle, rgb_t transp = WHITE);

           // Push a rotated copy of Sprite to TFT with optional transparent colour
  bool     pushRotated(int16_t angle, rgb_t transp = WHITE);

  void pushSprite(const int16_t x, const int16_t y);
  void pushSprite(const int16_t x, const int16_t y, const rgb_t transp);

  void fillSprite(const rgb_t color);

  TSD_SCREEN *_tft;
};
