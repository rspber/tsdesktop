/*
  SSD1963_480 driver
*/

#pragma once

#include <TFT_SSD196x.h>

class TFT_SSD1963_480 : public TFT_SSD196x {
public:
  TFT_SSD1963_480() : TFT_SSD196x() {}

protected:
  void init();

};
