/*
  ILI9xxx abstract driver
*/

#pragma once

#include <TFT_Protocol.h>

class TFT_ILI9xxx : public TFT_Protocol {
public:
  TFT_ILI9xxx() : TFT_Protocol() {}

protected:
  void rotation(const uint8_t r, const uint8_t REV);

};
