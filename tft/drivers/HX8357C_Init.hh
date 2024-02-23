/*
  HX8357C init
*/

  sendCmdData(HX8357D_SETC, (uint8_t*)"\xFF\x83\x57", 3);
  delay(50);
  sendCmdByte(HX8357D_SETCOM, 0x2C); //0x52 for HSD 3.0"
  sendCmd(TFT_SLPOUT);
  delay(200);
  sendCmdByte(TFT_TEARON, 0x00);
  sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
  //sendCmdByte(HX8357_SETPANEL, 0x09); // S960>S1, G1>G480, R-G-B, normally black
  //sendCmdData(HX8357_SETRGB, (uint8_t*)"\x43\x00\x06\x06", 4);
  sendCmdData(HX8357_SETPWR1, (uint8_t*)"\x00\x15\x0D\x0D\x83\x48", 6);
  sendCmdData(HX8357D_SETSTBA, (uint8_t*)"\x24\x24\x01\x3C\xC8\x08", 6);
  sendCmdData(HX8357D_SETCYC, (uint8_t*)"\x02\x40\x00\x2A\x2A\x0D\x4F", 7);
  sendCmdData(HX8357D_SETGAMMA, (uint8_t*)"\x00\x15\x1D\x2A\x31\x42\x4C\x53\x45\x40\x3B\x32\x2E\x28\x24\x03\x00\x15\x1D\x2A\x31\x42\x4C\x53\x45\x40\x3B\x32\x2E\x28\x24\x03\x00\x01", 34);
  sendCmdByte(TFT_MADCTL, 0x48);
  delay(20);
  sendCmd(TFT_INVON);
  delay(20);
  sendCmd(TFT_DISPON);
  delay(120);
