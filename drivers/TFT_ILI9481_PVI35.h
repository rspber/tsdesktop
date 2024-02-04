/*
  ILI9481_PVI35 driver
*/

#pragma once

#include "TFT_ILI9x81.h"

class TFT_ILI9481_PVI35 : public TFT_ILI9x81 {
public:
  TFT_ILI9481_PVI35() : TFT_ILI9x81() {}

protected:
  void init();

};
