/*
  Touch rotation
*/

uint8_t touch_9xxx(const uint8_t rotation)
{
  switch (rotation % 4) {
  case 0:
    return MAD_YX | MAD_MX | MAD_MY;
  case 1:
    return MAD_MX;
  case 2:
    return MAD_YX;
  case 3:
    return MAD_MY;
  }
  return 0;
}

uint8_t touch_9488_1(const uint8_t rotation)
{
  switch (rotation % 4) {
  case 0:
    return MAD_MY;
  case 1:
    return MAD_YX | MAD_MX | MAD_MY;
  case 2:
    return MAD_MX;
  case 3:
    return MAD_YX;
  }
  return 0;
}

uint8_t touch_7789(const uint8_t rotation)
{
  switch (rotation % 4) {
  case 0:
    return MAD_YX | MAD_MX;
  case 1:
    return 0;
  case 2:
    return MAD_YX | MAD_MY;
  case 3:
    return MAD_MX | MAD_MY;
  }
  return 0;
}

uint8_t touch_7796(const uint8_t rotation)
{
    switch (rotation % 4) {
        case 0:
            return MAD_YX | MAD_MY;
        case 1:
            return MAD_MY | MAD_MX;
        case 2:
            return MAD_YX | MAD_MX;
        case 3:
            return 0;
    }
    return 0;
}
