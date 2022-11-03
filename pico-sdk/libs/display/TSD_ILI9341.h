/*
  ILI9341 driver for TSDesktop on pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

  More information in TSD_ILI9341.cpp

*/

#pragma once

#include <TSD_GFX.h>
#include <Picoino.h>

class TSD_ILI9341 : public TSD_GFX {
public:
  TSD_ILI9341(const int16_t w, const int16_t h)
  {
    _width = WIDTH = w >= 0 ? w : 0;
    _height = HEIGHT = h >= 0 ? h : 0;
  }

  const int16_t width() { return _width; }
  const int16_t height() { return _height; }

  void begin(PicoSPI* spi, const int16_t RST = -1);

  void setRotation(const int8_t rotation);
  void invertDisplay(bool invert);

  void fillScreen(const rgb_t color = BLACK);

  void scrollTo(int16_t y);
  void setScrollMargins(int16_t top, int16_t bottom);

  void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);

//  uint8_t readcommand8(uint8_t reg, uint8_t index = 0);

  void displayOff();
  void displayOn();

  virtual void drawPixel(clip_t* clip, const int16_t x, const int16_t y, const rgb_t color);
  virtual void drawFastHLine(clip_t* clip, int16_t x, const int16_t y, int16_t w, const rgb_t color);
  virtual void drawFastVLine(clip_t* clip, const int16_t x, int16_t y, int16_t h, const rgb_t color);
  virtual void fillRect(clip_t* clip, const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color);

protected:
  void sendCmd(const uint8_t cmd);
  void sendData(const int16_t size, const uint8_t* data);
  void sendCmdData(const uint8_t cmd, const int16_t size, const uint8_t* data);
  void sendCmdData(const uint8_t cmd, const uint8_t data);
  void sendMDTData(const int16_t size, const uint8_t* data);
  void reset();

  void fill_hrect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, const rgb_t color);
  void fill_vrect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, const rgb_t color);

private:
  int16_t WIDTH, HEIGHT;
  int16_t _width;       ///< Display width as modified by rotation
  int16_t _height;      ///< Display height as modified by rotation

  PicoSPI* _spi;
  int16_t _RST;
};
