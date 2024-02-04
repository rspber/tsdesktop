/*
  SSD1963_800ALT driver
*/

#pragma once

#include <TFT_SSD196x.h>

class TFT_SSD1963_800ALT : public TFT_SSD196x {
public:
  TFT_SSD1963_800ALT() : TFT_SSD196x() {}

protected:
  void init();

};
