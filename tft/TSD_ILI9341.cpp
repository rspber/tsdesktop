/*
  ILI9341 driver for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

1. This is a modified Adafruit's ILI9341 driver: Adafruit_ILI9341

differences:
- Adafruit_SPITFT is missed, all spi functions are abstract
- added support for ILI9341 devices v1.3

2. chapter 2 further

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

/*
2. writeFillRectGradient: based on https://github.com/PaulStoffregen/ILI9341_t3

Original license below.
*/

/***************************************************
  This is our library for the Adafruit  ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "ILI9341_Regs.h"
#include "TSD_ILI9341.h"

#include <Setup.h>

#define BGR 1

#define SETUP_SPEED 2 * 1000 * 1000   // 2 MHz

#define beginTransaction(Hz) spi->spiBegin(Hz)
#define endTransaction() spi->spiEnd()
#define startSending() spi->startSending()
#define send(data) spi->send(data)
#define endSending() spi->endSending()
#define sendCmd(cmd) spi->sendCmd(cmd)
#define sendData(data, size) spi->sendData(data, size)
#define sendCmdData(cmd, data, size) spi->sendCmdData(cmd, data, size)
#define sendCmdByte(cmd, data) spi->sendCmdByte(cmd, data)
#define startTransferring() spi->startTransferring()
#define transfer(cmd) spi->transfer(cmd)
#define endTransferring() spi->endTransferring()

void TSD_ILI9341::hardReset()
{
  if (RST >= 0) {
    digitalWrite(RST, LOW);
    sleep_ms(50);
    digitalWrite(RST, HIGH);
    sleep_ms(50);
  }
}

void TSD_ILI9341::reset()
{
  hardReset();
  beginTransaction(SETUP_SPEED);   // slow down
  sendCmd(ILI9341_SWRESET); // Engage software reset
  endTransaction();
  sleep_ms(150);
}

void TSD_ILI9341::begin(TFT_SPI* aspi, const int16_t aRST)
{
  spi = aspi;
  spi->begin();

  RST = aRST;
  if (RST >= 0) {
    pinMode(RST, OUTPUT);
  }

  hardReset();

  beginTransaction(SETUP_SPEED);

  sendCmd(ILI9341_SWRESET); // Engage software reset
  sleep_ms(150);

#if ILI9341_VERSION < 3  // < v1.2
    sendCmdData(ILI9341_EF, (uint8_t*)"\x03\x80\x02", 3);              // unknown
    sendCmdData(ILI9341_PWCTRB, (uint8_t*)"\x00\xC1\x30", 3);      // retired in v.1.02
    sendCmdData(ILI9341_POSC, (uint8_t*)"\x64\x03\x12\x81", 4);          // retired in v.1.02
    sendCmdData(ILI9341_DTCA, (uint8_t*)"\x85\x00\x78", 3);          // retired in v.1.02
    sendCmdData(ILI9341_PWCTRA, (uint8_t*)"\x39\x2C\x00\x34\x02", 5);      // retired in v.1.02
#endif
  sendCmdByte(ILI9341_PUMPRC,   0x20);

#if ILI9341_VERSION < 3  // < v1.2
    sendCmdData(ILI9341_DTCB, (uint8_t*)"\x00\x00", 2);          // retired in v.1.02
#endif
#if ILI9341_VERSION < 3  // < v1.2
    sendCmdByte(ILI9341_PWCTR1, 0x23);             // Power control VRH[5:0]
#else
    sendCmdByte(ILI9341_MADCTL13, 0x21);           // xCRICxCC
#endif
  sendCmdByte(ILI9341_PWCTR2,   0x10);             // Power control SAP[2:0];BT[3:0]
  sendCmdData(ILI9341_VCOMCTR1, (uint8_t*)"\x31\x3c", 2);       // VCM control 1
  sendCmdByte(ILI9341_VCOMCTR2, 0xC0);             // VCM control 2
#if ILI9341_VERSION < 3  // < v1.2
    sendCmdByte(ILI9341_MADCTL,   0x48);             // Memory Access Control
#endif
  sendCmdByte(ILI9341_VSCRSADD, 0x00);             // Vertical scroll zero
#ifdef COLOR_565
  sendCmdByte(ILI9341_PIXFMT,   0x55);
#else
  sendCmdByte(ILI9341_PIXFMT,   0x66);
#endif
  sendCmdData(ILI9341_FRMCTR1, (uint8_t*)"\x00\x18", 2);
  sendCmdData(ILI9341_DFUNCTR, (uint8_t*)"\x08\x82\x27", 3);  // Display Function Control
  sendCmdByte(ILI9341_ENABLE3G, 0x00);             // 3Gamma Function Disable   // retired in v.1.02
  sendCmdByte(ILI9341_GAMMASET, 0x01);             // Gamma curve selected
#if ILI9341_VERSION < 3  // < v1.2
                                         //"\x0F\x31\x2B\x0C\x0E\x08\x4E\xF1\x37\x07\x10\x03\x0E\x09\x00" // original
    sendCmdData(ILI9341_GMCTRP1, (uint8_t*)"\x0F\x31\x2B\x0C\x0E\x08\x4E\x21\x26\x07\x10\x03\x0E\x09\x00", 15);  // positive gamma correction
                                         //"\x00\x0E\x14\x03\x11\x07\x31\xC1\x48\x08\x0F\x0C\x31\x36\x0F" // original
    sendCmdData(ILI9341_GMCTRN1, (uint8_t*)"\x00\x0E\x14\x03\x11\x07\x31\xC1\x48\x08\x0F\x0C\x31\x36\x0F", 15);  // negative gamma correction
#else
    sendCmdData(ILI9341_GMCTRP1, (uint8_t*)"\x0F\x31\x2B\x0C\x0E\x08\x30\xF1\x37\x07\x10\x03\x0E\x09\x00", 15);  // positive gamma correction v1.3
    sendCmdData(ILI9341_GMCTRN1, (uint8_t*)"\x00\x0E\x14\x03\x11\x07\x11\xC1\x48\x08\x0F\x0C\x31\x36\x0F", 15);  // negative gamma correction
#endif

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
    @brief  Read len * 8 bits of data from ILI9341 register.
       This is highly undocumented, it's really a hack but kinda works?
    @param    buf  The result, first byte is the reg, rest is a data read
    @param    reg  The command register to read data from
    @param    len  The number of bytes to read from register
 */
