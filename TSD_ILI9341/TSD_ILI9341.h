/*
  ILI9341 driver for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

  ILI9341 abstract driver, SPI free

  More information in TSD_ILI9341.cpp

*/

#pragma once

#include <TSD_GFX.h>
#include <Setup.h>

class TSD_ILI9341 : public TSD_GFX {
public:
  TSD_ILI9341(const int16_t w, const int16_t h)
  {
    _width = WIDTH = w >= 0 ? w : 0;
    _height = HEIGHT = h >= 0 ? h : 0;
  }

  const int16_t width() { return _width; }
  const int16_t height() { return _height; }

  void begin(const int16_t RST = -1);

  void setRotation(const int8_t rotation);
  void invertDisplay(bool invert);

  void hardReset();
  void reset();

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

  virtual void beginTransaction(const uint32_t Hz) = 0;
  virtual void endTransaction() = 0;

  virtual void sendCmd(const uint8_t cmd) = 0;
  virtual void sendData(const int16_t size, const uint8_t* data) = 0;
  virtual void sendCmdData(const uint8_t cmd, const int16_t size, const uint8_t* data) = 0;

  void sendCmdData(const uint8_t cmd, const uint8_t data)
  {
    sendCmdData(cmd, 1, &data);
  }

  void sendMDTData(const int16_t size, const uint8_t* data)
  {
    sendData(size * MDT_SIZE, data);
  }

protected:
  void fill_hrect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, const rgb_t color);
  void fill_vrect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, const rgb_t color);

private:
  int16_t WIDTH, HEIGHT;
  int16_t _width;       ///< Display width as modified by rotation
  int16_t _height;      ///< Display height as modified by rotation

  int16_t _RST;
};
