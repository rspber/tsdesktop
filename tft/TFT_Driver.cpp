/*
  TFT abstract driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include "TFT_Driver.h"
#include <Setup.h>

#define beginTransact(Hz) spi->spiBegin(Hz)
#define endTransact() spi->spiEnd()
#define startSending() spi->startSending()
#define send(data) spi->send(data)
#define endSending() spi->endSending()
#define sendCmd(cmd) spi->sendCmd(cmd)
#define sendData(data, size) spi->sendData(data, size)
#define sendCmdData(cmd, data, size) spi->sendCmdData(cmd, data, size)
#define sendCmdByte(cmd, data) spi->sendCmdByte(cmd, data)
#define startTransfer() spi->startTransfer()
#define transfer(cmd) spi->transfer(cmd)
#define transfer16(cmd) spi->transfer16(cmd)
#define endTransfer() spi->endTransfer()

void TFT_Driver::hardReset()
{
  if (RST >= 0) {
    digitalWrite(RST, LOW);
    sleep_ms(50);
    digitalWrite(RST, HIGH);
    sleep_ms(50);
  }
}

void TFT_Driver::reset()
{
  hardReset();
  beginTransact(TFT_SETUP_SPEED);   // slow down
  sendCmd(TFT_SWRESET); // Engage software reset
  endTransact();
  sleep_ms(150);
}

void TFT_Driver::displayOff()
{
  beginTransact(TFT_SETUP_SPEED);
  sendCmd(TFT_DISPOFF);
  endTransact();
}

void TFT_Driver::displayOn()
{
  beginTransact(TFT_SETUP_SPEED);
  sendCmd(TFT_DISPON);
  endTransact();
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
void TFT_Driver::readRegister(uint8_t* buf, const uint8_t reg, int8_t len)
{
  beginTransact(TFT_SETUP_SPEED);
  sendCmdByte(TFT_IDXRD, 0x10 + len);
  int i = 0;
  buf[i++] = reg;
  sendCmd(reg);
  startTransfer();
  while (--len >= 0) {
    buf[i++] = transfer(0);
  }
  endTransfer();
  endTransact();
}

void TFT_Driver::invertDisplay(bool invert)
{
  beginTransact(TFT_SETUP_SPEED);
  sendCmd(invert ? TFT_INVON : TFT_INVOFF);
  endTransact();
}

// actually not used
void TFT_Driver::scrollTo(int16_t y) {
  beginTransact(TFT_SETUP_SPEED);
  sendCmd(TFT_VSCRSADD);
  startSending();
  send(y >> 8);
  send(y & 0xff);
  endSending();
  endTransact();
}

// actually not used
void TFT_Driver::setScrollMargins(int16_t top, int16_t bottom) {
  // TFA+VSA+BFA must equal 320
  if (top + bottom <= getHEIGHT()) {
    uint16_t middle = getHEIGHT() - (top + bottom);
    beginTransact(TFT_SETUP_SPEED);
    sendCmd(TFT_VSCRDEF);
    startSending();
    send(top >> 8);
    send(top & 0xff);
    send(middle >> 8);
    send(middle & 0xff);
    send(bottom >> 8);
    send(bottom & 0xff);
    endSending();
    endTransact();
  }
}

// actually not used
void TFT_Driver::setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h)
{
  beginTransact(TFT_SETUP_SPEED);
  writeAddrWindow(x, y, w, h);
  endTransact();
}

void TFT_Driver::sendCmd2x16(const uint8_t cmd, const int16_t i1, const int16_t i2)
{
  sendCmd(cmd);
  startSending();
  send(i1 >> 8);
  send(i1 & 0xFF);
  send(i2 >> 8);
  send(i2 & 0xFF);
  endSending();
}

void TFT_Driver::writeAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h)
{
  sendCmd2x16(TFT_CASET, x, x + w - 1);
  sendCmd2x16(TFT_PASET, y, y + h - 1);
  sendCmd(TFT_RAMWR);
}

void TFT_Driver::readAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h)
{
  sendCmd2x16(TFT_CASET, x, x + w - 1);
  sendCmd2x16(TFT_PASET, y, y + h - 1);
  sendCmd(TFT_RAMRD);
}

rgb_t TFT_Driver::readPixel(clip_t* clip, int16_t x, int16_t y)
{
  if (x >= clip->x1 && y >= clip->y1 && x < clip->x2 && y < clip->y2) {
    beginTransact(TFT_SPI_READ_SPEED);
    readAddrWindow(x, y, 1, 1);
    startTransfer();
    transfer(0);  // the first is thorough
    rgb_t color;
#if defined(ST7796)
    if (MDT_SIZE <= 2) {
    // Read 2 byte 565 color
      uint8_t b0 = transfer(0);
      uint8_t b1 = transfer(0);
      color = rgb((b0 << 8) | b1);
    }
    else {
#else
    {
#endif
    // Read the 3 RGB bytes
      uint8_t r = transfer(0);
      uint8_t g = transfer(0);
      uint8_t b = transfer(0);
      color = RGB(r,g,b);
    }
    endTransfer();
    endTransact();
    return color;
  }
  return 0;
}

void TFT_Driver::storePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, over_t* t)
{
  endTransact();
  beginTransact(TFT_SPI_READ_SPEED);
  readAddrWindow(x, y, w, h);
  startTransfer();
  transfer(0);  // the first is thorough
  for (int i = w * h; --i >= 0; ) {
    if (t->len + 3 > t->size) {
      t->size += 100;
      t->buf = (uint8_t*)realloc(t->buf, t->size);
    }
    if (MDT_SIZE > 2) {
      // PIXFMT >= 0x?6
      // Read the 3 RGB bytes
      uint8_t r = transfer(0);
      uint8_t g = transfer(0);
      uint8_t b = transfer(0);
      t->buf[t->len++] = r;
      t->buf[t->len++] = g;
      t->buf[t->len++] = b;
    }
    else {
      uint8_t b0, b1;
#if defined(ST7796)
      // Read 2 byte 565 color
      b0 = transfer(0);
      b1 = transfer(0);
#else
      // Read the 3 RGB bytes
      uint8_t r = transfer(0);
      uint8_t g = transfer(0);
      uint8_t b = transfer(0);
      uint16_t c = RGB565(r,g,b);
      b0 = c >> 8;
      b1 = c & 0xff;
#endif
      t->buf[t->len++] = b0;
      t->buf[t->len++] = b1;
    }
  }
  endTransfer();
  endTransact();
  beginTransact(TFT_SPI_WRITE_SPEED);
}

void TFT_Driver::writeColor(const int16_t w, const int16_t h, const rgb_t color)
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

void TFT_Driver::writePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color)
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

void TFT_Driver::writeFillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z)
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

void TFT_Driver::writeFillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z)
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

void TFT_Driver::writeFillRectGradient(clip_t* clip, int16_t x, int16_t y, int16_t w, int16_t h, gradient_t* z)
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
