/*
  SSD196x abstract driver
*/

#include "TFT_SSD196x.h"
#include <Arduino.h>

#define BGR 1

void TFT_SSD196x::rotation(const uint8_t r, const uint8_t REV)
{
  switch (r % 4) {
    case 0:
      sendCmdByte(TFT_MADCTL, ((MAD_YX | MAD_GS) ^ REV) | (BGR << 3));
      setSize(getWIDTH(), getHEIGHT());
    break;
    case 1:
      sendCmdByte(TFT_MADCTL, (0x00 ^ REV) | (BGR << 3));
      setSize(getHEIGHT(), getWIDTH());
    break;
    case 2:
      sendCmdByte(TFT_MADCTL, ((MAD_YX | MAD_SS) ^ REV) | (BGR << 3));
      setSize(getWIDTH(), getHEIGHT());
    break;
    case 3:
      sendCmdByte(TFT_MADCTL, ((MAD_SS | MAD_GS) ^ REV) | (BGR << 3));
      setSize(getHEIGHT(), getWIDTH());
    break;
  }
}
