/*
  ILI9341 driver for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

This is a modified Adafruit's ILI9341 driver: Adafruit_ILI9341

differences:
- Adafruit_SPITFT is missed, all spi functions are abstract
- added support for ILI9341 devices v1.3

Original Adafruit's license attached below.
*/

/*!
 * @file Adafruit_ILI9341.cpp
 *
 * @mainpage Adafruit ILI9341 TFT Displays
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for Adafruit's ILI9341 driver for the
 * Arduino platform.
 *
 * This library works with the Adafruit 2.8" Touch Shield V2 (SPI)
 *    http://www.adafruit.com/products/1651
 *
 * Adafruit 2.4" TFT LCD with Touchscreen Breakout w/MicroSD Socket - ILI9341
 *    https://www.adafruit.com/product/2478
 *
 * 2.8" TFT LCD with Touchscreen Breakout Board w/MicroSD Socket - ILI9341
 *    https://www.adafruit.com/product/1770
 *
 * 2.2" 18-bit color TFT LCD display with microSD card breakout - ILI9340
 *    https://www.adafruit.com/product/1770
 *
 * TFT FeatherWing - 2.4" 320x240 Touchscreen For All Feathers
 *    https://www.adafruit.com/product/3315
 *
 * These displays use SPI to communicate, 4 or 5 pins are required
 * to interface (RST is optional).
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section dependencies Dependencies
 *
 * This library depends on <a href="https://github.com/adafruit/Adafruit_GFX">
 * Adafruit_GFX</a> being present on your system. Please make sure you have
 * installed the latest version before using this library.
 *
 * @section author Author
 *
 * Written by Limor "ladyada" Fried for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include "TSD_ILI9341.h"

#include <Setup.h>

#define BGR 1

//    # Command constants from ILI9341 datasheet
//#define ILI9341_NOP         0x00    // No-op register
#define ILI9341_SWRESET     0x01    // Software reset register
//#define ILI9341_RDDIDINF    0x04    // Read display identification information, returns non standard SPI 3 bytes
//#define ILI9341_RDDSTAT     0x09    // Read display status, returns non standard SPI 4 bytes
//#define ILI9341_RDDPWMODE   0x0A    // Read display power mode
//#define ILI9341_RDMADCTL    0x0B    // Read display MADCTL
//#define ILI9341_RDPIXFMT    0x0C    // Read display pixel format
//#define ILI9341_RDIMGFMT    0x0D    // Read display image format
//#define ILI9341_RDSELFDIAG  0x0F    // Read display self-diagnostic
//#define ILI9341_SLPIN       0x10    // Enter sleep mode
#define ILI9341_SLPOUT      0x11    // Exit sleep mode
//#define ILI9341_PTLON       0x12    // Partial mode on
//#define ILI9341_NORON       0x13    // Normal display mode on
#define ILI9341_INVOFF      0x20    // Display inversion off
#define ILI9341_INVON       0x21    // Display inversion on
#define ILI9341_GAMMASET    0x26    // Gamma set
#define ILI9341_DISPOFF     0x28    // Display off
#define ILI9341_DISPON      0x29    // Display on
#define ILI9341_CASET       0x2A    // Column address set
#define ILI9341_PASET       0x2B    // Page address set
#define ILI9341_RAMWR       0x2C    // Memory write
//#define ILI9341_RAMRD       0x2E    // Memory read
//#define ILI9341_PTLAR       0x30    // Partial area
#define ILI9341_VSCRDEF     0x33    // Vertical scrolling definition
#define ILI9341_MADCTL      0x36    // Memory access control
#define ILI9341_VSCRSADD    0x37    // Vertical scrolling start address
#define ILI9341_PIXFMT      0x3A    // COLMOD: Pixel format set
//#define ILI9341_WRITE_DISPLAY_BRIGHTNESS 0x51    // Brightness hardware dependent!
//#define ILI9341_READ_DISPLAY_BRIGHTNESS  0x52
//#define ILI9341_WRITE_CTRL_DISPLAY       0x53
//#define ILI9341_READ_CTRL_DISPLAY        0x54
//#define ILI9341_WRITE_CABC  0x55    // Write Content Adaptive Brightness Control
//#define ILI9341_READ_CABC   0x56    // Read Content Adaptive Brightness Control
//#define ILI9341_WRITE_CABC_MINIMUM       0x5E    // Write CABC Minimum Brightness
//#define ILI9341_READ_CABC_MINIMUM        0x5F    // Read CABC Minimum Brightness
#define ILI9341_FRMCTR1     0xB1    // Frame rate control (In normal mode/full colors)
//#define ILI9341_FRMCTR2     0xB2    // Frame rate control (In idle mode/8 colors)
//#define ILI9341_FRMCTR3     0xB3    // Frame rate control (In partial mode/full colors)
//#define ILI9341_INVCTR      0xB4    // Display inversion control
//#define ILI9341_BLPORCTL    0xB5    // Blanking Porch Controll : 0..255, from v.1.02  : 0..127
#define ILI9341_DFUNCTR     0xB6    // Display function control
//#define ILI9341_ENTMODSET   0xB7    // Entry Mode Set
#define ILI9341_PWCTR1      0xC0    // Power control 1
#define ILI9341_MADCTL13    0xC0    // v1.3 configuration xCRICxCC
//  I - inverse colors,
//  R - 0:RGB, 1:BGR,
//  C__C_CC - rotation + reverse mode
//  CxxCxCC             | CxxCxCC
//  0..0.00    0  <--   | 1..0.00    0  -->
//  0..0.01    0  -->   | 1..0.01    0  <--
//  0..0.10   90  -->   | 1..0.10   90  <--
//  0..0.11   90  <--   | 1..0.11   90  -->
//  0..1.00  180  -->   | 1..1.00  180  <--
//  0..1.01  180  <--   | 1..1.01  180  -->
//  0..1.10  270  <--   | 1..1.10  270  -->
//  0..1.11  270  -->   | 1..1.11  270  <--
#define ILI9341_PWCTR2      0xC1    // Power control 2
//#define ILI9341_PWCTR3      0xC2    // Power Control 3
//#define ILI9341_PWCTR4      0xC3    // Power Control 4
//#define ILI9341_PWCTR5      0xC4    // Power Control 5
#define ILI9341_VCOMCTR1      0xC5    // Set the VCOM(H/L) voltage
#define ILI9341_VCOMCTR2      0xC7    // Set the VCOM offset voltage
//#define ILI9341_RDID1       0xDA    // Read ID 1
//#define ILI9341_RDID2       0xDB    // Read ID 2
//#define ILI9341_RDID3       0xDC    // Read ID 3
//#define ILI9341_RDID4       0xD3    // Read ID 4
#define ILI9341_GMCTRP1     0xE0    // Positive gamma correction
#define ILI9341_GMCTRN1     0xE1    // Negative gamma correction
#define ILI9341_EF          0xEF    // unknown
//#define ILI9341_INTFCCTR    0xF6    // Interface control
#define ILI9341_PUMPRC      0xF7    // Pump ratio control
//#define ILI9341_PWCTR6      0xFC

// retired in version v.1.02
#define ILI9341_PWCTRA      0xCB    // Power control A
#define ILI9341_PWCTRB      0xCF    // Power control B
#define ILI9341_DTCA        0xE8    // Driver timing control A
// E9
#define ILI9341_DTCB        0xEA    // Driver timing control B
#define ILI9341_POSC        0xED    // Power on sequence control
#define ILI9341_ENABLE3G    0xF2    // Enable 3 gamma control

#define SETUP_SPEED 2 * 1000 * 1000   // 2 MHz

void TSD_ILI9341::hardReset()
{
  if (_RST >= 0) {
    digitalWrite(_RST, LOW);
    sleep_ms(50);
    digitalWrite(_RST, HIGH);
    sleep_ms(50);
  }
}

void TSD_ILI9341::reset()
{
  hardReset();
  beginTransaction(SETUP_SPEED);   // slow down
  sendCmd(ILI9341_SWRESET); // Engage software reset
  sleep_ms(150);
  endTransaction();
}

void TSD_ILI9341::begin(const int16_t RST)
{
  _RST = RST;

  if (RST >= 0) {
    pinMode(RST, OUTPUT);
  }

  hardReset();

  beginTransaction(SETUP_SPEED);

  sendCmd(ILI9341_SWRESET); // Engage software reset
  sleep_ms(150);

  if (ILI9341_VERSION < 3) {  // < v1.2
    sendCmdData(ILI9341_EF, (uint8_t*)"\x03\x80\x02", 3);              // unknown
    sendCmdData(ILI9341_PWCTRB, (uint8_t*)"\x00\xC1\x30", 3);      // retired in v.1.02
    sendCmdData(ILI9341_POSC, (uint8_t*)"\x64\x03\x12\x81", 4);          // retired in v.1.02
    sendCmdData(ILI9341_DTCA, (uint8_t*)"\x85\x00\x78", 3);          // retired in v.1.02
    sendCmdData(ILI9341_PWCTRA, (uint8_t*)"\x39\x2C\x00\x34\x02", 5);      // retired in v.1.02
  }
  sendCmdData(ILI9341_PUMPRC,   0x20);

  if (ILI9341_VERSION < 3) {  // < v1.2
    sendCmdData(ILI9341_DTCB, (uint8_t*)"\x00\x00", 2);          // retired in v.1.02
  }
  if (ILI9341_VERSION < 3) {  // < v1.2
    sendCmdData(ILI9341_PWCTR1, 0x23);             // Power control VRH[5:0]
  }
  else {
    sendCmdData(ILI9341_MADCTL13, 0x21);           // xCRICxCC
  }
  sendCmdData(ILI9341_PWCTR2,   0x10);             // Power control SAP[2:0];BT[3:0]
  sendCmdData(ILI9341_VCOMCTR1, (uint8_t*)"\x31\x3c", 2);       // VCM control 1
  sendCmdData(ILI9341_VCOMCTR2, 0xC0);             // VCM control 2
  if (ILI9341_VERSION < 3) {  // < v1.2
    sendCmdData(ILI9341_MADCTL,   0x48);             // Memory Access Control
  }
  sendCmdData(ILI9341_VSCRSADD, 0x00);             // Vertical scroll zero
#ifdef COLOR_565
  sendCmdData(ILI9341_PIXFMT,   0x55);
#else
  sendCmdData(ILI9341_PIXFMT,   0x66);
#endif
  sendCmdData(ILI9341_FRMCTR1, (uint8_t*)"\x00\x18", 2);
  sendCmdData(ILI9341_DFUNCTR, (uint8_t*)"\x08\x82\x27", 3);  // Display Function Control
  sendCmdData(ILI9341_ENABLE3G, 0x00);             // 3Gamma Function Disable   // retired in v.1.02
  sendCmdData(ILI9341_GAMMASET, 0x01);             // Gamma curve selected
  if (ILI9341_VERSION < 3) {  // < v1.2
                                         //"\x0F\x31\x2B\x0C\x0E\x08\x4E\xF1\x37\x07\x10\x03\x0E\x09\x00" // original
    sendCmdData(ILI9341_GMCTRP1, (uint8_t*)"\x0F\x31\x2B\x0C\x0E\x08\x4E\x21\x26\x07\x10\x03\x0E\x09\x00", 15);  // positive gamma correction
                                         //"\x00\x0E\x14\x03\x11\x07\x31\xC1\x48\x08\x0F\x0C\x31\x36\x0F" // original
    sendCmdData(ILI9341_GMCTRN1, (uint8_t*)"\x00\x0E\x14\x03\x11\x07\x31\xC1\x48\x08\x0F\x0C\x31\x36\x0F", 15);  // negative gamma correction
  }
  else {
    sendCmdData(ILI9341_GMCTRP1, (uint8_t*)"\x0F\x31\x2B\x0C\x0E\x08\x30\xF1\x37\x07\x10\x03\x0E\x09\x00", 15);  // positive gamma correction v1.3
    sendCmdData(ILI9341_GMCTRN1, (uint8_t*)"\x00\x0E\x14\x03\x11\x07\x11\xC1\x48\x08\x0F\x0C\x31\x36\x0F", 15);  // negative gamma correction
  }

  sendCmd(ILI9341_SLPOUT);   // Exit Sleep
  delay(150);

  sendCmd(ILI9341_DISPON);   // Display on
  delay(150);

  endTransaction();
}

void TSD_ILI9341::displayOff()
{
  beginTransaction(SETUP_SPEED);
  sendCmd(ILI9341_DISPOFF);
  endTransaction();
}

void TSD_ILI9341::displayOn()
{
  beginTransaction(SETUP_SPEED);
  sendCmd(ILI9341_DISPON);
  endTransaction();
}

/**************************************************************************/
/*!
   @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void TSD_ILI9341::setRotation(const int8_t rotation) {
  uint8_t m = 0;
  uint8_t g = 0; // v1.3
  switch (rotation % 4) { // can't be higher than 3
  case 0:
    m = 0x40 | (BGR << 3); // MX
    g = 0x01 | (BGR << 5);
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 1:
    m = 0x20 | (BGR << 3); // MV
    g = 0x02 | (BGR << 5);
    setSize(getHEIGHT(), getWIDTH());
    break;
  case 2:
    m = 0x80 | (BGR << 3); // MY
    g = 0x08 | (BGR << 5);
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 3:
    m = 0xe0 | (BGR << 3); // MX | MY | MV
    g = 0x0b | (BGR << 5);
    setSize(getHEIGHT(), getWIDTH());
    break;
  }
  beginTransaction(SETUP_SPEED);
  if (ILI9341_VERSION < 3) { // < v1.2
    sendCmdData(ILI9341_MADCTL, m);
  }
  else {
    sendCmdData(ILI9341_MADCTL13, g);
  }
  endTransaction();
}

/**************************************************************************/
/*!
   @brief   Enable/Disable display color inversion
    @param   invert True to invert, False to have normal color
*/
/**************************************************************************/
void TSD_ILI9341::invertDisplay(bool invert)
{
  beginTransaction(SETUP_SPEED);
  sendCmd(invert ? ILI9341_INVON : ILI9341_INVOFF);
  endTransaction();
}

