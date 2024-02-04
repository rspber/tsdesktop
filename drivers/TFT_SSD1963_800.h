/*
  SSD1963_800 driver
*/

#pragma once

#include <TFT_SSD196x.h>

class TFT_SSD1963_800 : public TFT_SSD196x {
public:
  TFT_SSD1963_800() : TFT_SSD196x() {}

protected:
  void init();

};
