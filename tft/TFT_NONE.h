/*
  NO protocol

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#if defined(TFT_NO_WRITE)

  #define tft_write_begin()
  #define tft_identification() "NONE"
  #define tft_startWriteCmd()
  #define tft_sendCmd(cmd)
  #define tft_sendCmdData(cmd, data, len)
  #define tft_startWrite()
  #define tft_endWrite()
  #define tft_writeAddrWindow(x, y, w, h)
  #define tft_startWriteColor()
  #define tft_writeMDTColor(c)
  #define tft_endWriteColor()
  #define tft_sendMDTColor(c, len)
  #define tft_sendMDTBuffer16(buffer, len)
  #define tft_sendMDTBuffer24(buffer, len)

#endif

#if defined(TFT_NO_READ)

  #define tft_read_begin()
  #define tft_startReading()
  #define tft_endReading()
  #define tft_readAddrWindow(x, y, w, h)
  inline const uint8_t tft_transfer(const uint8_t cmd) { return 0; }
  inline const uint8_t tft_transfer16(const uint8_t cmd) { return 0; }
  #define tft_readRegister(buf, reg, len)

#endif