/**************************************************************************/
/*!
   @brief   Scroll display memory
    @param   y How many pixels to scroll display by
*/
/**************************************************************************/
void TSD_ILI9341::scrollTo(int16_t y) {
  uint8_t data[2];
  data[0] = y >> 8;
  data[1] = y & 0xff;
  beginTransaction(SETUP_SPEED);
  sendCmdData(ILI9341_VSCRSADD, (uint8_t*)data, 2);
  endTransaction();
}

/**************************************************************************/
/*!
   @brief   Set the height of the Top and Bottom Scroll Margins
    @param   top The height of the Top scroll margin
    @param   bottom The height of the Bottom scroll margin
 */
 /**************************************************************************/
void TSD_ILI9341::setScrollMargins(int16_t top, int16_t bottom) {
  // TFA+VSA+BFA must equal 320
  if (top + bottom <= getHEIGHT()) {
    uint16_t middle = getHEIGHT() - (top + bottom);
    uint8_t data[6];
    data[0] = top >> 8;
    data[1] = top & 0xff;
    data[2] = middle >> 8;
    data[3] = middle & 0xff;
    data[4] = bottom >> 8;
    data[5] = bottom & 0xff;
    beginTransaction(SETUP_SPEED);
    sendCmdData(ILI9341_VSCRDEF, (uint8_t*)data, 6);
    endTransaction();
  }
}

