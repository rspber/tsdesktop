/*
  RM68140 rotation
*/

  uint8_t T[] {0x00, 0x02, 0x3B};
  switch (r % 4) {
    case 0: // Portrait
      sendCmdByte(TFT_MADCTL, (0x00 ^ REV) | TFT_MAD_BGR);
      T[1] |= ((TFT_MAD_SS ^ REV) << 4);
    break;
    case 1: // Landscape (Portrait + 90)
      sendCmdByte(TFT_MADCTL, (TFT_MAD_YX ^ REV) | TFT_MAD_BGR);
      T[1] |= 0x00;
    break;
    case 2: // Inverter portrait
      sendCmdByte(TFT_MADCTL, (0x00 ^ REV) | TFT_MAD_BGR);
      T[1] |= ((TFT_MAD_GS ^ REV) << 6);
    break;
    case 3: // Inverted landscape
      sendCmdByte(TFT_MADCTL, (TFT_MAD_YX ^ REV) | TFT_MAD_BGR);
      T[1] |= ((TFT_MAD_SS ^ REV) << 4) | ((TFT_MAD_GS ^ REV) << 6);
    break;
  }
  sendCmdData(0xB6, T, 3);
