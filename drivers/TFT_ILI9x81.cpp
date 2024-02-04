/*
  ILI9481 abstract driver
*/

#include "TFT_ILI9x81.h"
#include <Arduino.h>

#define BGR 1

void TFT_ILI9x81::rotation(const uint8_t r, const uint8_t REV)
{
  switch (r % 4) {
  case 0: // Portrait
    sendCmdByte(TFT_MADCTL, (MAD_SS ^ REV) | (BGR << 3));
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 1: // Landscape (Portrait + 90)
    sendCmdByte(TFT_MADCTL, (MAD_YX ^ REV) | (BGR << 3));
    setSize(getHEIGHT(), getWIDTH());
    break;
  case 2: // Inverter portrait
    sendCmdByte(TFT_MADCTL, (MAD_GS ^ REV) | (BGR << 3));
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 3: // Inverted landscape
    sendCmdByte(TFT_MADCTL, ((MAD_SS | MAD_GS | MAD_YX) ^ REV) | (BGR << 3));
    setSize(getHEIGHT(), getWIDTH());
    break;
  }
}
