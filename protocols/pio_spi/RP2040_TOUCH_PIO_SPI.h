/*
  RP2040 TOUCH PIO_SPI READ

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#if defined(TFT_PIO_SPI_WRITE) && defined(TOUCH_PIO_SPI)

  #include <stdint.h>

  void touch_begin();

  void touch_startReading();
  void touch_endReading();
  const uint16_t touch_transfer16(const uint8_t cmd);

#endif
