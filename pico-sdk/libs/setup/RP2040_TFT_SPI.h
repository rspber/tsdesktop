/*
  RP2040 TFT SPI for pico-sdk

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_SPI.h>
#include "hardware/spi.h"

class RP2040_TFT_SPI : public TFT_SPI {
public:
  RP2040_TFT_SPI(const int16_t aCS, const int16_t aDC, const uint Hz, spi_inst_t* aspi = spi0);

  void begin();

  void cs(const uint8_t mode);
  void dc(const uint8_t mode);
  void spiBegin(const uint Hz);
  void spiBegin();
  void spiEnd();

  void send(const uint8_t b);
  void endSending();

  const uint8_t transfer(const uint8_t cmd);
  const uint16_t transfer16(const uint8_t cmd);

protected:
  int16_t CS, DC;
  spi_inst_t* spi;
  uint spi_speed;
};
