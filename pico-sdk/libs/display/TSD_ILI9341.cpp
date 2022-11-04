/*
  ILI9341 driver for TSDesktop on pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

1. This is a modified Adafruit's ILI9341 driver: Adafruit_ILI9341
natively ported to pico-sdk

differences:
- Adafruit_SPITFT is missed, all spi functions are handled by Picoino
- only hardware spi is supported
- added support for ILI9341 devices v1.3

Original Adafruit's licence bellow.
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

/*
2. SPI TFT implementation for drawPixel and other draws comes from
https://github.com/rdagger/micropython-ili9341/ili9341.py,
partially rewriten from python to c,

changes:
- added overflow: clip
- setAddrWindow was used

that software is under the MIT license, attached below.
*/

/*
MIT License

Copyright (c) 2020

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "TSD_ILI9341.h"
#include <Picoino.h>

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

void TSD_ILI9341::sendCmd(const uint8_t cmd)
{
  _spi->writeCmd(cmd);
}

void TSD_ILI9341::sendData(const int16_t size, const uint8_t* data)
{
  _spi->writeData(data, size);
}

void TSD_ILI9341::sendCmdData(const uint8_t cmd, const int16_t size, const uint8_t* data)
{
  sendCmd(cmd);
  if (size > 0) {
    sendData(size, data);
  }
}

void TSD_ILI9341::sendCmdData(const uint8_t cmd, const uint8_t data)
{
  sendCmd(cmd);
  sendData(1, &data);
}

void TSD_ILI9341::reset()
{
  if (_RST >= 0) {
    digitalWrite(_RST, LOW);
    sleep_ms(50);
    digitalWrite(_RST, HIGH);
    sleep_ms(50);
  }
  else {                 // If no hardware reset pin...
    sendCmd(ILI9341_SWRESET); // Engage software reset
    sleep_ms(150);
  }
}

void TSD_ILI9341::begin(PicoSPI* spi, const int16_t RST)
{
  _spi = spi;
  _RST = RST;

  if (RST >= 0) {
    pinMode(RST, OUTPUT);
  }
  _spi->softTransaction(2 * 1000 * 1000);   // slow down

  reset();
  sendCmd(ILI9341_SWRESET); // Engage software reset
  sleep_ms(150);

  if (ILI9341_VERSION < 3) {  // < v1.2
    static uint8_t ILI9341_EF_DATA[] = {0x03, 0x80, 0x02};
    sendCmdData(ILI9341_EF, 3, ILI9341_EF_DATA);              // unknown

    static uint8_t ILI9341_PWCTRB_DATA[] = {0x00, 0xC1, 0x30};
    sendCmdData(ILI9341_PWCTRB, 3, ILI9341_PWCTRB_DATA);      // retired in v.1.02

    static uint8_t ILI9341_POSC_DATA[] = {0x64, 0x03, 0x12, 0x81};
    sendCmdData(ILI9341_POSC, 4, ILI9341_POSC_DATA);          // retired in v.1.02

    static uint8_t ILI9341_DTCA_DATA[] = {0x85, 0x00, 0x78};
    sendCmdData(ILI9341_DTCA, 3, ILI9341_DTCA_DATA);          // retired in v.1.02

    static uint8_t ILI9341_PWCTRA_DATA[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
    sendCmdData(ILI9341_PWCTRA, 5, ILI9341_PWCTRA_DATA);      // retired in v.1.02
  }

  sendCmdData(ILI9341_PUMPRC,   0x20);

  if (ILI9341_VERSION < 3) {  // < v1.2
    static uint8_t ILI9341_DTCB_DATA[] = {0x00, 0x00};
    sendCmdData(ILI9341_DTCB, 2, ILI9341_DTCB_DATA);          // retired in v.1.02
  }

  if (ILI9341_VERSION < 3) {  // < v1.2
    sendCmdData(ILI9341_PWCTR1, 0x23);             // Power control VRH[5:0]
  }
  else {
    sendCmdData(ILI9341_MADCTL13, 0x21);           // xCRICxCC
  }

  sendCmdData(ILI9341_PWCTR2,   0x10);             // Power control SAP[2:0];BT[3:0]

  static uint8_t ILI9341_VCOMCTR_DATA[] = {0x31, 0x3c};
  sendCmdData(ILI9341_VCOMCTR1, 2, ILI9341_VCOMCTR_DATA);       // VCM control 1

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

  static uint8_t ILI9341_FRMCTR1_DATA[] = {0x00, 0x18};
  sendCmdData(ILI9341_FRMCTR1, 2, ILI9341_FRMCTR1_DATA);

  static uint8_t ILI9341_DFUNCTR_DATA[] = {0x08, 0x82, 0x27};
  sendCmdData(ILI9341_DFUNCTR, 3, ILI9341_DFUNCTR_DATA);  // Display Function Control

  sendCmdData(ILI9341_ENABLE3G, 0x00);             // 3Gamma Function Disable   // retired in v.1.02

  sendCmdData(ILI9341_GAMMASET, 0x01);             // Gamma curve selected

  if (ILI9341_VERSION < 3) {  // < v1.2
//  static uint8_t ILI9341_GMCTRP1_DATA[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00}; // original
    static uint8_t ILI9341_GMCTRP1_DATA[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0x21, 0x26, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
    sendCmdData(ILI9341_GMCTRP1, 15, ILI9341_GMCTRP1_DATA);  // positive gamma correction

//  static uint8_t ILI9341_GMCTRN1_DATA[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F}; // original
    static uint8_t ILI9341_GMCTRN1_DATA[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
    sendCmdData(ILI9341_GMCTRN1, 15, ILI9341_GMCTRN1_DATA);  // negative gamma correction
  }
  else {
    static uint8_t ILI9341_GMCTRP1_DATA_V13[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x30, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
    sendCmdData(ILI9341_GMCTRP1, 15, ILI9341_GMCTRP1_DATA_V13);  // positive gamma correction v1.3

    static uint8_t ILI9341_GMCTRN1_DATA_V13[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x11, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
    sendCmdData(ILI9341_GMCTRN1, 15, ILI9341_GMCTRN1_DATA_V13);  // negative gamma correction
  }

  sendCmd(ILI9341_SLPOUT);   // Exit Sleep
  delay(150);

  sendCmd(ILI9341_DISPON);   // Display on
  delay(150);

  _spi->endTransaction();   // restore speed
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
    _width = WIDTH;
    _height = HEIGHT;
    break;
  case 1:
    m = 0x20 | (BGR << 3); // MV
    g = 0x02 | (BGR << 5);
    _width = HEIGHT;
    _height = WIDTH;
    break;
  case 2:
    m = 0x80 | (BGR << 3); // MY
    g = 0x08 | (BGR << 5);
    _width = WIDTH;
    _height = HEIGHT;
    break;
  case 3:
    m = 0xe0 | (BGR << 3); // MX | MY | MV
    g = 0x0b | (BGR << 5);
    _width = HEIGHT;
    _height = WIDTH;
    break;
  }
  if (ILI9341_VERSION < 3) { // < v1.2
    sendCmdData(ILI9341_MADCTL, m);
  }
  else {
    sendCmdData(ILI9341_MADCTL13, g);
  }
}

/**************************************************************************/
/*!
   @brief   Enable/Disable display color inversion
    @param   invert True to invert, False to have normal color
*/
/**************************************************************************/
void TSD_ILI9341::invertDisplay(bool invert)
{
  sendCmd(invert ? ILI9341_INVON : ILI9341_INVOFF);
}

