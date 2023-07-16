/*
  RP2040 TOUCH SPI READ for pico-sdk

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#if defined(TOUCH_SPI)

  #include <stdint.h>

  void touch_begin();

  void touch_startReading();
  void touch_endReading();
  const uint8_t touch_transfer(const uint8_t cmd);
  const uint16_t touch_transfer16(const uint8_t cmd);

#endif
