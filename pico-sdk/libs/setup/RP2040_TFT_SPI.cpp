/*
  RP2040 TFT SPI for pico-sdk

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include "RP2040_TFT_SPI.h"
#include <Setup.h>

void RP2040_TFT_SPI::begin()
{
  CS = TFT_CS;
  if (CS >= 0) {
    pinMode(CS, OUTPUT);
  }

  DC = SPI0_DC;
  if (DC >= 0) {
    pinMode(DC, OUTPUT);
  }

  spi = spi0;
  spi_speed = TFT_SPI_WRITE_SPEED;

  RST = TFT_RST;
  TFT_Class::begin();
}

void RP2040_TFT_SPI::cs(const uint8_t mode)
{
  if (CS >= 0) {
    cs_select(CS, mode);
  }
}

void RP2040_TFT_SPI::dc(const uint8_t mode)
{
  if (DC >= 0) {
    gpio_put(DC, mode);
  }
}

void RP2040_TFT_SPI::beginTransaction(const uint Hz)
{
  set_spi_speed(spi, Hz);
  cs(0);
}

void RP2040_TFT_SPI::beginTransaction()
{
  beginTransaction(spi_speed);
}

void RP2040_TFT_SPI::endTransaction()
{
  cs(1);
}

void RP2040_TFT_SPI::send(const uint8_t b)
{
  while (!spi_is_writable(spi)) tight_loop_contents();
  spi_get_hw(spi)->dr = (uint32_t)b;
}

void RP2040_TFT_SPI::endSending()
{
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
  uint8_t data;
  spi_read_blocking(spi, cmd, &data, 1);
  return data;
}

const uint16_t RP2040_TFT_SPI::transfer16(const uint8_t cmd)
{
  uint8_t data[2];
  spi_read_blocking(spi, cmd, data, 2);
  return (data[0] << 8) | data[1];
}

