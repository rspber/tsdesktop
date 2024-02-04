/*
  RM68140 driver 
*/

#include "TFT_RM68140.h"
#include <Arduino.h>

#define BGR 1

void TFT_RM68140::init()
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

void TFT_RM68140::rotation(const uint8_t r, const uint8_t REV)
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
