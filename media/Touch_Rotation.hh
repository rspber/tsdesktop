/*
  Touch rotation
*/

uint8_t touch_9xxx(const uint8_t rotation)
{
  switch (rotation % 4) {
  case 0:
    return TFT_MAD_YX | TFT_MAD_MX | TFT_MAD_MY;
  case 1:
    return TFT_MAD_MX;
  case 2:
    return TFT_MAD_YX;
  case 3:
    return TFT_MAD_MY;
  }
  return 0;
}

uint8_t touch_9488_1(const uint8_t rotation)
{
  switch (rotation % 4) {
  case 0:
    return TFT_MAD_MY;
  case 1:
    return TFT_MAD_YX | TFT_MAD_MX | TFT_MAD_MY;
  case 2:
    return TFT_MAD_MX;
  case 3:
    return TFT_MAD_YX;
  }
  return 0;
}

uint8_t touch_7789(const uint8_t rotation)
{
  switch (rotation % 4) {
  case 0:
    return TFT_MAD_YX | TFT_MAD_MX;
  case 1:
    return 0;
  case 2:
    return TFT_MAD_YX | TFT_MAD_MY;
  case 3:
    return TFT_MAD_MX | TFT_MAD_MY;
  }
  return 0;
}

uint8_t touch_7796(const uint8_t rotation)
{
    switch (rotation % 4) {
        case 0:
            return TFT_MAD_YX | TFT_MAD_MY;
        case 1:
            return TFT_MAD_MY | TFT_MAD_MX;
        case 2:
            return TFT_MAD_YX | TFT_MAD_MX;
        case 3:
            return 0;
    }
    return 0;
}
