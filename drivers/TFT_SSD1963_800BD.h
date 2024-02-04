/*
  SSD1963_800BD driver
*/

#pragma once

#include <TFT_SSD196x.h>

class TFT_SSD1963_800BD : public TFT_SSD196x {
public:
  TFT_SSD1963_800BD() : TFT_SSD196x() {}

protected:
  void init();

};
