/*
  RM68140 driver 
*/

#pragma once

#include <TFT_Protocol.h>

class TFT_RM68140 : public TFT_Protocol {
public:
  TFT_RM68140() : TFT_Protocol() {}

protected:
  void init();
  void rotation(const uint8_t r, const uint8_t REV);

};
