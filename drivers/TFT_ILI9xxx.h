/*
  ILI9xxx abstract driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_Protocol.h>

#define BGR 1

#define MAD_MY  0x80  // 00 top to botom, 80 bottom to top
#define MAD_MX  0x40  // 00 left to right, 40 right to left
#define MAD_YX  0x20  // it means that X and Y are exchanged, wrongly called MV
//#define MAD_MV  0x10  // vertical refresh direction, wrongly called ML
//#define MAD_RGB 0x00
//#define MAD_BGR 0x08
//#define MAD_MH  0x04 // horizontal refresh direction, 
#define MAD_SS  0x02 // horizontal flip
#define MAD_GS  0x01 // vertical flip

class TFT_ILI9xxx : public TFT_Protocol {
public:
  TFT_ILI9xxx() : TFT_Protocol() {}

protected:
  void rotation(const uint8_t r, const uint8_t REV)
  {
    switch (r % 4) {
    case 0:
      sendCmdByte(TFT_MADCTL, (MAD_MX ^ REV) | (BGR << 3));
      setSize(getWIDTH(), getHEIGHT());
      break;
    case 1:
      sendCmdByte(TFT_MADCTL, (MAD_YX ^ REV) | (BGR << 3));
      setSize(getHEIGHT(), getWIDTH());
      break;
    case 2:
      sendCmdByte(TFT_MADCTL, (MAD_MY ^ REV) | (BGR << 3));
      setSize(getWIDTH(), getHEIGHT());
      break;
    case 3:
      sendCmdByte(TFT_MADCTL, ((MAD_YX | MAD_MX | MAD_MY) ^ REV) | (BGR << 3));
      setSize(getHEIGHT(), getWIDTH());
      break;
    }
  }

};
