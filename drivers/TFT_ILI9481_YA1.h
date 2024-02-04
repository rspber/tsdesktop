/*
  ILI9481_YA1 driver   效果不好
*/

#pragma once

#include "TFT_ILI9x81.h"

class TFT_ILI9481_YA1 : public TFT_ILI9x81 {
public:
  TFT_ILI9481_YA1() : TFT_ILI9x81() {}

protected:
  void init();

};
