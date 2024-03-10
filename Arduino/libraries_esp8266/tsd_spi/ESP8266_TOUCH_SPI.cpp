/*
  ESP8266 TOUCH SPI READ for Arduino

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#ifdef TOUCH_SPI

#include <ESP8266_TOUCH_SPI.h>
#include "SPI.h"
#include <Arduino.h>

#define spi SPI

#if defined(TOUCH_SPI_CS) && (TOUCH_SPI_CS >= 0)
  #define TOUCH_SPI_CS_L digitalWrite(TOUCH_SPI_CS, 0)
  #define TOUCH_SPI_CS_H digitalWrite(TOUCH_SPI_CS, 1)
#else
  #define TOUCH_SPI_CS_L
  #define TOUCH_SPI_CS_H
#endif

  SPISettings settings_touch;

void esp8266_touch_spi_initBus()
{
  if (TOUCH_SPI_CS >= 0) {
    pinMode(TOUCH_SPI_CS, OUTPUT);
    digitalWrite(TOUCH_SPI_CS, HIGH);
  }
  settings_touch = SPISettings(TOUCH_SPI_SPEED, MSBFIRST, SPI_MODE0);
}

void touch_begin()
{
  esp8266_touch_spi_initBus();
}

void touch_startReading()
{
  spi.beginTransaction(settings_touch);
  TOUCH_SPI_CS_L;
}

void touch_endReading()
{
  TOUCH_SPI_CS_H;
  spi.endTransaction();
}

const uint8_t touch_transfer(const uint8_t cmd)
{
  return spi.transfer(cmd);
}

const uint16_t touch_transfer16(const uint8_t cmd)
{
  return spi.transfer16(cmd);
}

#endif