/*
  SSD196x rotation
*/

  switch (r % 4) {
    case 0:
      sendCmdByte(TFT_MADCTL, ((MAD_YX | MAD_GS) ^ REV) | MAD_BGR);
    break;
    case 1:
      sendCmdByte(TFT_MADCTL, (0x00 ^ REV) | MAD_BGR);
    break;
    case 2:
      sendCmdByte(TFT_MADCTL, ((MAD_YX | MAD_SS) ^ REV) | MAD_BGR);
    break;
    case 3:
      sendCmdByte(TFT_MADCTL, ((MAD_SS | MAD_GS) ^ REV) | MAD_BGR);
    break;
  }
