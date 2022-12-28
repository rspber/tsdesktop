/*
  ILI9341 driver for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

  More information in TSD_ILI9341.cpp

*/

#pragma once

#include <TSD_SCREEN.h>
#include <TFT_SPI.h>

class TSD_ILI9341 : public TSD_SCREEN {
public:
  TSD_ILI9341(const int16_t w, const int16_t h)
   : TSD_SCREEN(w, h) {}

  void begin(TFT_SPI* aspi, const int16_t aRST = -1);

  void setRotation(const int8_t rotation);
  void invertDisplay(bool invert);

  void hardReset();
  void reset();

  void scrollTo(int16_t y);
  void setScrollMargins(int16_t top, int16_t bottom);

  void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);

  void displayOff();
  void displayOn();

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

private:
  void sendCmd2x16(const uint8_t cmd, const int16_t i1, const int16_t i2);

  void writeAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);
  void readAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);

  rgb_t readPixel(clip_t* clip, int16_t x, int16_t y);

  void writePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color);

  void writeColor(const int16_t w, const int16_t h, const rgb_t color);

private:
  void writeFillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);
  void writeFillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);

private:
  TFT_SPI* spi;
  int16_t RST;
};
