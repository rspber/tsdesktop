/*
  ILI9xxx rotation
*/

  switch (r % 4) {
  case 0:
    sendCmdByte(TFT_MADCTL, (MAD_MX ^ REV) | MAD_BGR);
    break;
  case 1:
    sendCmdByte(TFT_MADCTL, (MAD_YX ^ REV) | MAD_BGR);
    break;
  case 2:
    sendCmdByte(TFT_MADCTL, (MAD_MY ^ REV) | MAD_BGR);
    break;
  case 3:
    sendCmdByte(TFT_MADCTL, ((MAD_YX | MAD_MX | MAD_MY) ^ REV) | MAD_BGR);
    break;
  }