/**************************************************************************/
/*!
    @brief   Set the "address window" - the rectangle we will write to RAM with
   the next chunk of      SPI data writes. The ILI9341 will automatically wrap
   the data as each row is filled
    @param   x1  TFT memory 'x' origin
    @param   y1  TFT memory 'y' origin
    @param   w   Width of rectangle
    @param   h   Height of rectangle
*/
/**************************************************************************/
void TSD_ILI9341::setAddrWindow(int16_t x1, int16_t y1, int16_t w, int16_t h) {
  static int16_t old_x1 = 0x7fff, old_x2 = 0x7fff;
  static int16_t old_y1 = 0x7fff, old_y2 = 0x7fff;

  #define SWAP16(x) (x >> 8 | x << 8)

  int16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
  if (x1 != old_x1 || x2 != old_x2) {
    sendCmd(ILI9341_CASET); // Column address set
    uint16_t buf[2];
    buf[0] = SWAP16(x1);
    buf[1] = SWAP16(x2);
    sendData((const uint8_t*)buf, sizeof(buf));
    old_x1 = x1;
    old_x2 = x2;
  }
  if (y1 != old_y1 || y2 != old_y2) {
    sendCmd(ILI9341_PASET); // Row address set
    uint16_t buf[2];
    buf[0] = SWAP16(y1);
    buf[1] = SWAP16(y2);
    sendData((const uint8_t*)buf, sizeof(buf));
    old_y1 = y1;
    old_y2 = y2;
  }
  sendCmd(ILI9341_RAMWR); // Write to RAM
}

