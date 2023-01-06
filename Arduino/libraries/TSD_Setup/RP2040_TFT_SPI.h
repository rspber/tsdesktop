/*
  RP2040 TFT SPI for Arduino

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_Class.h>
#include <SPI.h>

class RP2040_TFT_SPI : public TFT_Class {
public:
  RP2040_TFT_SPI(const int16_t w, const int16_t h)
   : TFT_Class(w, h) {}

  void begin();

protected:
  void cs(const uint8_t mode);
  void dc(const uint8_t mode);
  void beginTransaction(const uint Hz);
  void beginTransaction();
  void endTransaction();

  void send(const uint8_t b);
  void endSending();

  const uint8_t transfer(const uint8_t cmd);
  const uint16_t transfer16(const uint8_t cmd);

protected:
  int16_t CS, DC;
  SPIClass* _spi;
  uint8_t spiMode = SPI_MODE0;
  SPISettings settings;
};
