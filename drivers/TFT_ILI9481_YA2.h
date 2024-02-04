/*
  ILI9481_YA2 driver
*/

#pragma once

#include "TFT_ILI9x81.h"

class TFT_ILI9481_YA2 : public TFT_ILI9x81 {
public:
  TFT_ILI9481_YA2() : TFT_ILI9x81() {}

protected:
  void init();

};
