/*
  ILI9xxx rotation
*/

  switch (r % 4) {
  case 0:
    sendCmdByte(TFT_MADCTL, (TFT_MAD_MX ^ REV) | TFT_MAD_BGR);
    break;
  case 1:
    sendCmdByte(TFT_MADCTL, (TFT_MAD_YX ^ REV) | TFT_MAD_BGR);
    break;
  case 2:
    sendCmdByte(TFT_MADCTL, (TFT_MAD_MY ^ REV) | TFT_MAD_BGR);
    break;
  case 3:
    sendCmdByte(TFT_MADCTL, ((TFT_MAD_YX | TFT_MAD_MX | TFT_MAD_MY) ^ REV) | TFT_MAD_BGR);
    break;
  }
