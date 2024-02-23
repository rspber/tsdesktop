/*
  HX8357D init
*/

  sendCmdData(HX8357D_SETC, (uint8_t*)"\xFF\x83\x57", 3);
  delay(300);
  sendCmdData(HX8357_SETRGB, (uint8_t*)"\x80\x00\x06\x06", 4);
  //0x80 enable SDO pin!  -  0x00 disable SDO pin!
  sendCmdByte(HX8357D_SETCOM, 0x25);  // -1.52V
  sendCmdByte(HX8357_SETOSC, 0x68);  // Normal mode 70Hz, Idle mode 55 Hz
  sendCmdByte(HX8357_SETPANEL, 0x05);  // BGR, Gate direction swapped
  sendCmdData(HX8357_SETPWR1, (uint8_t*)"\x00\x15\x1C\x1C\x83\xAA", 6);
    // 0x00 Not deep standby
    // 0x15 BT
    // 0x1C VSPR
    // 0x1C VSNR
    // 0x83 AP
    // 0xAA FS
  sendCmdData(HX8357D_SETSTBA, (uint8_t*)"\x50\x50\x01\x3C\x1E\x08", 6);
    // 0x50 OPON normal
    // 0x50 OPON idle
    // 0x01 STBA
    // 0x3C STBA
    // 0x1E STBA
    // 0x08 GEN
  sendCmdData(HX8357D_SETCYC, (uint8_t*)"\x02\x40\x00\x2A\x2A\x0D\x78", 7);
    // 0x02 NW 0x02
    // 0x40 RTN
    // 0x00 DIV
    // 0x2A DUM
    // 0x2A DUM
    // 0x0D GDON
    // 0x78 GDOFF
  sendCmdData(HX8357D_SETGAMMA, (uint8_t*)"\x02\x0A\x11\x1d\x23\x35\x41\x4b\x4b\x42\x3A\x27\x1B\x08\x09\x03\x02\x0A\x11\x1d\x23\x35\x41\x4b\x4b\x42\x3A\x27\x1B\x08\x09\x03\x00\x01", 34);
  sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
  sendCmdByte(TFT_MADCTL, MAD_MX | MAD_MY | MAD_BGR);
  sendCmdByte(TFT_TEARON, 0x00);
  sendCmdData(TFT_TEARLINE, (uint8_t*)"\x00\x02", 2);
  sendCmd(TFT_SLPOUT);
  delay(150);
  sendCmd(TFT_DISPON);  // display on
  delay(50);
