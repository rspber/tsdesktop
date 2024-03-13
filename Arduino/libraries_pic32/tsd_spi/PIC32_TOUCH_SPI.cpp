/*
  PIC32 TOUCH SPI READ for Arduino

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#ifdef TOUCH_SPI

#include <PIC32_TOUCH_SPI.h>
#include "SPI.h"
#include <Arduino.h>

#if defined(TOUCH_SPI_CS) && (TOUCH_SPI_CS >= 0)
  #define TOUCH_SPI_CS_L digitalWrite(TOUCH_SPI_CS, 0)
  #define TOUCH_SPI_CS_H digitalWrite(TOUCH_SPI_CS, 1)
#else
  #define TOUCH_SPI_CS_L
  #define TOUCH_SPI_CS_H
#endif

  SPIClass* pic32_touch_SPI = 0;
  SPISettings settings_touch;

extern void initPin(const int16_t pin, PinMode mode);

void pic32_touch_spi_initBus()
{
  initPin(TOUCH_SPI_CS, OUTPUT);
  settings_touch = SPISettings(TOUCH_SPI_SPEED, MSBFIRST, SPI_MODE0);
}

void touch_begin()
{
  pic32_touch_spi_initBus();
}

void touch_startReading()
{
  if (!pic32_touch_SPI) {
    pic32_touch_SPI = &SPI;
    pic32_touch_SPI->begin();
  }
  pic32_touch_SPI->beginTransaction(settings_touch);
  TOUCH_SPI_CS_L;
}

void touch_endReading()
{
  TOUCH_SPI_CS_H;
  pic32_touch_SPI->endTransaction();
}

const uint8_t touch_transfer(const uint8_t cmd)
{
  return pic32_touch_SPI->transfer(cmd);
}

const uint16_t touch_transfer16(const uint8_t cmd)
{
  return pic32_touch_SPI->transfer16(cmd);
}

#endif
