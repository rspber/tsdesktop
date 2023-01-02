/*
  TFT abstract driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

  More information in TFT_Driver.cpp

*/

#pragma once

#include <TSD_SCREEN.h>
#include <TFT_SPI.h>

// common ILIxxxx commands

#define TFT_NOP         0x00    // No-op register
#define TFT_SWRESET     0x01    // Software reset register
#define TFT_SLPIN       0x10    // Enter sleep mode
#define TFT_SLPOUT      0x11    // Exit sleep mode
#define TFT_PTLON       0x12    // Partial mode on
#define TFT_NORON       0x13    // Normal display mode on
#define TFT_INVOFF      0x20    // Display inversion off
#define TFT_INVON       0x21    // Display inversion on
#define TFT_DISPOFF     0x28    // Display off
#define TFT_DISPON      0x29    // Display on
#define TFT_CASET       0x2A    // Column address set
#define TFT_PASET       0x2B    // Page address set
#define TFT_RAMWR       0x2C    // Memory write
#define TFT_RAMRD       0x2E    // Memory read
#define TFT_PTLAR       0x30    // Partial area
#define TFT_VSCRDEF     0x33    // Vertical scrolling definition
#define TFT_TEAROFF     0x34    // Tearing effect line off
#define TFT_TEARON      0x35    // Tearing effect line on
#define TFT_MADCTL      0x36    // Memory data access control
#define TFT_VSCRSADD    0x37    // Vertical scrolling start address
#define TFT_IDLEOFF     0x38    // Exit idle mode
#define TFT_IDLEON      0x39    // Enter idle mode
#define TFT_PIXFMT      0x3A    // COLMOD: Pixel format set
#define TFT_RWRCNT      0x3C    // Write memory continue
#define TFT_RRDNCT      0x3E    // Read memory continue
#define TFT_TEARLINE    0x44    // Set tear scanline
#define TFT_RDSCLN      0x45    // Get scanline
#define TFT_WRDISBRI    0x51    // Write display brightness
#define TFT_RDDISBRI    0x52    // Read display brightness
#define TFT_WRCTRLDIS   0x53    // Write ctrl display
#define TFT_RDCTRLDIS   0x54    // Read ctrl display
#define TFT_WRITE_CABC  0x55    // Write Content Adaptive Brightness Control
#define TFT_READ_CABC   0x56    // Read Content Adaptive Brightness Control
#define TFT_WRCABCMIN   0x5E    // Write CABC Minimum Brightness
#define TFT_RDCABCMIN   0x5F    // Read CABC Minimum Brightness
#define TFT_IDXRD       0xD9    // undocumented

#if defined COLOR_565
  #define TFT_PIXFMT_VALUE 0x55
#else
  #define TFT_PIXFMT_VALUE 0x66
#endif
/*
#define MAD_MY  0x80
#define MAD_MX  0x40
#define MAD_MV  0x20
#define MAD_ML  0x10
#define MAD_RGB 0x00
#define MAD_BGR 0x08
#define MAD_MH  0x04
#define MAD_SS  0x02
#define MAD_GS  0x01
*/

class TFT_Driver : public TSD_SCREEN {
public:
  TFT_Driver(const int16_t w, const int16_t h)
   : TSD_SCREEN(w, h) {}

  virtual void begin(TFT_SPI* aspi, const int16_t aRST = -1) = 0;
  virtual void setRotation(const uint8_t rotation, const uint8_t REV) = 0;

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
