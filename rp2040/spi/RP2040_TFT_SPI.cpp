/*
  RP2040 TFT SPI for pico-sdk

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#if defined(TFT_SPI_WRITE) || defined(TFT_SPI_READ)

#include "RP2040_TFT_SPI.h"
#include <rp2040_spi.h>
#include "hardware/gpio.h"
#include <api/Common.h>

#define TFT_CASET       0x2A    // Column address set
#define TFT_PASET       0x2B    // Page address set
#define TFT_RAMWR       0x2C    // Memory write
#define TFT_RAMRD       0x2E    // Memory read
#define TFT_IDXRD       0xD9    // undocumented

#if defined(TFT_SPI_CS) && (TFT_SPI_CS >= 0)
  #define SPI_CS_L gpio_put(TFT_SPI_CS, 0)
  #define SPI_CS_H gpio_put(TFT_SPI_CS, 1)
#else
  #define SPI_CS_L
  #define SPI_CS_H
#endif

#if defined(TFT_SPI_DC) && (TFT_SPI_DC >= 0)
  #define SPI_DC_C gpio_put(TFT_SPI_DC, 0)
  #define SPI_DC_D gpio_put(TFT_SPI_DC, 1)
#else
  #define SPI_DC_C
  #define SPI_DC_D
#endif

#define SPI_X rp2040_spi

spi_inst_t* rp2040_spi = 0;

extern void initPin(const int16_t pin, PinMode mode);
extern void tft_hardReset(const int16_t RST);

void rp2040_spi_initBus()
{
#ifdef TOUCH_SPI_CS 
  initPin(TOUCH_SPI_CS, OUTPUT);
#endif
#ifdef TFT_SPI_CS
  initPin(TFT_SPI_CS, OUTPUT);
#endif
#ifdef TFT_SPI_DC
  initPin(TFT_SPI_DC, OUTPUT);
#endif
#ifdef TFT_SPI_RST
  initPin(TFT_SPI_RST, OUTPUT);
  tft_hardReset(TFT_SPI_RST);
#endif

  setup_spi(TFT_SPI_MISO, TFT_SPI_CLK, TFT_SPI_MOSI, 0);
}

inline void spi_send(const uint8_t b)
{
  while (!spi_is_writable(SPI_X)) tight_loop_contents();
  spi_get_hw(SPI_X)->dr = (uint32_t)b;	// TX_FIFO
}

inline void spi_send16(const uint16_t w)
{
  spi_send(w >> 8);
  spi_send(w);
}

inline void spi_endSending()
{
  // Drain RX FIFO, then wait for shifting to finish (which may be *after*
  // TX FIFO drains), then drain RX FIFO again
  while (spi_is_readable(SPI_X)) (void)spi_get_hw(SPI_X)->dr;
  while (spi_is_busy(SPI_X)) tight_loop_contents();
  while (spi_is_readable(SPI_X)) (void)spi_get_hw(SPI_X)->dr;

  // Don't leave overrun flag set
  spi_get_hw(SPI_X)->icr = SPI_SSPICR_RORIC_BITS;
}

#endif



#ifdef TFT_SPI_WRITE

const char* tft_identification() { return "RP2040 SPI"; }

void rp2040_spi_wr_enable(const bool enable)
{
}

void tft_write_begin()
{
  rp2040_spi_initBus();
  rp2040_spi = init_spi(0, TFT_SPI_WRITE_SPEED);
}

void tft_startWriteCmd()
{
  set_spi_speed(rp2040_spi, TFT_SPI_SETUP_SPEED);
  SPI_CS_L;
}

void tft_sendCmd(const uint8_t cmd)
{
  SPI_CS_H;
  SPI_CS_L;
  SPI_DC_C;
  spi_send(cmd);
  spi_endSending();
  SPI_DC_D;
}

void tft_sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size)
{
  SPI_CS_H;
  SPI_CS_L;
  SPI_DC_C;
  spi_send(cmd);
  spi_endSending();
  SPI_DC_D;
  for (int16_t i = 0; i < size; ++i) {
    spi_send(data[i]);
  }
  spi_endSending();
}

void tft_startWrite()
{
  set_spi_speed(rp2040_spi, TFT_SPI_WRITE_SPEED);
  SPI_CS_L;
}

void tft_endWrite()
{
  SPI_CS_H;
}

void tft_writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  SPI_DC_C;
  spi_send(TFT_CASET);
  spi_endSending();
  SPI_DC_D;
  spi_send16(x);
  spi_send16(x + w - 1);
  spi_endSending();
  SPI_DC_C;
  spi_send(TFT_PASET);
  spi_endSending();
  SPI_DC_D;
  spi_send16(y);
  spi_send16(y + h - 1);
  spi_endSending();
  SPI_DC_C;
  spi_send(TFT_RAMWR);
  spi_endSending();
  SPI_DC_D;
}

void tft_sendMDTColor(const mdt_t c)
{
  #if MDT_SIZE > 2
    spi_send(c >> 16);
  #endif
    spi_send(c >> 8);
    spi_send(c);

  spi_endSending();
}

void tft_sendMDTColor(const mdt_t c, int32_t len)
{
  while (--len >= 0) {
  #if MDT_SIZE > 2
    spi_send(c >> 16);
  #endif
    spi_send(c >> 8);
    spi_send(c);
  }
  spi_endSending();
}

void tft_sendMDTBuffer16(const uint8_t* p, int32_t len)
{
  while (--len >= 0) {
    spi_send(*p++);
    spi_send(*p++);
  }
  spi_endSending();
}

void tft_sendMDTBuffer24(const uint8_t* p, int32_t len)
{
  while (--len >= 0) {
    spi_send(*p++);
    spi_send(*p++);
    spi_send(*p++);
  }
  spi_endSending();
}

#endif




#if defined(TFT_SPI_READ)

#ifdef TFT_PIO_SPI_WRITE
  void rp2040_pio_spi_enable(bool state);
#endif

void tft_read_begin()
{
#ifndef TFT_SPI_WRITE
  rp2040_spi_initBus();
#endif
}

void tft_startReading()
{
#ifdef TFT_PIO_SPI_WRITE
  rp2040_pio_spi_enable(false);
#endif
  if (!rp2040_spi) {
    rp2040_spi = init_spi(0, TFT_SPI_READ_SPEED);
  }
  set_spi_speed(rp2040_spi, TFT_SPI_READ_SPEED);
  SPI_CS_L;
}

void tft_endReading()
{
  SPI_CS_H;
#ifdef TFT_PIO_SPI_WRITE
  deinit_spi(rp2040_spi);
  rp2040_spi = 0;
  rp2040_pio_spi_enable(true);
#endif
}

void tft_readAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  SPI_DC_C;
  spi_send(TFT_CASET);
  spi_endSending();
  SPI_DC_D;
  spi_send16(x);
  spi_send16(x + w - 1);
  spi_endSending();
  SPI_DC_C;
  spi_send(TFT_PASET);
  spi_endSending();
  SPI_DC_D;
  spi_send16(y);
  spi_send16(y + h - 1);
  spi_endSending();
  SPI_DC_C;
  spi_send(TFT_RAMRD);
  spi_endSending();
  SPI_DC_D;
}

const uint8_t tft_transfer(const uint8_t cmd)
{
  uint8_t data;
  spi_read_blocking(rp2040_spi, cmd, &data, 1);
  return data;
}

const uint16_t tft_transfer16(const uint8_t cmd)
{
  uint8_t data[2];
  spi_read_blocking(rp2040_spi, cmd, data, 2);
  return ((int)data[0] << 8) | data[1];
}

#endif
