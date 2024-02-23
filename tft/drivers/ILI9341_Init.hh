/*
  ILI9341 Init
*/

  sendCmd(TFT_SWRESET); // Engage software reset
  delay(150);
  sendCmdData(ILI9341_EF, (uint8_t*)"\x03\x80\x02", 3);                // unknown
  sendCmdData(ILI9341_PWCTRB, (uint8_t*)"\x00\xC1\x30", 3);            // retired in v.1.02
  sendCmdData(ILI9341_POSC, (uint8_t*)"\x64\x03\x12\x81", 4);          // retired in v.1.02
  sendCmdData(ILI9341_DTCA, (uint8_t*)"\x85\x00\x78", 3);              // retired in v.1.02
  sendCmdData(ILI9341_PWCTRA, (uint8_t*)"\x39\x2C\x00\x34\x02", 5);    // retired in v.1.02
  sendCmdByte(ILI9341_PUMPRC,   0x20);
  sendCmdData(ILI9341_DTCB, (uint8_t*)"\x00\x00", 2);                  // retired in v.1.02
  sendCmdByte(ILI9341_PWCTR1, 0x23);
  sendCmdByte(ILI9341_PWCTR2,   0x10);
  sendCmdData(ILI9341_VCOMCTR1, (uint8_t*)"\x31\x3c", 2);
  sendCmdByte(ILI9341_VCOMCTR2, 0xC0);
  sendCmdByte(TFT_MADCTL,   0x48);
  sendCmdByte(TFT_VSCRSADD, 0x00);
  sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
  sendCmdData(ILI9341_FRMCTR1, (uint8_t*)"\x00\x18", 2);   // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
  sendCmdData(ILI9341_DFUNCTR, (uint8_t*)"\x08\x82\x27", 3);
  sendCmdByte(ILI9341_ENABLE3G, 0x00);                                 // retired in v.1.02
  sendCmdByte(ILI9341_GAMMASET, 0x01);
                                        //"\x0F\x31\x2B\x0C\x0E\x08\x4E\xF1\x37\x07\x10\x03\x0E\x09\x00" // original
  sendCmdData(ILI9341_PVGAMMA, (uint8_t*)"\x0F\x31\x2B\x0C\x0E\x08\x4E\x21\x26\x07\x10\x03\x0E\x09\x00", 15);
                                        //"\x00\x0E\x14\x03\x11\x07\x31\xC1\x48\x08\x0F\x0C\x31\x36\x0F" // original
  sendCmdData(ILI9341_NVGAMMA, (uint8_t*)"\x00\x0E\x14\x03\x11\x07\x31\xC1\x48\x08\x0F\x0C\x31\x36\x0F", 15);
  sendCmd(TFT_SLPOUT);
  delay(150);
  sendCmd(TFT_DISPON);
  delay(150);
