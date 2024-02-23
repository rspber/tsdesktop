/*
  ILI9488 init
*/

  sendCmdData(ILI9488_PVGAMMA, (uint8_t*)"\x00\x03\x09\x08\x16\x0A\x3F\x78\x4C\x09\x0A\x08\x16\x1A\x0F", 15);
  sendCmdData(ILI9488_NVGAMMA, (uint8_t*)"\x00\x16\x19\x03\x0F\x05\x32\x45\x46\x04\x0E\x0D\x35\x37\x0F", 15);
  sendCmdData(ILI9488_PWCTR1, (uint8_t*)"\x17\x15", 2);
    // 0x17 Vreg1out
    // 0x15 Verg2out
  sendCmdByte(ILI9488_PWCTR2, 0x41);  // VGH,VGL
  sendCmdData(ILI9488_VCOMCTR1, (uint8_t*)"\x00\x12\x80", 3);
    // 0x00
    // 0x12 Vcom
    // 0x80
  sendCmdByte(TFT_MADCTL, 0x48);

  // NOTE: For 16bit parallel transfer, IM jumpers need to be set
  // as 010.
  sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
  sendCmdByte(ILI9488_INTFMDCTL, 0x00);
  sendCmdByte(ILI9488_FRMCTR1, 0xA0);     // 60Hz
  sendCmdByte(ILI9488_INVCTR, 0x02);     // 2-dot
  sendCmdData(ILI9488_DFUNCTR, (uint8_t*)"\x02\x02\x3B", 3);
    // 0x02 MCU
    // 0x02 Source,Gate scan direction
    // 0x3B
  sendCmdByte(ILI9488_ENTMODSET, 0xC6);
  sendCmdData(ILI9488_ADJCTL3, (uint8_t*)"\xA9\x51\x2C\x82", 4);
    // 0x82 D7 stream, loose
  sendCmd(TFT_SLPOUT);
  delay(120);
  sendCmd(TFT_DISPON);
  delay(25);
