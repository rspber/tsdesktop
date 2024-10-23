/*
  TFT_eSPI runner

  Copyright (c) 2024, rspber (https://github.com/rspber)

  With this stuff TFT_eSPI examples can be run with (almost) NO changes.

*/

#pragma once

#include <TFT_SCREEN.h>
#include <math.h>

class TFT_eSPI : public TFT_SCREEN {
public:
  TFT_eSPI() : TFT_SCREEN() {}

  void init() { begin(); }

};

#include <TFT_eSprite.h>
