/*
  TFT SPI

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <sys/types.h>

class TFT_SPI {
public:
  TFT_SPI() {}

  virtual void begin() {};

  virtual void cs(const uint8_t mode) {}
  virtual void dc(const uint8_t mode) {}

  virtual void spiBegin(const uint Hz)
  {
    cs(0);
  }

  virtual void spiBegin()
  {
    cs(0);
  }

  virtual void spiEnd()
  {
    cs(1);
  }

  virtual void startSending()
  {
  }

  virtual void send(const uint8_t b) = 0;

  virtual void endSending()
  {
  }

  virtual void sendData(const uint8_t data)
  {
    startSending();
    send(data);
    endSending();
  }

  virtual void sendCmd(const uint8_t cmd)
  {
    dc(0);
    sendData(cmd);
    dc(1);
  }

  virtual void sendData(const uint8_t* data, const int16_t size)
  {
    startSending();
    for (int i = 0; i < size; ++i) {
      send(data[i]);
    }
    endSending();
  }

  void sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size)
  {
    sendCmd(cmd);
    if (size > 0) {
      sendData(data, size);
    }
  }

  void sendCmdByte(const uint8_t cmd, const uint8_t data)
  {
    sendCmd(cmd);
    sendData(data);
  }

  virtual void startTransferring()
  {
  }

  virtual const uint8_t transfer(const uint8_t cmd) = 0;
  virtual const uint16_t transfer16(const uint8_t cmd) = 0;

  virtual void endTransferring()
  {
  }

};
