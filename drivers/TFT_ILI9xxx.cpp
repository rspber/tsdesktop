/*
  ILI9xxx abstract driver
*/

#include "TFT_ILI9xxx.h"
#include <Arduino.h>

#define BGR 1

void TFT_ILI9xxx::rotation(const uint8_t r, const uint8_t REV)
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
