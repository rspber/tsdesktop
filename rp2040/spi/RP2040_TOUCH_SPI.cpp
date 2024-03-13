/*
  RP2040 TOUCH SPI READ for pico-sdk

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#ifdef TOUCH_SPI

#include <RP2040_TOUCH_SPI.h>

#include "hardware/gpio.h"
#include <rp2040_spi.h>
#include <api/Common.h>

#ifdef TFT_PIO_SPI_WRITE
  void rp2040_pio_spi_enable(bool state);
#endif

#if defined(TOUCH_SPI_CS) && (TOUCH_SPI_CS >= 0)
  #define TOUCH_SPI_CS_L gpio_put(TOUCH_SPI_CS, 0)
  #define TOUCH_SPI_CS_H gpio_put(TOUCH_SPI_CS, 1)
#else
  #define TOUCH_SPI_CS_L
  #define TOUCH_SPI_CS_H
#endif

  spi_inst_t* rp2040_touch_spi = 0;

extern void initPin(const int16_t pin, PinMode mode);

void rp2040_touch_spi_initBus()
{
  initPin(TOUCH_SPI_CS, OUTPUT);
  setup_spi(TFT_SPI_MISO, TFT_SPI_CLK, TFT_SPI_MOSI, 0);
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
  if (!rp2040_touch_spi) {
    rp2040_touch_spi = init_spi(0, TOUCH_SPI_SPEED);
  }
  set_spi_speed(rp2040_touch_spi, TOUCH_SPI_SPEED);
  TOUCH_SPI_CS_L;
}

void touch_endReading()
{
  TOUCH_SPI_CS_H;
#ifdef TFT_PIO_SPI_WRITE
  deinit_spi(rp2040_touch_spi);
  rp2040_touch_spi = 0;
  rp2040_pio_spi_enable(true);
#endif
}

const uint8_t touch_transfer(const uint8_t cmd)
{
  uint8_t data;
  spi_read_blocking(rp2040_touch_spi, cmd, &data, 1);
  return data;
}

const uint16_t touch_transfer16(const uint8_t cmd)
{
  uint8_t data[2];
  spi_read_blocking(rp2040_touch_spi, cmd, data, 2);
  return ((int)data[0] << 8) | data[1];
}

#endif
