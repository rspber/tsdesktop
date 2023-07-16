/*
  ILI9481_YA1 driver   效果不好

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include "TFT_ILI9x81.h"

class TFT_ILI9481_YA1 : public TFT_ILI9x81 {
public:
  TFT_ILI9481_YA1() : TFT_ILI9x81() {}

protected:
  void init()
  {
    sendCmd(TFT_SLPOUT);
    delay(20);
    sendCmdData(ILI9481_POWERSET, (uint8_t*)"\x07\x42\x1B", 3);
    sendCmdData(ILI9481_VCOMCTL, (uint8_t*)"\x00\x14\x1B", 3);
    sendCmdData(ILI9481_POWNORMD, (uint8_t*)"\x01\x12", 2);
    sendCmdData(ILI9481_PANELDRV, (uint8_t*)"\x10\x3B\x00\x02\x01", 5);
    sendCmdByte(ILI9481_FRAINCTL, 0x03);
    sendCmdData(ILI9481_GAMMASET, (uint8_t*)"\x00\x46\x44\x50\x04\x16\x33\x13\x77\x05\x0F\x00", 12);
    sendCmdByte(TFT_MADCTL, 0x0A);
    sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
//    sendCmd(TFT_INVON);
    sendCmd(TFT_DISPON);
  }

};
