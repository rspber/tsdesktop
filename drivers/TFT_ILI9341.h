/*
  ILI9341 driver

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_Driver.h>

class TFT_ILI9341 : public TFT_Driver {
public:
  TFT_ILI9341(const int16_t w, const int16_t h)
   : TFT_Driver(w, h) {}

  void begin(TFT_SPI* aspi, const int16_t aRST = -1);

  void setRotation(const int8_t rotation);
};
