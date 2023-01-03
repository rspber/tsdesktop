/*
  Setup for TSDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include "Setup.h"
#include <pico/stdio.h>
#include <Display.h>
#include <Touch.h>
#include <RP2040_TFT_SPI.h>

void init_hardware()
{
  stdio_init_all();
}

RP2040_TFT_SPI displaySPI(TFT_CS, SPI0_DC, TFT_SPI_WRITE_SPEED);
RP2040_TFT_SPI touchSPI(TOUCH_CS, SPI0_DC, TOUCH_SPI_SPEED);

#define MAD_MY  0x80  // 00 top to botom, 80 bottom to top
#define MAD_MX  0x40  // 00 left to right, 40 right to left
#define MAD_YX  0x20  // it means that X and Y are exchanged, wrongly called MV
//#define MAD_MV  0x10  // vertical refresh direction, wrongly called ML
//#define MAD_RGB 0x00
//#define MAD_BGR 0x08
//#define MAD_MH  0x04 // horizontal refresh direction,
#define MAD_SS  0x02 // horizontal flip
#define MAD_GS  0x01 // vertical flip

void media_begin(uint8_t rotation)
{
  display.begin(&displaySPI, TFT_RST);

#if defined(GC9A01)
  uint8_t rev = 1 - REVERSE_MODE;
#else
  uint8_t rev = REVERSE_MODE;
#endif

#if defined(HX8357D)
  uint8_t REV = rev ? 0 : MAD_MY;
#else
  uint8_t REV = rev ? MAD_MX : 0;
#endif

  display.setRotation(rotation, REV);

//  display.invertDisplay(true);    // invert display colors  WHITE <-> BLACK

  touch.begin(&touchSPI);

#if defined(ILI9341) || defined(ILI9481) || defined(ILI9486) || defined(ILI9488)
  rotation = rotation & 1 ? rotation : ((rotation + 2) % 4);
#endif  

  touch.setRotation(rotation, rev);

  display.clearDisplay();
}
