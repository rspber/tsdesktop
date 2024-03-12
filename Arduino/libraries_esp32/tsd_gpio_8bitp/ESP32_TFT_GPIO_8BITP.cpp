/*
  ESP32 TFT GPIO 8BITP

  Copyright (c) 2024, rspber (https://github.com/rspber)

  based on

  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_ESP32.h
  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_ESP32.c

*/

#include <Setup.h>

#if defined(TFT_GPIO_8BITP_WRITE) || defined(TFT_GPIO_8BITP_READ)

#include "ESP32_TFT_GPIO_8BITP.h"
#include <Arduino.h>

  #define TFT_CASET       0x2A    // Column address set
  #define TFT_PASET       0x2B    // Page address set
  #define TFT_RAMWR       0x2C    // Memory write
  #define TFT_RAMRD       0x2E    // Memory read
  #define TFT_IDXRD       0xD9    // undocumented

   // Data/Command pin
  #if (TFT_8BITP_DC >= 32)
    #define GPIO_DC_C    GPIO_32_PIN_LOW(TFT_8BITP_DC)
    #define GPIO_DC_D    GPIO_32_PIN_HIGH(TFT_8BITP_DC)
  #elif (TFT_8BITP_DC >= 0)
    #define GPIO_DC_C    GPIO_00_PIN_LOW(TFT_8BITP_DC)
    #define GPIO_DC_D    GPIO_00_PIN_HIGH(TFT_8BITP_DC)
  #else
    #define GPIO_DC_C
    #define GPIO_DC_D
  #endif

   // ChipSelect pin
  #if (TFT_8BITP_CS >= 32)
    #define GPIO_CS_L    GPIO_32_PIN_LOW(TFT_8BITP_CS)
    #define GPIO_CS_H    GPIO_32_PIN_HIGH(TFT_8BITP_CS)
  #elif (TFT_8BITP_CS >= 0)
    #define GPIO_CS_L    GPIO_00_PIN_LOW(TFT_8BITP_CS)
    #define GPIO_CS_H    GPIO_00_PIN_HIGH(TFT_8BITP_CS)
  #else
    #define GPIO_CS_L
    #define GPIO_CS_H
  #endif

   // Write pin
  #if (TFT_8BITP_WR >= 32)
    // Note: it will be ~1.25x faster if the TFT_8BITP_WR pin uses a GPIO pin lower than 32
    #define GPIO_WR_L    GPIO_32_PIN_LOW(TFT_8BITP_WR)
    #define GPIO_WR_H    GPIO_32_PIN_HIGH(TFT_8BITP_WR)
  #elif (TFT_8BITP_WR >= 0)
    #define GPIO_WR_L    GPIO_00_PIN_LOW(TFT_8BITP_WR)
    #define GPIO_WR_H    GPIO_00_PIN_HIGH(TFT_8BITP_WR)
  #else
    #define GPIO_WR_L
    #define GPIO_WR_H
  #endif

  // Read pin
  #if (TFT_8BITP_RD >= 32)
    #define GPIO_RD_L    GPIO_32_PIN_LOW(TFT_8BITP_RD)
    #define GPIO_RD_H    GPIO_32_PIN_HIGH(TFT_8BITP_RD)
  #elif (TFT_8BITP_RD >= 0)
    #define GPIO_RD_L    GPIO_00_PIN_LOW(TFT_8BITP_RD)
    //#define RD_L digitalWrite(TFT_8BITP_WR, LOW)
    #define GPIO_RD_H    GPIO_00_PIN_LOW(TFT_8BITP_RD)
    //#define RD_H digitalWrite(TFT_8BITP_WR, HIGH)
  #else
    #define GPIO_RD_L
    #define GPIO_RD_H
  #endif

  // Lookup table for ESP32 parallel bus interface uses 1KB RAM,
  uint32_t xset_mask[256];

  // Create a bit set lookup table for data bus - wastes 1kbyte of RAM but speeds things up dramatically
  // can then use e.g. GPIO.out_w1ts = set_mask(0xFF); to set data bus to 0xFF
