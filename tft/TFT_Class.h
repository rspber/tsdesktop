/*
  TFT abstract driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TSD_SCREEN.h>
#include <Setup.h>

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

#if defined(COLOR_565)
  #define TFT_PIXFMT_VALUE 0x55
#else
  #define TFT_PIXFMT_VALUE 0x66
#endif
/*
#define MAD_MY  0x80  // 00 top to botom, 80 bottom to top
#define MAD_MX  0x40  // 00 left to right, 40 right to left
#define MAD_YX  0x20  // it means that X and Y are exchanged, wrongly called MV
#define MAD_MV  0x10  // vertical refresh direction, wrongly called ML
#define MAD_RGB 0x00
#define MAD_BGR 0x08
#define MAD_MH  0x04 // horizontal refresh direction,
#define MAD_SS  0x02 // horizontal flip
#define MAD_GS  0x01 // vertical flip
*/

class TFT_Class : public TSD_SCREEN {
public:
  TFT_Class(const int16_t w, const int16_t h)
   : TSD_SCREEN(w, h) {}

  virtual void begin()
  {
    if (RST >= 0) {
      pinMode(RST, OUTPUT);
    }
    hardReset();
    beginTransaction(TFT_SETUP_SPEED);
    init();
    endTransaction();
  }

  void setRotation(const uint8_t r, const uint8_t REV)
  {
    beginTransaction(TFT_SETUP_SPEED);
    rotation(r, REV);
    endTransaction();
  }

  virtual void readRegister(uint8_t* buf, const uint8_t reg, int8_t len);

  virtual void invertDisplay(bool invert);

  virtual void hardReset();
  virtual void reset();

// actually not used
  virtual void scrollTo(int16_t y);
  virtual void setScrollMargins(int16_t top, int16_t bottom);

  virtual void displayOff();
  virtual void displayOn();

  virtual rgb_t readPixel(clip_t* clip, int16_t x, int16_t y);

// override by the driver
protected:
  virtual void init() = 0;
  virtual void rotation(const uint8_t r, const uint8_t REV) = 0;

// override by the transfer protocol
protected:

  virtual void cs(const uint8_t mode) {}
  virtual void dc(const uint8_t mode) {}

  virtual void beginTransaction(uint Hz)
  {
    cs(0);
  }

  virtual void beginTransaction() = 0;

  virtual void endTransaction()
  {
    cs(1);
  }

  void startWrite()
  {
    beginTransaction();
  }

  void endWrite()
  {
    endTransaction();
  }

  virtual void startSending()
  {
//#if defined(ST7789) || defined(ST7796)
    cs(1);
    cs(0);
//#endif
  }

  virtual void send(const uint8_t data) = 0;
  virtual void endSending() {}

  virtual void sendData(const uint8_t data)
  {
    startSending();
    send(data);
    endSending();
  }

  virtual void sendCmd(const uint8_t cmd)
  {
    dc(0);
    sendData(cmd);
    dc(1);
  }

  virtual void sendData(const uint8_t* data, const int16_t size)
  {
    startSending();
    for (int i = 0; i < size; ++i) {
      send(data[i]);
    }
    endSending();
  }

  virtual void sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size)
  {
    sendCmd(cmd);
    if (size > 0) {
      sendData(data, size);
    }
  }

  virtual void sendCmdByte(const uint8_t cmd, const uint8_t data)
  {
    sendCmd(cmd);
    sendData(data);
  }

  virtual void startTransfer() {}
  virtual const uint8_t transfer(const uint8_t cmd) = 0;
  virtual const uint16_t transfer16(const uint8_t cmd) = 0;
  virtual void endTransfer() {}

protected:
  virtual void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);

  void writePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color);
  void writeFillRectGradient(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);

  virtual void sendCmd2x16(const uint8_t cmd, const int16_t i1, const int16_t i2);

  virtual void writeAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);
  virtual void readAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h);

  virtual void storePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, over_t* t);

  virtual void writeColor(const int16_t w, const int16_t h, const rgb_t color);

protected:
  virtual void writeFillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);
  virtual void writeFillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z);

protected:
  int16_t RST = -1;
};
