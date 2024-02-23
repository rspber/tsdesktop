/*
  ILI9481 rotation
*/

  switch (r % 4) {
  case 0: // Portrait
    sendCmdByte(TFT_MADCTL, (MAD_SS ^ REV) | MAD_BGR);
    break;
  case 1: // Landscape (Portrait + 90)
    sendCmdByte(TFT_MADCTL, (MAD_YX ^ REV) | MAD_BGR);
    break;
  case 2: // Inverter portrait
    sendCmdByte(TFT_MADCTL, (MAD_GS ^ REV) | MAD_BGR);
    break;
  case 3: // Inverted landscape
    sendCmdByte(TFT_MADCTL, ((MAD_SS | MAD_GS | MAD_YX) ^ REV) | MAD_BGR);
    break;
  }
