/*
  ILI9341 driver for TSDesktop on Arduino

  Copyright (c) 2022, rspber (https://github.com/rspber)

  Based on Adafruit's Adafruit_SPITFT

Original Adafruit's licence below.
*/

/*!
 * @file Adafruit_SPITFT.cpp
 *
 * @mainpage Adafruit SPI TFT Displays (and some others)
 *
 * @section intro_sec Introduction
 *
 * Part of Adafruit's GFX graphics library. Originally this class was
 * written to handle a range of color TFT displays connected via SPI,
 * but over time this library and some display-specific subclasses have
 * mutated to include some color OLEDs as well as parallel-interfaced
 * displays. The name's been kept for the sake of older code.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!

 * @section dependencies Dependencies
 *
 * This library depends on <a href="https://github.com/adafruit/Adafruit_GFX">
 * Adafruit_GFX</a> being present on your system. Please make sure you have
 * installed the latest version before using this library.
 *
 * @section author Author
 *
 * Written by Limor "ladyada" Fried for Adafruit Industries,
 * with contributions from the open source community.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 */

#pragma once

#include <TSD_ILI9341.h>
#include <SPI.h>

class TSD_ILI9341_SPI : public TSD_ILI9341 {
public:
  TSD_ILI9341_SPI(const int16_t w, const int16_t h, const int16_t cs, const int16_t dc, const int16_t rst = -1)
    : TSD_ILI9341(w, h), _cs(cs), _dc(dc), _rst(rst)
  {
  }

  void begin(const uint32_t freq, SPIClass* spi = &SPI)
  {
    _spi = spi;

    if (_cs >= 0) {
      digitalWrite(_cs, HIGH);
      pinMode(_cs, OUTPUT);
    }
    digitalWrite(_dc, HIGH);
    pinMode(_dc, OUTPUT);

    settings = SPISettings(freq, MSBFIRST, spiMode);
    spi->begin();
    TSD_ILI9341::begin(_rst);
  }

  void CS(bool state)
  {
    if (_cs >= 0) {
      digitalWrite(_cs, state);
    }
  }

  void DC(bool state)
  {
    if (_dc >= 0) {
      digitalWrite(_dc, state);
    }
  }

  void beginTransaction(const uint32_t Hz)
  {
    SPISettings tmp(Hz, MSBFIRST, spiMode);
    _spi->beginTransaction(tmp);
  }

  void endTransaction()
  {
    _spi->endTransaction();
  }

  void startWrite()
  {
    BEGIN_TRANSACTION();
  }

  void endWrite()
  {
    END_TRANSACTION();
  }

  void startWriteData()
  {
    DC(HIGH);
    CS(LOW);
  }

  void pushByte(const uint8_t u8bit)
  {
#if defined(ARDUINO_ARCH_RP2040)
    spi_inst_t *pi_spi = _spi == &SPI ? spi0 : spi1;
    while (!spi_is_writable(pi_spi))
      tight_loop_contents();
    spi_get_hw(pi_spi)->dr = (uint32_t)u8bit;
#elif defined(ESP8266) || defined(ESP32)
    _spi->wri te(b);
#else
    _spi->tran sfer(b);
#endif
  }

  void endWriteData()
  {
#if defined(ARDUINO_ARCH_RP2040)
    spi_inst_t *pi_spi = _spi == &SPI ? spi0 : spi1;
    // Drain RX FIFO, then wait for shifting to finish (which may be *after*
    // TX FIFO drains), then drain RX FIFO again
    while (spi_is_readable(pi_spi))
        (void)spi_get_hw(pi_spi)->dr;
    while (spi_get_hw(pi_spi)->sr & SPI_SSPSR_BSY_BITS)
        tight_loop_contents();
    while (spi_is_readable(pi_spi))
        (void)spi_get_hw(pi_spi)->dr;

    // Don't leave overrun flag set
    spi_get_hw(pi_spi)->icr = SPI_SSPICR_RORIC_BITS;
#endif
    DC(HIGH);
    CS(HIGH);
  }

  void spiWrite(const uint8_t b)
  {
#if defined(ARDUINO_ARCH_RP2040)
    spi_inst_t *pi_spi = _spi == &SPI ? spi0 : spi1;
    spi_write_blocking(pi_spi, &b, 1);
#elif defined(ESP8266) || defined(ESP32)
    _spi->wri te(b);
#else
    _spi->tran sfer(b);
#endif
  }

  void sendCmd(const uint8_t cmd)
  {
    CS(LOW);
    DC(LOW);
    spiWrite(cmd);
    DC(HIGH);
    CS(HIGH);
  }

  void sendData(const int16_t size, const uint8_t* data)
  {
    CS(LOW);
    DC(HIGH);
#if defined(ARDUINO_ARCH_RP2040)
    spi_inst_t *pi_spi = _spi == &SPI ? spi0 : spi1;
    spi_write_blocking(pi_spi, data, size);
#else
    uint8_t* p = (uint8_t*)data;
    for (int i = 0; i < size; ++i) {
      spiWrite(*p++);
    }
#endif
    CS(HIGH);
    DC(HIGH);
  }

  void sendCmdData(const uint8_t cmd, const int16_t size, const uint8_t* data)
  {
    CS(LOW);
    DC(LOW);
    spiWrite(cmd);

    DC(HIGH);
#if defined(ARDUINO_ARCH_RP2040)
    spi_inst_t *pi_spi = _spi == &SPI ? spi0 : spi1;
    spi_write_blocking(pi_spi, data, size);
#else
    uint8_t* p = (uint8_t*)data;
    for (int i = 0; i < size; ++i) {
      spiWrite(*p++);
    }
#endif
    CS(HIGH);
    DC(HIGH);
  }

protected:
  inline void BEGIN_TRANSACTION(void)
  {
    _spi->beginTransaction(settings);
  }
  inline void END_TRANSACTION(void)
  {
    _spi->endTransaction();
  }

private:
  uint8_t spiMode = SPI_MODE0;
  SPISettings settings;
  SPIClass* _spi;
  int16_t _cs, _dc, _rst;
};

