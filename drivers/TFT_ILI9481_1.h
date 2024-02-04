/*
  ILI9481_1 driver, default
*/

#pragma once

#include "TFT_ILI9x81.h"

class TFT_ILI9481_1 : public TFT_ILI9x81 {
public:
  TFT_ILI9481_1() : TFT_ILI9x81() {}

protected:
  void init();

};
