/*
  RP2040 TFT SPI for Arduino

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include "RP2040_TFT_SPI.h"
#include "Setup.h"
#include "hardware/spi.h"

void RP2040_TFT_SPI::begin()
{
  CS = TFT_CS;
  if (CS >= 0) {
    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);
  }

  DC = TFT_DC;
  if (DC >= 0) {
    pinMode(DC, OUTPUT);
    digitalWrite(DC, HIGH);
  }

  _spi = &SPI;
  settings = SPISettings(TFT_WRITE_SPEED, MSBFIRST, spiMode);

  _spi->begin();

  RST = TFT_RST;
  TFT_Class::begin();
}

void RP2040_TFT_SPI::cs(const uint8_t mode)
{
  if (CS >= 0) {
    digitalWrite(CS, mode);
  }
}

void RP2040_TFT_SPI::dc(const uint8_t mode)
{
  if (DC >= 0) {
    digitalWrite(DC, mode);
  }
}

void RP2040_TFT_SPI::beginTransaction(const uint Hz)
{
  SPISettings tmp(Hz, MSBFIRST, spiMode);
  _spi->beginTransaction(tmp);
  cs(0);
}

void RP2040_TFT_SPI::beginTransaction()
{
  _spi->beginTransaction(settings);
  cs(0);
}

void RP2040_TFT_SPI::endTransaction()
{
  _spi->endTransaction();
  cs(1);
}

void RP2040_TFT_SPI::send(const uint8_t b)
{
  spi_inst_t* spi = _spi == &SPI ? spi0 : spi1;

  while (!spi_is_writable(spi)) tight_loop_contents();
  spi_get_hw(spi)->dr = (uint32_t)b;
}

void RP2040_TFT_SPI::endSending()
{
  spi_inst_t* spi = _spi == &SPI ? spi0 : spi1;

  // Drain RX FIFO, then wait for shifting to finish (which may be *after*
  // TX FIFO drains), then drain RX FIFO again
  while (spi_is_readable(spi)) (void)spi_get_hw(spi)->dr;
  while (spi_is_busy(spi)) tight_loop_contents();
  while (spi_is_readable(spi)) (void)spi_get_hw(spi)->dr;

  // Don't leave overrun flag set
  spi_get_hw(spi)->icr = SPI_SSPICR_RORIC_BITS;
}

const uint8_t RP2040_TFT_SPI::transfer(const uint8_t cmd)
{
  return _spi->transfer(cmd);
}

const uint16_t RP2040_TFT_SPI::transfer16(const uint8_t cmd)
{
  return _spi->transfer16(cmd);
}
