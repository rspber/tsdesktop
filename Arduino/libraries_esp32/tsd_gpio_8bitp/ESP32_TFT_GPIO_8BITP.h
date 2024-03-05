/*
  ESP32 TFT GPIO 8BITP

  based on

  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_ESP32.h
  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_ESP32.c

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#pragma once

#if defined(TFT_GPIO_8BITP_WRITE)

  #include <rgb.h>

// write / read

  #define GPIO_32_PIN_LOW( PIN) GPIO.out1_w1tc.val = (1 << (PIN - 32))
  #define GPIO_32_PIN_HIGH(PIN) GPIO.out1_w1ts.val = (1 << (PIN - 32))

  #define GPIO_00_PIN_LOW( PIN) GPIO.out_w1tc = (1 << PIN)
  #define GPIO_00_PIN_HIGH(PIN) GPIO.out_w1ts = (1 << PIN)

// write

  void tft_write_begin();

  const char* tft_identification();

  void tft_startWriteCmd();
  void tft_sendCmd(const uint8_t cmd);
  void tft_sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t len);

  void tft_startWrite();
  void tft_endWrite();

  void tft_writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h);

  #define tft_startWriteColor()

  void tft_writeMDTColor(mdt_t c);

  #define tft_endWriteColor()

  void tft_sendMDTColor(const mdt_t c, int32_t len);
  void tft_sendMDTBuffer16(const uint8_t* buffer, int32_t len);
  void tft_sendMDTBuffer24(const uint8_t* buffer, int32_t len);

#endif

// read

#if defined(TFT_GPIO_8BITP_READ)

  void tft_read_begin();

  void tft_startReading();
  void tft_endReading();

  void tft_readAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h);

  const uint8_t tft_transfer(const uint8_t cmd);
  const uint16_t tft_transfer16(const uint8_t cmd);

  void tft_readRegister(uint8_t* buf, const uint8_t reg, int8_t len);

#endif
