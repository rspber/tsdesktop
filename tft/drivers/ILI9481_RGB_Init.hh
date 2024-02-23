/*
  ILI9481_RGB init
*/

  sendCmd(TFT_SLPOUT);
  delay(20);
  sendCmdData(ILI9481_POWERSET, (uint8_t*)"\x07\x41\x1D", 3);
  sendCmdData(ILI9481_VCOMCTL, (uint8_t*)"\x00\x2B\x1F", 3);
  sendCmdData(ILI9481_POWNORMD, (uint8_t*)"\x01\x11", 2);
  sendCmdData(ILI9481_PANELDRV, (uint8_t*)"\x10\x3B\x00\x02\x11\x00", 6);
  sendCmdByte(ILI9481_FRAINCTL, 0x03);
  sendCmdByte(ILI9481_INTRFCTL, 0x80);
  sendCmdData(ILI9481_GAMMASET, (uint8_t*)"\x00\x14\x33\x10\x00\x16\x44\x36\x77\x00\x0F\x00", 12);
  sendCmdByte(ILI9481_CMDACCPRO, 0x00);
  sendCmdByte(0xE4, 0xA0);  // undocummented
  sendCmdByte(0xF0, 0x08);  // undocummented
  sendCmdData(0xF3, (uint8_t*)"\x40\x0A", 2);  // undocummented
  sendCmdByte(0xF6, 0x84);  // undocummented
  sendCmdByte(0xF7, 0x80);  // undocummented
  sendCmdData(ILI9481_FMEMACIN, (uint8_t*)"\x00\x01\x06\x30", 4);
  sendCmdByte(ILI9481_DMFMWSET, 0x00);
//  sendCmdData(0x0C, (uint8_t*)"\x00\x55", 2);                   ???
  sendCmdByte(TFT_MADCTL, 0x0A);
  sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
  //sendCmd(TFT_INVON);
  sendCmd(TFT_DISPON);
  delay(25);
