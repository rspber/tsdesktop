/*
  PCF8575 for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <Picoino.h>

// deviceAddress base = 0x20 + depends on address bits
#define PCF8575_ADDRESS 0x20

class TSD_PCF8575 : public PicoI2C {
public:
  TSD_PCF8575(i2c_inst_t* i2c = i2c0, const uint8_t deviceAddress = PCF8575_ADDRESS)
    : PicoI2C(deviceAddress, i2c) {}

  const bool begin(const uint16_t out16bits = 0xFFFF);
  const bool read16(uint16_t& out16bits);
  const bool write16(const uint16_t out16bits);
};