/**************************************************************************/
/*!
   @brief    Fill the screen completely with one color. Update in subclasses if
   desired!
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void TSD_ILI9341::fillScreen(const rgb_t color)
{
  clip_t clip{0, 0, _width, _height};
  fillRect(&clip, 0, 0, _width, _height, color);
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
  sendCmdData(ILI9341_VSCRSADD, 2, (uint8_t*)data);
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
  if (top + bottom <= HEIGHT) {
    uint16_t middle = HEIGHT - (top + bottom);
    uint8_t data[6];
    data[0] = top >> 8;
    data[1] = top & 0xff;
    data[2] = middle >> 8;
    data[3] = middle & 0xff;
    data[4] = bottom >> 8;
    data[5] = bottom & 0xff;
    sendCmdData(ILI9341_VSCRDEF, 6, (uint8_t*)data);
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
    sendData(sizeof(buf), (const uint8_t*)buf);
    old_x1 = x1;
    old_x2 = x2;
  }
  if (y1 != old_y1 || y2 != old_y2) {
    sendCmd(ILI9341_PASET); // Row address set
    uint16_t buf[2];
    buf[0] = SWAP16(y1);
    buf[1] = SWAP16(y2);
    sendData(sizeof(buf), (const uint8_t*)buf);
    old_y1 = y1;
    old_y2 = y2;
  }
  sendCmd(ILI9341_RAMWR); // Write to RAM
}

void TSD_ILI9341::sendMDTData(int16_t size, const uint8_t* data)
{
  sendData(size * MDT_SIZE, data);
}



// from MicroPython_ILI9341, mostly modified

void TSD_ILI9341::displayOff()
{
  sendCmd(ILI9341_DISPOFF);
}

void TSD_ILI9341::displayOn()
{
  sendCmd(ILI9341_DISPON);
}

void TSD_ILI9341::drawPixel(clip_t* clip, const int16_t x, const int16_t y, const rgb_t color)
{
  if (x >= clip->x1 && y >= clip->y1 && x < clip->x2 && y < clip->y2) {
    setAddrWindow(x, y, 1, 1);
    uint8_t buf[8];
    sendMDTData(1, mdt_color(buf, color, 1));
  }
}

void TSD_ILI9341::drawFastHLine(clip_t* clip, int16_t x, const int16_t y, int16_t w, const rgb_t color)
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
    sendMDTData(w, buffer_mdt_color(color, w));
  }
}

void TSD_ILI9341::drawFastVLine(clip_t* clip, const int16_t x, int16_t y, int16_t h, const rgb_t color)
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
    sendMDTData(h, buffer_mdt_color(color, h));
  }
}

void TSD_ILI9341::fill_hrect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, const rgb_t color)
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
    int16_t chunk_height = 2048 / w;
    int16_t chunk_count = h / chunk_height;
    int16_t remainder = h % chunk_height;
    int16_t chunk_size = chunk_height * w;
    int16_t chunk_y = y;
    if (chunk_count) {
      const uint8_t* buf = buffer_mdt_color(color, chunk_size);
      for (int16_t i = 0; i < chunk_count; ++i) {
        setAddrWindow(x, chunk_y, w, chunk_height);
        sendMDTData(chunk_size, buf);
        chunk_y += chunk_height;
      }
    }
    if (remainder) {
      setAddrWindow(x, chunk_y, w, remainder);
      sendMDTData(remainder * w, buffer_mdt_color(color, remainder * w));
    }
  }
}

void TSD_ILI9341::fill_vrect(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, const rgb_t color)
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
    int16_t chunk_width = 2048 / h;
    int16_t chunk_count = w / chunk_width;
    int16_t remainder = w % chunk_width;
    int16_t chunk_size = chunk_width * h;
    int16_t chunk_x = x;
    if (chunk_count) {
      const uint8_t* buf = buffer_mdt_color(color, chunk_size);
      for (int16_t i = 0; i < chunk_count; ++i) {
        setAddrWindow(chunk_x, y, chunk_width, h);
        sendMDTData(chunk_size, buf);
        chunk_x += chunk_width;
      }
    }
    if (remainder) {
      setAddrWindow(chunk_x, y, remainder, h);
      sendMDTData(remainder * h, buffer_mdt_color(color, remainder * h));
    }
  }
}

void TSD_ILI9341::fillRect(clip_t* clip, const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color)
{
  if (w > h) {
    fill_hrect(clip, x, y, w, h, color);
  }
  else {
    fill_vrect(clip, x, y, w, h, color);
  }
}
