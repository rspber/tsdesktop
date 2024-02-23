/*
  RM68140 init
*/

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
