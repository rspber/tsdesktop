/*
  RP2040 RESISTIVE TOUCH (mostly pinned into 8BITP pins)

  Copyright (c) 2023, rspber (https://github.com/rspber)

  Based on:
  https://github.com/adafruit/Adafruit_TouchScreen/TouchScreen.h

  License in file LimorFried_license.txt

  Originally notes below:
*/

/*
// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License
*/

#pragma once

#include <Setup.h>
#include <stdint.h>

#if defined(TOUCH_RESISTIVE)

class TSD_SPFD5408 {    // or 5402 maybe
public:
  TSD_SPFD5408() {}

  void begin();
  bool IamHere(int16_t& x, int16_t& y);
};

#endif
