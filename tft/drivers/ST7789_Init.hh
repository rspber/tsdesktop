/*
  ST7789 init
*/

  sendCmd(TFT_SWRESET);
  delay(150);
  sendCmd(TFT_SLPOUT);
  delay(120);
  sendCmd(TFT_NORON);
  sendCmdByte(TFT_MADCTL, MAD_BGR);
  sendCmdData(0xB6, (uint8_t*)"\x0A\x82", 2);
  sendCmdData(ST7789_RAMCTRL, (uint8_t*)"\x00\xE0", 2);
  sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
  delay(10);
  sendCmdData(ST7789_PORCTRL, (uint8_t*)"\x0c\x0c\x00\x33\x33", 5);
  sendCmdByte(ST7789_GCTRL, 0x35);
  sendCmdByte(ST7789_VCOMS, 0x28);
  sendCmdByte(ST7789_LCMCTRL, 0x04);  // this is right !
  sendCmdData(ST7789_VDVVRHEN, (uint8_t*)"\x01\xFF", 2);
  sendCmdByte(ST7789_VRHS, 0x10);
  sendCmdByte(ST7789_VDVSET, 0x20);
  sendCmdByte(ST7789_FRCTR2, 0x0F);
  sendCmdData(ST7789_PWCTRL1, (uint8_t*)"\xa4\xa1", 2);
  sendCmdData(ST7789_PVGAMCTRL, (uint8_t*)"\xd0\x00\x02\x07\x0a\x28\x32\x44\x42\x06\x0e\x12\x14\x17", 14);
  sendCmdData(ST7789_NVGAMCTRL, (uint8_t*)"\xd0\x00\x02\x07\x0a\x28\x31\x54\x47\x0e\x1c\x17\x1b\x1e", 14);
//  sendCmd(TFT_INVON);
  sendCmd(TFT_DISPON);
  delay(120);