void esp32_gpio_8bitp_init_mask()
{
  for (int c = 0; c < 256; c++) {
    xset_mask[c] = 0;
    if ( c & 0x01 ) xset_mask[c] |= (1 << TFT_8BITP_D0);
    if ( c & 0x02 ) xset_mask[c] |= (1 << TFT_8BITP_D1);
    if ( c & 0x04 ) xset_mask[c] |= (1 << TFT_8BITP_D2);
    if ( c & 0x08 ) xset_mask[c] |= (1 << TFT_8BITP_D3);
    if ( c & 0x10 ) xset_mask[c] |= (1 << TFT_8BITP_D4);
    if ( c & 0x20 ) xset_mask[c] |= (1 << TFT_8BITP_D5);
    if ( c & 0x40 ) xset_mask[c] |= (1 << TFT_8BITP_D6);
    if ( c & 0x80 ) xset_mask[c] |= (1 << TFT_8BITP_D7);
  }
}

  // Mask for the 8 data bits to set pin directions
  #define GPIO_DIR_MASK \
    (1 << TFT_8BITP_D0) | \
    (1 << TFT_8BITP_D1) | \
    (1 << TFT_8BITP_D2) | \
    (1 << TFT_8BITP_D3) | \
    (1 << TFT_8BITP_D4) | \
    (1 << TFT_8BITP_D5) | \
    (1 << TFT_8BITP_D6) | \
    (1 << TFT_8BITP_D7)

  #define DATA_BUS_HIGH GPIO.out_w1ts = GPIO_DIR_MASK

  #if (TFT_8BITP_WR >= 32)
    // Data bits and the write line are cleared sequentially
    #define DATA_BUS_LOW_WR_L GPIO.out_w1tc = GPIO_DIR_MASK; GPIO_WR_L
  #elif (TFT_8BITP_WR >= 0)
    // Data bits and the write line are cleared to 0 in one step (1.25x faster)
    #define DATA_BUS_LOW_WR_L GPIO.out_w1tc = GPIO_DIR_MASK | (1 << TFT_8BITP_WR)
  #else
    #define DATA_BUS_LOW_WR_L
  #endif

  // A lookup table is used to set the different bit patterns, this uses 1kByte of RAM
//  #define set_mask(C) xset_mask[C] // 63fps Sprite rendering test 33% faster, graphicstest only 1.8% faster than shifting in real time
  #define GPIO_SEND_8(C) DATA_BUS_LOW_WR_L; GPIO.out_w1ts = xset_mask[(uint8_t)(C)]; GPIO_WR_H

void esp32_gpio_8bitp_busDir(const uint8_t mode)
{
  pinMode(TFT_8BITP_D0, mode);
  pinMode(TFT_8BITP_D1, mode);
  pinMode(TFT_8BITP_D2, mode);
  pinMode(TFT_8BITP_D3, mode);
  pinMode(TFT_8BITP_D4, mode);
  pinMode(TFT_8BITP_D5, mode);
  pinMode(TFT_8BITP_D6, mode);
  pinMode(TFT_8BITP_D7, mode);
}

void tft_setBUSWriteMode()
{
  esp32_gpio_8bitp_busDir(OUTPUT);
}

void tft_setBUSReadMode()
{
  esp32_gpio_8bitp_busDir(INPUT);
}

extern void initPin(const int16_t pin, PinMode mode);
extern void tft_hardReset(const int16_t RST);

void esp32_gpio_8bitp_initBus()
{
#ifdef TFT_8BITP_CS
  initPin(TFT_8BITP_CS, OUTPUT);
#endif
#ifdef TFT_8BITP_DC
  initPin(TFT_8BITP_DC, OUTPUT);
#endif
#ifdef TFT_8BITP_WR
  initPin(TFT_8BITP_WR, OUTPUT);
#endif
#ifdef TFT_8BITP_RD
  initPin(TFT_8BITP_RD, OUTPUT);
#endif
#ifdef TFT_8BITP_RST
  initPin(TFT_8BITP_RST, OUTPUT);
  tft_hardReset(TFT_8BITP_RST);
#endif

  tft_setBUSWriteMode();
}

