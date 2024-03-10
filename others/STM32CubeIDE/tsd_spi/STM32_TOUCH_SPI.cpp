/*
  STM32 TOUCH SPI READ for Arduino

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#ifdef TOUCH_SPI

#include "SPI.h"

extern SPIClass* stm32_SPI;

  SPISettings settings_touch;

void stm32_touch_spi_initBus()
{
  if (TOUCH_SPI_CS >= 0) {
    pinMode(TOUCH_SPI_CS, OUTPUT);
    digitalWrite(TOUCH_SPI_CS, HIGH);
  }
  settings_touch = SPISettings(TOUCH_SPI_SPEED, MSBFIRST, SPI_MODE0);
}

void touch_begin()
{
  stm32_touch_spi_initBus();
}

void touch_startReading()
{
  stm32_SPI->beginTransaction(settings_touch);
  TOUCH_SPI_CS_L;
}

void touch_endReading()
{
  TOUCH_SPI_CS_H;
  stm32_SPI->endTransaction();
}

const uint8_t touch_transfer(const uint8_t cmd)
{
  return stm32_SPI->transfer(cmd);
}

const uint16_t touch_transfer16(const uint8_t cmd)
{
  return stm32_SPI->transfer16(cmd);
}

#endif
