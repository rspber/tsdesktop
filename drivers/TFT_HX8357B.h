/*
  HX8357B driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_ILI9xxx.h>

#define HX8357_PANELDRV   0xC0    // W/R (7) Panel Driving Setting
#define HX8357_FRAINCTL   0xC5    // W/R (1) Frame rate and Inversion Control
#define HX8357_GAMMASET   0xC8    // W/R (12) Gamma Setting
#define HX8357_POWERSET   0xD0    // W/R (3) Power Setting
#define HX8357_VCOMCTL    0xD1    // W/R (3) VCOM Control
#define HX8357_POWNORMD   0xD2    // W/R (2) Power Setting for Normal Mode

class TFT_HX8357B : public TFT_ILI9xxx {
public:
  TFT_HX8357B() : TFT_ILI9xxx() {}

protected:
  void init()
  {
    sendCmd(TFT_SLPOUT);
    delay(20);
    sendCmdData(HX8357_POWERSET, (uint8_t*)"\x07\x42\x18", 3);
    sendCmdData(HX8357_VCOMCTL, (uint8_t*)"\x00\x07\x10", 3);
    sendCmdData(HX8357_POWNORMD, (uint8_t*)"\x01\x02", 2);
    sendCmdData(HX8357_PANELDRV, (uint8_t*)"\x10\x3B\x00\x02\x11", 5);
    sendCmdByte(HX8357_FRAINCTL, 0x08);
    sendCmdData(HX8357_GAMMASET, (uint8_t*)"\x00\x32\x36\x45\x06\x16\x37\x75\x77\x54\x0C\x00", 12);
    sendCmdByte(TFT_MADCTL, 0x0a);
    sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
    delay(120);
    sendCmd(TFT_DISPON);
    delay(25);
  }

};
