/*
  GC9A01 driver 
*/

#pragma once

#include <TFT_ILI9xxx.h>

class TFT_GC9A01 : public TFT_ILI9xxx {
public:
  TFT_GC9A01() : TFT_ILI9xxx() {}

protected:
  void init();

};