void TSD_ILI9341::writeColor(int16_t w, int16_t h, const rgb_t color)
{
  uint8_t buf[8];
  mdt_color(buf, color, 1);
  startWriteData();
  for (int j = 0; j < h; ++j) {
    for (int i = 0; i < w; ++i) {
      for (int k = 0; k < MDT_SIZE; ++k) {
        pushByte(buf[k]);
      }
    }
  }
  endWriteData();
}

void TSD_ILI9341::writePixel(clip_t* clip, int16_t x, int16_t y, const rgb_t color)
{
  if (x >= clip->x1 && y >= clip->y1 && x < clip->x2 && y < clip->y2) {
    setAddrWindow(x, y, 1, 1);
    writeColor(1, 1, color);
  }
}

void TSD_ILI9341::writeFastHLine(clip_t* clip, int16_t x, int16_t y, int16_t w, const rgb_t color)
{
  if (x < clip->x1) {
    w -= clip->x1 - x;
    x = clip->x1;
  }
  if (x + w > clip->x2) {
    w = clip->x2 - x;
  }
  if (y >= clip->y1 && y < clip->y2 && w > 0) {
    setAddrWindow(x, y, w, 1);
    writeColor(w, 1, color);
  }
}

void TSD_ILI9341::writeFastVLine(clip_t* clip, int16_t x, int16_t y, int16_t h, const rgb_t color)
{
  if (y < clip->y1) {
    h -= clip->y1 - y;
    y = clip->y1;
  }
  if (y + h > clip->y2) {
    h = clip->y2 - y;
  }
  if (x >= clip->x1 && x < clip->x2 && h > 0) {
    setAddrWindow(x, y, 1, h);
    writeColor(1, h, color);
  }
}

void TSD_ILI9341::writeFillRect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, const rgb_t color)
{
  if (x < clip->x1) {
    w -= clip->x1 - x;
    x = clip->x1;
  }
  if (x + w > clip->x2) {
    w = clip->x2 - x;
  }
  if (y < clip->y1) {
    h -= clip->y1 - y;
    y = clip->y1;
  }
  if (y + h > clip->y2) {
    h = clip->y2 - y;
  }
  if (w > 0 && h > 0) {
    setAddrWindow(x, y, w, h);
    writeColor(w, h, color);
  }
}
