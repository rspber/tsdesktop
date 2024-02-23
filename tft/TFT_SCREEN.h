/*
  TFT SCREEN

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#pragma once

#include <TSD_SCREEN.h>

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

#define MAD_MY  0x80  // 00 top to botom, 80 bottom to top
#define MAD_MX  0x40  // 00 left to right, 40 right to left
#define MAD_YX  0x20  // it means that X and Y are exchanged, wrongly called MV
#define MAD_MV  0x10  // vertical refresh direction, wrongly called ML
#define MAD_BGR 0x08
#define MAD_RGB 0x00
#define MAD_MH  0x04 // horizontal refresh direction,
#define MAD_SS  0x02 // horizontal flip
#define MAD_GS  0x01 // vertical flip

class TFT_SCREEN : public TSD_SCREEN {
public:
  TFT_SCREEN() : TSD_SCREEN(TFT_WIDTH, TFT_HEIGHT)
  {
  }
  const char* protocol();

  virtual void begin();
  virtual void setRotation(const uint8_t r, const uint8_t REV = 0);

  void invertDisplay(bool invert);

  void displayOff();
  void displayOn();

// currently not used
  void scrollTo(int16_t y);
  void setScrollMargins(int16_t top, int16_t bottom);
//
  rgb_t readPixel(clip_t& clip, int16_t x, int16_t y);
  void readRegister(uint8_t* buf, const uint8_t reg, int8_t len);

  void sendCmd(const uint8_t cmd);
  void sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size);
  void sendCmdByte(const uint8_t cmd, const uint8_t b);

  rgb_t innerReadPixel(int16_t x, int16_t y);

  void startWrite() override;
  void endWrite() override;
  void writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h) override;
  void sendMDTColor1(const mdt_t c) override;
  void sendMDTColor(const mdt_t c, const int32_t len) override;
  void drawPixel1(const int16_t x, const int16_t y, const rgb_t color) override;
  void drawPixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color) override;
  void drawMDTBuffer(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const uint8_t* buffer) override;
};
