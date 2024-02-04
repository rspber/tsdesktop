/*
  R61581 driver 
*/

#pragma once

#include <TFT_Protocol.h>

class TFT_R61581 : public TFT_Protocol {
public:
  TFT_R61581() : TFT_Protocol() {}

protected:
  void init();
  void rotation(const uint8_t r, const uint8_t REV);

};
