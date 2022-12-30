/*
  TFT abstract driver

  Copyright (c) 2022, rspber (https://github.com/rspber)

  More information in TFT_Driver.cpp

*/

#pragma once

#include <TSD_SCREEN.h>
#include <TFT_SPI.h>

class TFT_Driver : public TSD_SCREEN {
public:
  TFT_Driver(const int16_t w, const int16_t h)
   : TSD_SCREEN(w, h) {}

  virtual void begin(TFT_SPI* aspi, const int16_t aRST = -1) = 0;
  virtual void setRotation(const int8_t rotation) = 0;

  virtual void readRegister(uint8_t* buf, const uint8_t reg, int8_t len);

  virtual void invertDisplay(bool invert);

  virtual void hardReset();
  virtual void reset();

  virtual void scrollTo(int16_t y);
  virtual void setScrollMargins(int16_t top, int16_t bottom);

  virtual void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);

  virtual void displayOff();
  virtual void displayOn();

  void startWrite()
  {
    spi->spiBegin();
  }

  void endWrite()
  {
    spi->spiEnd();
  }

  void writePixel(clip_t* clip, int16_t x, int16_t y, const rgb_t color);
  void writeFastHLine(clip_t* clip, int16_t x, int16_t y, int16_t w, const rgb_t color);
  void writeFastVLine(clip_t* clip, int16_t x, int16_t y, int16_t h, const rgb_t color);
  void writeFillRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, const rgb_t color);
  void writeFillRectGradient(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);

  virtual rgb_t readPixel(clip_t* clip, int16_t x, int16_t y);

protected:
  void sendCmd2x16(const uint8_t cmd, const int16_t i1, const int16_t i2);

  void writeAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);
  void readAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);

  void writePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color);
  void storePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, over_t* t);

  void writeColor(const int16_t w, const int16_t h, const rgb_t color);

protected:
  void writeFillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);
  void writeFillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);

protected:
  TFT_SPI* spi;
  int16_t RST;
};
