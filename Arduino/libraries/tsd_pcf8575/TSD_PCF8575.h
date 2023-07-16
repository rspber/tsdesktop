/*
  PCF8575 for TSDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include "Wire.h"
#include "Setup.h"

// deviceAddress base = 0x20 + depends on address bits
#define PCF8575_ADDRESS 0x20

class TSD_PCF8575 {
public:
  TSD_PCF8575(const uint8_t i2c_nr = 0, const uint8_t deviceAddress = PCF8575_ADDRESS)
  {
    _i2c_nr = i2c_nr;
    _addr = deviceAddress;
  }
  const bool begin(const uint16_t out16bits = 0xFFFF);
  const bool isConnected();
  const bool read16(uint16_t& out16bits);
  const bool write16(const uint16_t out16bits);
private:
  TwoWire* _wire;
  uint8_t _i2c_nr;
  uint8_t _addr;
};
