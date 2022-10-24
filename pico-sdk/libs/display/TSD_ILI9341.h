/*
  ILI9341 driver for TSDesktop on pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

  More information in TSD_ILI9341.cpp

*/

#pragma once

#include "TSD_GFX.h"
#include <Picoino.h>

#define ILI9341_TFTWIDTH 240  ///< ILI9341 max TFT width
#define ILI9341_TFTHEIGHT 320 ///< ILI9341 max TFT height

class TSD_ILI9341 : public TSD_GFX {
public:
  TSD_ILI9341(const int16_t w = ILI9341_TFTWIDTH, const int16_t h = ILI9341_TFTHEIGHT)
    : TSD_GFX(w, h)
  {
  }

  void begin(PicoSPI* spi, const int16_t RST = -1);

  void setRotation(uint8_t m);
  void invertDisplay(bool invert);

  void fillScreen(const uint16_t color = BLACK);

  void scrollTo(uint16_t y);
  void setScrollMargins(uint16_t top, uint16_t bottom);

  void displayOff();
  void displayOn();

  virtual void drawPixel(const int16_t x, const int16_t y, const uint16_t color);
  virtual void drawFastHLine(int16_t x, const int16_t y, int16_t w, const uint16_t color);
  virtual void drawFastVLine(const int16_t x, int16_t y, int16_t h, const uint16_t color);
  virtual void fillRect(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const uint16_t color);

  uint16_t width() { return _width; }
  uint16_t height() { return _height; }

protected:
  void sendCmd(const uint8_t cmd);
  void sendData(const uint8_t* data, const int16_t size);
  void sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size);
  void reset();

  void block(const int16_t x0, const int16_t y0, const int16_t x1, const int16_t y1, const uint16_t* data, int16_t size);
  void fill_hrect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t color);
  void fill_vrect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t color);

private:
  PicoSPI* _spi;
  int16_t _RST;
  int8_t rotation;
};
