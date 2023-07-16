/*
  RM68140 driver 

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

class TFT_RM68140 : public TFT_Protocol {
public:
  TFT_RM68140() : TFT_Protocol() {}

protected:
  void init()
  {
    sendCmd(TFT_SLPOUT);
    delay(20);

    sendCmdData(0xD0, (uint8_t*)"\x07\x42\x18", 3);
    sendCmdData(0xD1, (uint8_t*)"\x00\x07\x10", 3);
    sendCmdData(0xD2, (uint8_t*)"\x01\x02", 2);
    sendCmdData(0xC0, (uint8_t*)"\x10\x3B\x00\x02\x11", 5);
    sendCmdByte(0xC5, 0x03);
    sendCmdData(0xC8, (uint8_t*)"\x00\x32\x36\x45\x06\x16\x37\x75\x77\x54\x0C\x00", 12);
    sendCmdByte(TFT_MADCTL, 0x0A);
    sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
    delay(120);
    sendCmd(TFT_DISPON);
    delay(25);
  }

  void rotation(const uint8_t r, const uint8_t REV)
  {
    uint8_t T[] {0x00, 0x02, 0x3B};
    switch (r % 4) {
      case 0: // Portrait
        sendCmdByte(TFT_MADCTL, (0x00 ^ REV) | (BGR << 3));
        T[1] |= ((MAD_SS ^ REV) << 4);
        setSize(getWIDTH(), getHEIGHT());
      break;
      case 1: // Landscape (Portrait + 90)
        sendCmdByte(TFT_MADCTL, (MAD_YX ^ REV) | (BGR << 3));
        T[1] |= 0x00;
        setSize(getHEIGHT(), getWIDTH());
      break;
      case 2: // Inverter portrait
        sendCmdByte(TFT_MADCTL, (0x00 ^ REV) | (BGR << 3));
        T[1] |= ((MAD_GS ^ REV) << 6);
        setSize(getWIDTH(), getHEIGHT());
      break;
      case 3: // Inverted landscape
        sendCmdByte(TFT_MADCTL, (MAD_YX ^ REV) | (BGR << 3));
        T[1] |= ((MAD_SS ^ REV) << 4) | ((MAD_GS ^ REV) << 6);
        setSize(getHEIGHT(), getWIDTH());
      break;
    }
    sendCmdData(0xB6, T, 3);
  }

};
