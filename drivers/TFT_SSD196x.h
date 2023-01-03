/*
  SSD196x abstract driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_Driver.h>

#include <Setup.h>

#define BGR 1

#define MAD_MY  0x80  // 00 top to botom, 80 bottom to top
#define MAD_MX  0x40  // 00 left to right, 40 right to left
#define MAD_YX  0x20  // it means that X and Y are exchanged, wrongly called MV
//#define MAD_MV  0x10  // vertical refresh direction, wrongly called ML
//#define MAD_RGB 0x00
//#define MAD_BGR 0x08
//#define MAD_MH  0x04 // horizontal refresh direction, 
#define MAD_SS  0x02 // horizontal flip
#define MAD_GS  0x01 // vertical flip

#define beginTransact(Hz) spi->spiBegin(Hz)
#define endTransact() spi->spiEnd()
#define sendCmd(cmd) spi->sendCmd(cmd)
#define sendData(data, size) spi->sendData(data, size)
#define sendCmdData(cmd, data, size) spi->sendCmdData(cmd, data, size)
#define sendCmdByte(cmd, data) spi->sendCmdByte(cmd, data)

class TFT_SSD196x : public TFT_Driver {
public:
  TFT_SSD196x(const int16_t w, const int16_t h) : TFT_Driver(w, h) {}

  void begin(TFT_SPI* aspi, const int16_t aRST = -1)
  {
    spi = aspi;
    spi->begin();

    RST = aRST;
    if (RST >= 0) {
      pinMode(RST, OUTPUT);
    }

    hardReset();

    beginTransact(TFT_SETUP_SPEED);

    init();

    endTransact();
  }

  virtual void init() = 0;

  void setRotation(const uint8_t rotation, const uint8_t REV)
  {
    beginTransact(TFT_SETUP_SPEED);
    switch (rotation % 4) {
      case 0:
        sendCmdByte(TFT_MADCTL, ((MAD_YX | MAD_GS) ^ REV) | (BGR << 3));
        setSize(getWIDTH(), getHEIGHT());
      break;
      case 1:
        sendCmdByte(TFT_MADCTL, (0x00 ^ REV) | (BGR << 3));
        setSize(getHEIGHT(), getWIDTH());
      break;
      case 2:
        sendCmdByte(TFT_MADCTL, ((MAD_YX | MAD_SS) ^ REV) | (BGR << 3));
        setSize(getWIDTH(), getHEIGHT());
      break;
      case 3:
        sendCmdByte(TFT_MADCTL, ((MAD_SS | MAD_GS) ^ REV) | (BGR << 3));
        setSize(getHEIGHT(), getWIDTH());
      break;
    }
    endTransact();
  }

};
