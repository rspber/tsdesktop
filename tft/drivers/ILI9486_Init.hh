/*
  ILI9486 init
*/

  sendCmd(TFT_SWRESET);
  delay(120);
  sendCmd(TFT_SLPOUT);
  delay(120);
  sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
  sendCmdData(ILI9486_PWCTR1, (uint8_t*)"\x0E\0E", 2);
  sendCmdData(ILI9486_PWCTR2, (uint8_t*)"\x41\x00", 2);
  sendCmdByte(ILI9486_PWCTR3, 0x55);
  sendCmdData(ILI9486_VCOMCTR1, (uint8_t*)"\x00\x00\x00\x00", 4);
  sendCmdData(ILI9486_PVGAMMA, (uint8_t*)"\x0F\x1F\x1C\x0C\x0F\x08\x48\x98\x37\x0A\x13\x04\x11\x0D\x00", 15);
  sendCmdData(ILI9486_NVGAMMA, (uint8_t*)"\x0F\x32\x2E\x0B\x0D\x05\x47\x75\x37\x06\x10\x03\x24\x20\x00", 15);
  //sendCmd(TFT_INVON);
  sendCmdByte(TFT_MADCTL, 0x48);
  sendCmd(TFT_DISPON);
  delay(150);
