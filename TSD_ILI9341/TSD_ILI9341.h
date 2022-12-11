/*
  ILI9341 driver for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

  ILI9341 abstract driver, SPI free

  More information in TSD_ILI9341.cpp

*/

#pragma once

#include <TSD_SCREEN.h>
#include <Setup.h>

class TSD_ILI9341 : public TSD_SCREEN {
public:
  TSD_ILI9341(const int16_t w, const int16_t h)
   : TSD_SCREEN(w, h) {}

  void begin(const int16_t RST = -1);

  void setRotation(const int8_t rotation);
  void invertDisplay(bool invert);

  void hardReset();
  void reset();

  void scrollTo(int16_t y);
  void setScrollMargins(int16_t top, int16_t bottom);

  void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);

//  uint8_t readcommand8(uint8_t reg, uint8_t index = 0);

  void displayOff();
  void displayOn();

  virtual void writePixel(clip_t* clip, int16_t x, int16_t y, const rgb_t color);
  virtual void writeFastHLine(clip_t* clip, int16_t x, int16_t y, int16_t w, const rgb_t color);
  virtual void writeFastVLine(clip_t* clip, int16_t x, int16_t y, int16_t h, const rgb_t color);
  virtual void writeFillRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, const rgb_t color);
  virtual void writeFillRectGradient(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);

  virtual void beginTransaction(const uint32_t Hz) = 0;
  virtual void endTransaction() = 0;

  virtual void startWriteData() = 0;
  virtual void pushByte(const uint8_t u8bit) = 0;
  virtual void endWriteData() = 0;

  virtual void sendCmd(const uint8_t cmd) = 0;
  virtual void sendData(const uint8_t* data, const int16_t size) = 0;
  virtual void sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size) = 0;

  void sendCmdData(const uint8_t cmd, const uint8_t data)
  {
    sendCmdData(cmd, &data, 1);
  }

  void writeColor(int16_t w, int16_t h, const rgb_t color);

private:
  void writeFillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);
  void writeFillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);

private:
  int16_t _RST;
};
