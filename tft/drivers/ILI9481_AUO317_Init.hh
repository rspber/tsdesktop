/*
  ILI9481_AUO317 init
*/

  sendCmd(TFT_SLPOUT);
  delay(20);
  sendCmdData(ILI9481_POWERSET, (uint8_t*)"\x07\x40\x1D", 3);
  sendCmdData(ILI9481_VCOMCTL, (uint8_t*)"\x00\x18\x13", 3);
  sendCmdData(ILI9481_POWNORMD, (uint8_t*)"\x01\x11", 2);
  sendCmdData(ILI9481_PANELDRV, (uint8_t*)"\x10\x3B\x00\x02\x11", 5);
  sendCmdByte(ILI9481_FRAINCTL, 0x03);
  sendCmdData(ILI9481_GAMMASET, (uint8_t*)"\x00\x44\x06\x44\x0A\x08\x17\x33\x77\x44\x08\x0C", 12);
  sendCmdByte(ILI9481_CMDACCPRO, 0x00);
  sendCmdByte(0xE4, 0xA0);  // undocumented
  sendCmdByte(0xF0, 0x01);  // undocumented
  sendCmdByte(TFT_MADCTL, 0x0A);
  sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
  //sendCmd(TFT_INVON);
  sendCmd(TFT_DISPON);
  delay(25);
