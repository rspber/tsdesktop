/*
  RP2040 TFT SPI

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

*/

#pragma once

#if defined(TFT_SPI_WRITE) || defined(TFT_SPI_READ)

  #include <rgb.h>

#endif

#if defined(TFT_SPI_WRITE)

  void rp2040_spi_wr_enable(const bool enable);
  void tft_write_begin();

  const char* tft_identification();

  void tft_startWriteCmd();
  void tft_sendCmd(const uint8_t cmd);
  void tft_sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size);

  void tft_startWrite();
  void tft_endWrite();
  void tft_writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h);

  void tft_sendMDTColor(const mdt_t c);
  void tft_sendMDTColor(const mdt_t c, int32_t len);
  void tft_sendMDTBuffer16(const uint8_t* buffer, int32_t len);
  void tft_sendMDTBuffer24(const uint8_t* buffer, int32_t len);

#endif



#if defined(TFT_SPI_READ)

  void tft_read_begin();

  void tft_startReading();
  void tft_endReading();

  void tft_readAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h);

  #define tft_setBUSWriteMode()
  #define tft_setBUSReadMode()

  const uint8_t tft_transfer(const uint8_t cmd);
  const uint16_t tft_transfer16(const uint8_t cmd);

#endif
