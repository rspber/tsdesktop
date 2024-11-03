/*
  R61581 rotation
*/

  switch (r % 4) {
    case 0: // Portrait
      sendCmdByte(TFT_MADCTL, (TFT_MAD_MX ^ REV) | TFT_MAD_BGR);
    break;
    case 1: // Landscape (Portrait + 90)
      sendCmdByte(TFT_MADCTL, (TFT_MAD_YX ^ REV) | TFT_MAD_BGR);
    break;
    case 2: // Inverter portrait
      sendCmdByte(TFT_MADCTL, (TFT_MAD_GS ^ REV) | TFT_MAD_BGR);
    break;
    case 3: // Inverted landscape
      sendCmdByte(TFT_MADCTL, ((TFT_MAD_YX | TFT_MAD_MX | TFT_MAD_GS) ^ REV) | TFT_MAD_BGR);
    break;
  }
