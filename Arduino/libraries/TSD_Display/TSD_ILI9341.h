/*
  ILI9341 driver for TSDesktop on pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

  More information in TSD_ILI9341.cpp

*/

#pragma once

#include "TSD_SPITFT.h"
#include "Arduino.h"

class TSD_ILI9341 : public TSD_SPITFT {
public:
  TSD_ILI9341(const int16_t w, const int16_t h, int8_t cs, int8_t dc,
                  int8_t rst = -1)
   : TSD_SPITFT(cs, dc, rst)
  {
    _width = WIDTH = w >= 0 ? w : 0;
    _height = HEIGHT = h >= 0 ? h : 0;
  }

  const int16_t width() { return _width; }
  const int16_t height() { return _height; }

  void begin(uint32_t freq = 0);

  void setRotation(const int8_t rotation);
  void invertDisplay(bool invert);

  void fillScreen(const rgb_t color = BLACK);

  void scrollTo(uint16_t y);
  void setScrollMargins(uint16_t top, uint16_t bottom);

  void displayOff();
  void displayOn();

  // Transaction API not used by GFX
  void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

  uint8_t readcommand8(uint8_t reg, uint8_t index = 0);

protected:
  void sendCmd(const uint8_t cmd);
  void sendData(const int16_t size, const uint8_t* data);
  void sendCmdData(const uint8_t cmd, const int16_t size, const uint8_t* data);
  void sendCmdData(const uint8_t cmd, const uint8_t data);
  void reset();

private:
  int16_t WIDTH, HEIGHT;
  int16_t _width;       ///< Display width as modified by rotation
  int16_t _height;      ///< Display height as modified by rotation
};