inline void GPIO_SEND_16(uint16_t d)
{
  GPIO_SEND_8(d >> 8);
  GPIO_SEND_8(d);
}

inline void GPIO_SEND_24(uint32_t d)
{
  GPIO_SEND_8(d >> 16);
  GPIO_SEND_8(d >> 8);
  GPIO_SEND_8(d);
}

void tft_startWrite()
{
  DATA_BUS_HIGH;
  GPIO_CS_L;
}

void tft_endWrite()
{
  DATA_BUS_HIGH;
  GPIO_CS_H;
}

#endif



// write

#if defined(TFT_GPIO_8BITP_WRITE)

  const char* tft_identification() { return "ESP32 GPIO 8BITP"; }

void tft_write_begin()
{
  esp32_gpio_8bitp_init_mask();
  esp32_gpio_8bitp_initBus();
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

void tft_sendMDTColor(const mdt_t c)
{
  #if defined(COLOR_565)
    #if defined (SSD1963_DRIVER)
      // Write 18 bit color to TFT
      GPIO_SEND_8((c & 0xF800) >> 8);
      GPIO_SEND_8((c & 0x07E0) >> 3);
      GPIO_SEND_8((c & 0x001F) << 3);
    #else
      GPIO_SEND_16(c);
    #endif
  #else
    GPIO_SEND_24(c);
  #endif
}

void tft_sendMDTColor(const mdt_t c, int32_t len)
{
  while (--len >= 0) {
  #if defined(COLOR_565)
    #if defined (SSD1963_DRIVER)
      // Write 18 bit color to TFT
      GPIO_SEND_8((c & 0xF800) >> 8);
      GPIO_SEND_8((c & 0x07E0) >> 3);
      GPIO_SEND_8((c & 0x001F) << 3);
    #else
      GPIO_SEND_16(c);
    #endif
  #else
    GPIO_SEND_24(c);
  #endif
  }
}

void tft_sendMDTBuffer16(const uint8_t* p, int32_t len)
{
  while (--len >= 0) {
  #if defined (SSD1963_DRIVER)
    tft_sendMDTColor((*p++ >> 8) | *p++);
  #else
    GPIO_SEND_8(*p++);
    GPIO_SEND_8(*p++);
  #endif
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

void tft_read_begin()
{
}

void tft_startReading()
{
  GPIO_CS_L;
}

void tft_endReading()
{
  tft_setBUSWriteMode();
  GPIO_CS_H;
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
}

const uint8_t tft_transfer(const uint8_t cmd)
{
  GPIO_RD_L;
  // falling edge, the receiver puts the data on the bus
  uint32_t reg;           // Read all GPIO pins 0-31
  reg = gpio_input_get(); // Read three times to allow for bus access time
  reg = gpio_input_get();
  reg = gpio_input_get(); // Data should be stable now
  GPIO_RD_H;

  uint8_t b = 0;
  // Check GPIO bits used and build value
  b |= (((reg>>TFT_8BITP_D0)&1) << 0);
  b |= (((reg>>TFT_8BITP_D1)&1) << 1);
  b |= (((reg>>TFT_8BITP_D2)&1) << 2);
  b |= (((reg>>TFT_8BITP_D3)&1) << 3);
  b |= (((reg>>TFT_8BITP_D4)&1) << 4);
  b |= (((reg>>TFT_8BITP_D5)&1) << 5);
  b |= (((reg>>TFT_8BITP_D6)&1) << 6);
  b |= (((reg>>TFT_8BITP_D7)&1) << 7);

  return b;
}

const uint16_t tft_transfer16(const uint8_t cmd)
{
  uint8_t b0 = tft_transfer(cmd);
  uint8_t b1 = tft_transfer(cmd);
  return (b0 << 8) | b1;
}

#endif
