/*
  ILI9481_CPT29 driver
*/

#pragma once

#include "TFT_ILI9x81.h"

class TFT_ILI9481_CPT29 : public TFT_ILI9x81 {
public:
  TFT_ILI9481_CPT29() : TFT_ILI9x81() {}

protected:
  void init();

};
