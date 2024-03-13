/*
  ESP32 TOUCH SPI READ for Arduino

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#ifdef TOUCH_SPI

#include <ESP32_TOUCH_SPI.h>
#include "SPI.h"
#include <Arduino.h>

#include <ESP32_TFT_SPI.h>

#if defined(TOUCH_SPI_CS)
  #if (TOUCH_SPI_CS >= 32)
    #define TOUCH_SPI_CS_L    GPIO_32_PIN_LOW(TOUCH_SPI_CS);   GPIO_32_PIN_LOW(TOUCH_SPI_CS)
    #define TOUCH_SPI_CS_H    GPIO_32_PIN_HIGH(TOUCH_SPI_CS)
  #elif (TOUCH_SPI_CS >= 0)
    #define TOUCH_SPI_CS_L    GPIO_00_PIN_LOW(TOUCH_SPI_CS);   GPIO_00_PIN_LOW(TOUCH_SPI_CS)
    #define TOUCH_SPI_CS_H    GPIO_00_PIN_HIGH(TOUCH_SPI_CS)
  #else
    #define TOUCH_SPI_CS_L
    #define TOUCH_SPI_CS_H
  #endif
#else
  #define TOUCH_SPI_CS_L
  #define TOUCH_SPI_CS_H
#endif

extern SPIClass* esp32_SPI;

  SPISettings settings_touch;

extern void initPin(const int16_t pin, PinMode mode);

void esp32_touch_spi_initBus()
{
  initPin(TOUCH_SPI_CS, OUTPUT);
  settings_touch = SPISettings(TOUCH_SPI_SPEED, MSBFIRST, SPI_MODE0);
}

void touch_begin()
{
  esp32_touch_spi_initBus();
}

void touch_startReading()
{
  esp32_SPI->beginTransaction(settings_touch);
  TOUCH_SPI_CS_L;
}

void touch_endReading()
{
  TOUCH_SPI_CS_H;
  esp32_SPI->endTransaction();
}

const uint8_t touch_transfer(const uint8_t cmd)
{
  return esp32_SPI->transfer(cmd);
}

const uint16_t touch_transfer16(const uint8_t cmd)
{
  return esp32_SPI->transfer16(cmd);
}

#endif