/**************************************************************************/
void TSD_ILI9341::readRegister(uint8_t* buf, const uint8_t reg, int8_t len)
{
  beginTransaction(SETUP_SPEED);
  sendCmdByte(ILI9341_RDBYIDX, 0x10 + len);
  int i = 0;
  buf[i++] = reg;
  sendCmd(reg);
  while (--len >= 0) {
    buf[i++] = transfer(0);
  }
  endTransaction();
}

/**************************************************************************/
/*!
   @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void TSD_ILI9341::setRotation(const int8_t rotation)
{
# if ILI9341_VERSION < 3 // < v1.2
  uint8_t m = 0;
  switch (rotation % 4) { // can't be higher than 3
  case 0:
    m = 0x40 | (BGR << 3); // MX
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 1:
    m = 0x20 | (BGR << 3); // MV
    setSize(getHEIGHT(), getWIDTH());
    break;
  case 2:
    m = 0x80 | (BGR << 3); // MY
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 3:
    m = 0xe0 | (BGR << 3); // MX | MY | MV
    setSize(getHEIGHT(), getWIDTH());
    break;
  }
  beginTransaction(SETUP_SPEED);
    sendCmdByte(ILI9341_MADCTL, m);
  endTransaction();
#else
  uint8_t g = 0; // v1.3
  switch (rotation % 4) { // can't be higher than 3
  case 0:
    g = 0x01 | (BGR << 5);
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 1:
    g = 0x02 | (BGR << 5);
    setSize(getHEIGHT(), getWIDTH());
    break;
  case 2:
    g = 0x08 | (BGR << 5);
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 3:
    g = 0x0b | (BGR << 5);
    setSize(getHEIGHT(), getWIDTH());
    break;
  }
  beginTransaction(SETUP_SPEED);
    sendCmdByte(ILI9341_MADCTL13, g);
  endTransaction();
#endif
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
  beginTransaction(SETUP_SPEED);
  sendCmd(ILI9341_VSCRSADD);
  startSending();
  send(y >> 8);
  send(y & 0xff);
  endSending();
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
    beginTransaction(SETUP_SPEED);
    sendCmd(ILI9341_VSCRDEF);
    startSending();
    send(top >> 8);
    send(top & 0xff);
    send(middle >> 8);
    send(middle & 0xff);
    send(bottom >> 8);
    send(bottom & 0xff);
    endSending();
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
void TSD_ILI9341::setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h)
{
  beginTransaction(SETUP_SPEED);
  writeAddrWindow(x, y, w, h);
  endTransaction();
}

void TSD_ILI9341::sendCmd2x16(const uint8_t cmd, const int16_t i1, const int16_t i2)
{
  sendCmd(cmd);
  startSending();
  send(i1 >> 8);
  send(i1 & 0xFF);
  send(i2 >> 8);
  send(i2 & 0xFF);
  endSending();
}

void TSD_ILI9341::writeAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h)
{
  sendCmd2x16(ILI9341_CASET, x, x + w - 1);
  sendCmd2x16(ILI9341_PASET, y, y + h - 1);
  sendCmd(ILI9341_RAMWR);
}

void TSD_ILI9341::readAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h)
{
  sendCmd2x16(ILI9341_CASET, x, x + w - 1);
  sendCmd2x16(ILI9341_PASET, y, y + h - 1);
  sendCmd(ILI9341_RAMRD);
}

rgb_t TSD_ILI9341::readPixel(clip_t* clip, int16_t x, int16_t y)
{
  if (x >= clip->x1 && y >= clip->y1 && x < clip->x2 && y < clip->y2) {
    beginTransaction(TFT_SPI_READ_SPEED);
    readAddrWindow(x, y, 1, 1);
    startTransferring();
    transfer(0);  // the first is thorough
    // Read the 3 RGB bytes, color is in the top 6 bits of each byte
    uint8_t r = transfer(0);
    uint8_t g = transfer(0);
    uint8_t b = transfer(0);
    endTransferring();
    endTransaction();
    return RGB(r,g,b);
  }
  return 0;
}

void TSD_ILI9341::storePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, over_t* t)
{
  endTransaction();
  beginTransaction(TFT_SPI_READ_SPEED);
  readAddrWindow(x, y, w, h);
  startTransferring();
  transfer(0);  // the first is thorough
  for (int i = w * h; --i >= 0; ) {
    // Read the 3 RGB bytes, color is in the top 6 bits of each byte
    uint8_t r = transfer(0);
    uint8_t g = transfer(0);
    uint8_t b = transfer(0);
    if (t->len + 3 > t->size) {
      t->size += 100;
      t->buf = (uint8_t*)realloc(t->buf, t->size);
    }
    if (MDT_SIZE > 2) {
      t->buf[t->len++] = r;
      t->buf[t->len++] = g;
      t->buf[t->len++] = b;
    }
    else {
      uint16_t c = RGB565(r,g,b);
      t->buf[t->len++] = c >> 8;
      t->buf[t->len++] = c & 0xff;
    }
  }
  endTransferring();
  endTransaction();
  beginTransaction(TFT_SPI_WRITE_SPEED);
}

void TSD_ILI9341::writeColor(const int16_t w, const int16_t h, const rgb_t color)
{
  uint8_t buf[8];
  mdt_color(buf, color, 1);
  startSending();
  for (int j = 0; j < h; ++j) {
    for (int i = 0; i < w; ++i) {
      for (int k = 0; k < MDT_SIZE; ++k) {
        send(buf[k]);
      }
    }
  }
  endSending();
}

void TSD_ILI9341::writePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color)
{
  // very dubious method to detect pointer in rgb_t type
  // in rp2040 pointers are 4 byte:
  // code: 1x xx xx xx
  // data: 2x xx xx xx
  // RGB(r,g,b) adds 0xFF000000 to color definition
  if ((color & 0xFF000000) != 0xFF000000) {	// not a RGB color
    over_t* t = (over_t*)color;
    if (t->mode == 1) {   // store underlaing pixels
      storePixels(x, y, w, h, t);
    }
    if (t->mode == 2) {   // restore background from buf
      writeAddrWindow(x, y, w, h);
      startSending();
      for (int i = w * h * MDT_SIZE; --i >= 0; ) {
        send(t->buf[t->len++]);
      }
      endSending();
      return;
    }
    writeAddrWindow(x, y, w, h);
    writeColor(w, h, t->color);
  }
  else {
    writeAddrWindow(x, y, w, h);
    writeColor(w, h, color);
  }
}

void TSD_ILI9341::writePixel(clip_t* clip, int16_t x, int16_t y, const rgb_t color)
{
  if (x >= clip->x1 && y >= clip->y1 && x < clip->x2 && y < clip->y2) {
    writePixels(x, y, 1, 1, color);
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
    writePixels(x, y, w, 1, color);
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
    writePixels(x, y, 1, h, color);
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
    writePixels(x, y, w, h, color);
  }
}

// color565toRGB14		- converts 16 bit 565 format color to 14 bit RGB (2 bits clear for math and sign)
// returns 00rrrrr000000000,00gggggg00000000,00bbbbb000000000
// thus not overloading sign, and allowing up to double for additions for fixed point delta
static void RGB14fromColor(rgb_t color, int16_t &r, int16_t &g, int16_t &b)
{
//#ifdef COLOR_565
//  r = (color >> 2) & 0x3E00;
//  g = (color << 3) & 0x3F00;
//  b = (color << 9) & 0x3E00;
//#else
  r = (color >> 10) & 0x3F00;
  g = (color >>  2) & 0x3F00;
  b = (color <<  6) & 0x3F00;
//#endif
}

// RGB14tocolor565		- converts 14 bit RGB back to 16 bit 565 format color
static rgb_t RGB14toColor(int16_t r, int16_t g, int16_t b)
{
//#ifdef COLOR_565
//  return (((r & 0x3E00) << 2) | ((g & 0x3F00) >> 3) | ((b & 0x3E00) >> 9));
//#else
  return (((r & 0x3F00) << 10) | ((g & 0x3F00) << 2) | ((b & 0x3F00) >> 6));
//#endif
}

void TSD_ILI9341::writeFillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z)
{
  int16_t r1, g1, b1;
  RGB14fromColor(z->color1, r1, g1, b1);
  int16_t r2, g2, b2;
  RGB14fromColor(z->color2, r2, g2, b2);
  if (z->deg == 2) {
    writeAddrWindow(x, y, w, h);
    startSending();
  }
  int16_t r = r1;
  int16_t g = g1;
  int16_t b = b1;
  int8_t prc = z->percent;
  if (prc < 0) {
    prc = 0;
  }
  if (prc > 100) {
    prc = 100;
  }
  for (int j = 0; j < h; ++j) {

    if (z->deg == 4) {
      writeAddrWindow(x, y + h - 1 - j, w, 1);
      startSending();
    }

    uint8_t buf[8];
    mdt_color(buf, RGB14toColor(r, g, b), 1);
    for (int i = 0; i < w; ++i) {
      for (int k = 0; k < MDT_SIZE; ++k) {
        send(buf[k]);
      }
    }

    if (z->deg == 4) {
      endSending();
    }

    int adj = prc - 50;
    if (adj > 45)
      adj = 45;
    else
    if (adj < -45)
      adj = -45;

    int16_t p = prc + (50 - prc + adj) * j * 2 / h;
    int16_t dr = (int)(r2 - r1) * 50 / (p * h);
    int16_t dg = (int)(g2 - g1) * 50 / (p * h);
    int16_t db = (int)(b2 - b1) * 50 / (p * h);
    int q;
    q = r + dr;
    if ((r2 >= r1 && q <= r2 && q >= r1) || (r1 >= r2 && q <= r1 && q >= r2)) r = q; else
      q = 0;
    q = g + dg;
    if ((g2 >= g1 && q <= g2 && q >= g1) || (g1 >= g2 && q <= g1 && q >= g2)) g = q; else
      q = 0;
    q = b + db;
    if ((b2 >= b1 && q <= b2 && q >= b1) || (b1 >= b2 && q <= b1 && q >= b2)) b = q; else
      q = 0;
  }
  if (z->deg == 2) {
    endSending();
  }
}

void TSD_ILI9341::writeFillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z)
{
  int16_t r1, g1, b1;
  RGB14fromColor(z->color1, r1, g1, b1);
  int16_t r2, g2, b2;
  RGB14fromColor(z->color2, r2, g2, b2);
  writeAddrWindow(x, y, w, h);
  startSending();
  int8_t prc = z->percent;
  if (prc < 0) {
    prc = 0;
  }
  if (prc > 100) {
    prc = 100;
  }

  uint8_t* tmp;
  if (z->deg == 3) {
    tmp = (uint8_t*)malloc(w * MDT_SIZE);
  }

  for (int j = 0; j < h; ++j) {
    int16_t r = r1;
    int16_t g = g1;
    int16_t b = b1;
    for (int i = 0; i < w; ++i) {
      uint8_t buf[8];
      mdt_color(buf, RGB14toColor(r, g, b), 1);
      for (int k = 0; k < MDT_SIZE; ++k) {
        if (z->deg == 3) {
          tmp[(w - 1 - i) * MDT_SIZE + k] = buf[k];
        }
        else {
          send(buf[k]);
        }
      }
      int adj = prc - 50;
      if (adj > 45)
        adj = 45;
      else
      if (adj < -45)
        adj = -45;

      int16_t p = prc + (50 - prc + adj) * i * 2 / w;
      int16_t dr = (int)(r2 - r1) * 50 / (p * w);
      int16_t dg = (int)(g2 - g1) * 50 / (p * w);
      int16_t db = (int)(b2 - b1) * 50 / (p * w);
      int q;
      q = r + dr;
      if ((r2 >= r1 && q <= r2 && q >= r1) || (r1 >= r2 && q <= r1 && q >= r2)) r = q; else
        q = 0;
      q = g + dg;
      if ((g2 >= g1 && q <= g2 && q >= g1) || (g1 >= g2 && q <= g1 && q >= g2)) g = q; else
        q = 0;
      q = b + db;
      if ((b2 >= b1 && q <= b2 && q >= b1) || (b1 >= b2 && q <= b1 && q >= b2)) b = q; else
        q = 0;
    }
    if (z->deg == 3) {
      for (int k = 0; k < w * MDT_SIZE; ++k) {
        send(tmp[k]);
      }
    }
  }

  if (z->deg == 3) {
    free(tmp);
  }

  endSending();
}

void TSD_ILI9341::writeFillRectGradient(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z)
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
    if (z->deg == 1 || z->deg == 3) {
      writeFillRectHGradient(x, y, w, h, z);
    }
    if (z->deg == 2 || z->deg == 4) {
      writeFillRectVGradient(x, y, w, h, z);
    }
  }
}
