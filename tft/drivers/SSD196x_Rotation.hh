/*
  SSD196x rotation
*/

  switch (r % 4) {
    case 0:
      sendCmdByte(TFT_MADCTL, ((TFT_MAD_YX | TFT_MAD_GS) ^ REV) | TFT_MAD_BGR);
    break;
    case 1:
      sendCmdByte(TFT_MADCTL, (0x00 ^ REV) | TFT_MAD_BGR);
    break;
    case 2:
      sendCmdByte(TFT_MADCTL, ((TFT_MAD_YX | TFT_MAD_SS) ^ REV) | TFT_MAD_BGR);
    break;
    case 3:
      sendCmdByte(TFT_MADCTL, ((TFT_MAD_SS | TFT_MAD_GS) ^ REV) | TFT_MAD_BGR);
    break;
  }
