/*
  RP2040 TFT GPIO 8BITP

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

*/

/*
  rp2040:\> Couldn't a second processor take care of it ?
*/

#pragma once

#if defined(TFT_GPIO_8BITP_WRITE) || defined(TFT_GPIO_8BITP_READ)

  #include "hardware/gpio.h"
  #include <rgb.h>

  extern uint32_t gpio_out;

  inline void heyah_gpio() { sio_hw->gpio_out = gpio_out; }

  inline void GPIO_CLR(const uint8_t pin)
  {
    gpio_out &= ~(1ul << pin);
  }

  inline void GPIO_SET(const uint8_t pin)
  {
    gpio_out |= (1ul << pin);
  }

#if defined(TFT_8BITP_CS) && (TFT_8BITP_CS >= 0)
  #define GPIO_CS_L GPIO_CLR(TFT_8BITP_CS); heyah_gpio()
  #define GPIO_CS_H GPIO_SET(TFT_8BITP_CS); heyah_gpio()
#else
  #define GPIO_CS_L
  #define GPIO_CS_H
#endif

#if defined(TFT_8BITP_DC) && (TFT_8BITP_DC >= 0)
  #define GPIO_DC_C GPIO_CLR(TFT_8BITP_DC); heyah_gpio()
  #define GPIO_DC_D GPIO_SET(TFT_8BITP_DC); heyah_gpio()
#else
  #define GPIO_DC_C
  #define GPIO_DC_D
#endif

#if defined(TFT_8BITP_WR) && (TFT_8BITP_WR >= 0)
  #define GPIO_WR_L GPIO_CLR(TFT_8BITP_WR); heyah_gpio()
  #define GPIO_WR_H GPIO_SET(TFT_8BITP_WR); heyah_gpio()
#else
  #define GPIO_WR_L
  #define GPIO_WR_H
#endif

  #define DATA_BUS_HIGH gpio_out |= (0xff << TFT_8BITP_D0)

  inline void GPIO_SEND_8(uint8_t d)
  {
    GPIO_WR_L;
    gpio_out &= ~(0xff << TFT_8BITP_D0);
    gpio_out |= (d << TFT_8BITP_D0);
    heyah_gpio();
    GPIO_WR_H;    // rising edge, receiver reads the data
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

#endif



// write

#if defined(TFT_GPIO_8BITP_WRITE)

  void tft_write_begin();

  const char* tft_identification();

  void tft_startWriteCmd();
  void tft_sendCmd(const uint8_t cmd);
  void tft_sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t len);

  inline void tft_startWrite()
  {
    DATA_BUS_HIGH;
    GPIO_CS_L;
  }

  inline void tft_endWrite()
  {
    DATA_BUS_HIGH;
    GPIO_CS_H;
  }

  void tft_writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h);

  void tft_sendMDTColor(const mdt_t c);
  void tft_sendMDTColor(const mdt_t c, int32_t len);
  void tft_sendMDTBuffer16(const uint8_t* buffer, int32_t len);
  void tft_sendMDTBuffer24(const uint8_t* buffer, int32_t len);

#endif



// read

#if defined(TFT_GPIO_8BITP_READ)

#if defined(TFT_8BITP_RD) && (TFT_8BITP_RD >= 0)
  #define GPIO_RD_L GPIO_CLR(TFT_8BITP_RD); heyah_gpio()
  #define GPIO_RD_H GPIO_SET(TFT_8BITP_RD); heyah_gpio()
#else
  #define GPIO_RD_L
  #define GPIO_RD_H
#endif

  void tft_read_begin();

  void tft_startReading();
  void tft_endReading();

  void tft_readAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h);

  extern uint32_t gpio_oe;

  inline void tft_setBUSWriteMode()
  {
    gpio_oe |= (0xff << TFT_8BITP_D0);
    sio_hw->gpio_oe = gpio_oe;
  }

  inline void tft_setBUSReadMode()
  {
    gpio_oe &= ~(0xff << TFT_8BITP_D0);
    sio_hw->gpio_oe = gpio_oe;
  }

  inline uint32_t whatsUp_gpio() { return sio_hw->gpio_in; }

  const uint8_t tft_transfer(const uint8_t cmd);
  const uint16_t tft_transfer16(const uint8_t cmd);

#endif
