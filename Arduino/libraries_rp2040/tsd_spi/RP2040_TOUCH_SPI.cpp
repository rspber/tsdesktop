/*
  RP2040 TOUCH SPI READ for Arduino

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#ifdef TOUCH_SPI

#include <RP2040_TOUCH_SPI.h>
#include "SPI.h"
#include <api/Common.h>

#ifdef TFT_PIO_SPI_WRITE
  void rp2040_pio_spi_enable(bool state);
#endif

#if defined(TOUCH_SPI_CS) && (TOUCH_SPI_CS >= 0)
  #define TOUCH_SPI_CS_L digitalWrite(TOUCH_SPI_CS, 0)
  #define TOUCH_SPI_CS_H digitalWrite(TOUCH_SPI_CS, 1)
#else
  #define TOUCH_SPI_CS_L
  #define TOUCH_SPI_CS_H
#endif

  SPIClass* rp2040_touch_SPI = 0;
  SPISettings settings_touch;

extern void initPin(const int16_t pin, PinMode mode);

void rp2040_touch_spi_initBus()
{
  initPin(TOUCH_SPI_CS, OUTPUT);
  settings_touch = SPISettings(TOUCH_SPI_SPEED, MSBFIRST, SPI_MODE0);
}

void touch_begin()
{
  rp2040_touch_spi_initBus();
}

void touch_startReading()
{
#ifdef TFT_PIO_SPI_WRITE
  rp2040_pio_spi_enable(false);
#endif
  if (!rp2040_touch_SPI) {
    rp2040_touch_SPI = &SPI;
    rp2040_touch_SPI->begin();
  }
  rp2040_touch_SPI->beginTransaction(settings_touch);
  TOUCH_SPI_CS_L;
}

void touch_endReading()
{
  TOUCH_SPI_CS_H;
  rp2040_touch_SPI->endTransaction();
#ifdef TFT_PIO_SPI_WRITE
  rp2040_touch_SPI->end();
  rp2040_touch_SPI = 0;
  rp2040_pio_spi_enable(true);
#endif
}

const uint8_t touch_transfer(const uint8_t cmd)
{
  return rp2040_touch_SPI->transfer(cmd);
}

const uint16_t touch_transfer16(const uint8_t cmd)
{
  return rp2040_touch_SPI->transfer16(cmd);
}

#endif
