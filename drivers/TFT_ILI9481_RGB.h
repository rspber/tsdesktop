/*
  ILI9481_RGB driver
*/

#pragma once

#include "TFT_ILI9x81.h"

class TFT_ILI9481_RGB : public TFT_ILI9x81 {
public:
  TFT_ILI9481_RGB() : TFT_ILI9x81() {}

protected:
  void init();

};
