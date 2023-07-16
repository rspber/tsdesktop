/*
  RP2040 TFT GPIO 8BITP

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#if defined(TFT_GPIO_8BITP_WRITE) || defined(TFT_GPIO_8BITP_READ)

  #include "RP2040_TFT_GPIO_8BITP.h"
  #include <api/Common.h>

  #define TFT_CASET       0x2A    // Column address set
  #define TFT_PASET       0x2B    // Page address set
  #define TFT_RAMWR       0x2C    // Memory write
  #define TFT_RAMRD       0x2E    // Memory read
  #define TFT_IDXRD       0xD9    // undocumented

  // global variables

  uint32_t gpio_out = -1; // bus high default
  uint32_t gpio_oe = -1;  // all pins output dafault

void tft_hardReset(const int16_t RST);

void rp2040_gpio_8bitp_initBus()
{
#ifdef TFT_8BITP_CS
  if (TFT_8BITP_CS >= 0) {
    pinMode(TFT_8BITP_CS, OUTPUT);
    GPIO_SET(TFT_8BITP_CS);
  }
#endif
#ifdef TFT_8BITP_DC
  if (TFT_8BITP_DC >= 0) {
    pinMode(TFT_8BITP_DC, OUTPUT);
    GPIO_SET(TFT_8BITP_DC);
  }
#endif
#ifdef TFT_8BITP_WR
  if (TFT_8BITP_WR >= 0) {
    pinMode(TFT_8BITP_WR, OUTPUT);
    GPIO_SET(TFT_8BITP_WR);
  }
#endif
#ifdef TFT_8BITP_RD
  if (TFT_8BITP_RD >= 0) {
    pinMode(TFT_8BITP_RD, OUTPUT);
    GPIO_SET(TFT_8BITP_RD);
  }
#endif
#ifdef TFT_8BITP_RST
  if (TFT_8BITP_RST >= 0) {
    pinMode(TFT_8BITP_RST, OUTPUT);
    GPIO_SET(TFT_8BITP_RST);
    digitalWrite(TFT_8BITP_RST, HIGH);
  }
  tft_hardReset(TFT_8BITP_RST);
#endif

  heyah_gpio();

// init data pins
  {
    uint8_t pin = TFT_8BITP_D0;
    for (int i = 0; i < 8; ++i) {
      pinMode(pin++, OUTPUT);
    }
  }
}

#endif



// write

#if defined(TFT_GPIO_8BITP_WRITE)

  const char* tft_identification() { return "RP2040 GPIO 8BITP"; }

void tft_write_begin()
{
  rp2040_gpio_8bitp_initBus();
}

void tft_startWriteCmd()
{
  DATA_BUS_HIGH;
  GPIO_CS_L;
}

void tft_sendCmd(const uint8_t cmd)
{
  GPIO_DC_C;
  GPIO_CS_H;
  GPIO_CS_L;
  GPIO_SEND_8(cmd);
  GPIO_DC_D;
}

void tft_sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t len)
{
  GPIO_DC_C;
  GPIO_CS_H;
  GPIO_CS_L;
  GPIO_SEND_8(cmd);
  GPIO_DC_D;

  for (int i = 0; i < len; ++i) {
    GPIO_SEND_8(data[i]);
  }
}

void tft_writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  GPIO_DC_C;
  GPIO_SEND_8(TFT_CASET);
  GPIO_DC_D;
  GPIO_SEND_16(x);
  GPIO_SEND_16(x + w - 1);
  GPIO_DC_C;
  GPIO_SEND_8(TFT_PASET);
  GPIO_DC_D;
  GPIO_SEND_16(y);
  GPIO_SEND_16(y + h - 1);
  GPIO_DC_C;
  GPIO_SEND_8(TFT_RAMWR);
  GPIO_DC_D;
}

void tft_sendMDTColor(const mdt_t c, int32_t len)
{
  while (--len >= 0) {
#if defined(COLOR_565)
    GPIO_SEND_16(c);
#else
    GPIO_SEND_24(c);
#endif
  }
}

void tft_sendMDTBuffer16(const uint8_t* p, int32_t len)
{
  while (--len >= 0) {
    GPIO_SEND_8(*p++);
    GPIO_SEND_8(*p++);
  }
}

void tft_sendMDTBuffer24(const uint8_t* p, int32_t len)
{
  while (--len >= 0) {
    GPIO_SEND_8(*p++);
    GPIO_SEND_8(*p++);
    GPIO_SEND_8(*p++);
  }
}

#endif




// read

#if defined(TFT_GPIO_8BITP_READ)

#ifdef TFT_PIO_8BITP_WRITE
  void rp2040_pio_8bitp_enable(bool state);
#endif

void tft_read_begin()
{
#ifndef TFT_GPIO_8BITP_WRITE
  rp2040_gpio_8bitp_initBus();
#endif
}

void tft_startReading()
{
#ifdef TFT_PIO_8BITP_WRITE
  rp2040_pio_8bitp_enable(false);
#endif
  GPIO_CS_L;
}

void tft_endReading()
{
  setBUSWrite();
  GPIO_CS_H;
#ifdef TFT_PIO_8BITP_WRITE
  rp2040_pio_8bitp_enable(true);
#endif
}

void tft_readAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  GPIO_DC_C;
  GPIO_SEND_8(TFT_CASET);
  GPIO_DC_D;
  GPIO_SEND_16(x);
  GPIO_SEND_16(x + w - 1);
  GPIO_DC_C;
  GPIO_SEND_8(TFT_PASET);
  GPIO_DC_D;
  GPIO_SEND_16(y);
  GPIO_SEND_16(y + h - 1);
  GPIO_DC_C;
  GPIO_SEND_8(TFT_RAMRD);
  GPIO_DC_D;
  setBUSRead();
}

const uint8_t tft_transfer(const uint8_t cmd)
{
  GPIO_RD_L;
  // falling edge, the receiver puts the data on the bus
  GPIO_RD_H;

// with OVERLAID causes spontaneous black reading errors
// the solution is sometimes to do desktop.draw(true)

  // in pico-sdk the speed is much faster ???
  // some delay is needed, but error color reading stays
//  GPIO_RD_H;
//  GPIO_RD_H;
//  GPIO_RD_H;

  uint32_t d = whatsUp_gpio();
  return d >> TFT_8BITP_D0;
}

const uint16_t tft_transfer16(const uint8_t cmd)
{
  uint8_t b0 = tft_transfer(cmd);
  uint8_t b1 = tft_transfer(cmd);
  return (b0 << 8) | b1;
}

/**************************************************************************/
/*!
    @brief  Read len * 8 bits of data from ILI9341 register.
       This is highly undocumented, it's really a hack but kinda works?
    @param    buf  The result, first byte is the reg, rest is a data read
    @param    reg  The command register to read data from
    @param    len  The number of bytes to read from register
 */
/***********************+***************************************************/
void tft_readRegister(uint8_t* buf, const uint8_t reg, int8_t len)
{
  tft_startReading();
  if (reg) {
    GPIO_DC_C;
    GPIO_CS_H;
    GPIO_CS_L;
    GPIO_SEND_8(TFT_IDXRD);
    GPIO_DC_D;
    GPIO_SEND_8(0x10 + len);
  }
  int i = 0;
  buf[i++] = reg;
  GPIO_DC_C;
  GPIO_CS_H;
  GPIO_CS_L;
  GPIO_SEND_8(reg);
  GPIO_DC_D;
  setBUSRead();
//  delay(1);
  while (--len >= 0) {
    buf[i++] = tft_transfer(0);
  }
  tft_endReading();
}

#endif
