/*
  STM32 TOUCH SPI READ for Arduino

  Copyright (c) 2024, rspber (https://github.com/rspber)

  see STM32_TFT_SPI.cpp for more

*/

#include <Setup.h>

#ifdef TOUCH_SPI

#include "SPI.h"

////////////////////////////////////////////////////////////////////////////////////////
// Define the touch screen chip select pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#if defined (TOUCH_SPI_CS) && (TOUCH_SPI_CS >= 0)
  // Speed is not important for this signal
  #define TOUCH_SPI_CS_L digitalWrite(TOUCH_SPI_CS, LOW)
  #define TOUCH_SPI_CS_H digitalWrite(TOUCH_SPI_CS, HIGH)
#else
  #define TOUCH_SPI_CS_L // No macro allocated so it generates no code
  #define TOUCH_SPI_CS_H // No macro allocated so it generates no code
#endif

extern SPIClass& stm32_SPI;

  SPISettings settings_touch;

extern void initPin(const int16_t pin, PinMode mode);

void stm32_touch_spi_initBus()
{
  initPin(TOUCH_SPI_CS, OUTPUT);
  settings_touch = SPISettings(TOUCH_SPI_SPEED, MSBFIRST, SPI_MODE0);
}

void touch_begin()
{
  stm32_touch_spi_initBus();
}

void touch_startReading()
{
  stm32_SPI.beginTransaction(settings_touch);
  TOUCH_SPI_CS_L;
}

void touch_endReading()
{
  TOUCH_SPI_CS_H;
  stm32_SPI.endTransaction();
}

const uint8_t touch_transfer(const uint8_t cmd)
{
  return stm32_SPI.transfer(cmd);
}

const uint16_t touch_transfer16(const uint8_t cmd)
{
  return stm32_SPI.transfer16(cmd);
}

#endif
