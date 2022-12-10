/*
  ILI9341 driver for TSDesktop on Picoino

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <TSD_ILI9341.h>
#include <Picoino.h>

class TSD_ILI9341_SPI : public TSD_ILI9341 {
public:
  TSD_ILI9341_SPI(const int16_t w, const int16_t h)
    : TSD_ILI9341(w, h)
  {
  }

  void begin(PicoSPI* spi, const int16_t RST = -1)
  {
    _spi = spi;

    TSD_ILI9341::begin(RST);
  }

  void beginTransaction(const uint32_t Hz)
  {
    _spi->beginTransaction(Hz);
  }

  void endTransaction()
  {
    _spi->endTransaction();
  }

  void startWrite()
  {
    _spi->beginTransaction();
  }

  void endWrite()
  {
    _spi->endTransaction();
  }

  void startWriteData()
  {
    _spi->dc(1);
    _spi->cs(0);
  }

  void pushByte(const uint8_t u8bit)
  {
    _spi->pushByte(u8bit);
  }

  void endWriteData()
  {
    _spi->endWrite();
    _spi->dc(1);
    _spi->cs(1);
  }

  void sendCmd(const uint8_t cmd)
  {
    _spi->writeCmd(cmd);
  }

  void sendData(const uint8_t* data, const int16_t size)
  {
    _spi->writeData(data, size);
  }

  void sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size)
  {
    _spi->writeCmdData(cmd, data, size);
  }

private:
  PicoSPI* _spi;
};
