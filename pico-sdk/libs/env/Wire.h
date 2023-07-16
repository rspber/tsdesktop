/*
  Wire for pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <Setup.h>
#include "rp2040_i2c.h"

class TwoWire {
public:
  TwoWire(const uint8_t addr, const uint8_t i2c_nr = 0)
  {
    _i2c_nr = i2c_nr;
    _addr = addr;
  }

  void begin()
  {
    if (_i2c_nr == 0) {
      _i2c = init_i2c(I2C0_SDA_PIN, I2C0_SCL_PIN, _i2c_nr);
    }
    else {
      _i2c = init_i2c(I2C1_SDA_PIN, I2C1_SCL_PIN, _i2c_nr);
    }  
  }

  // I2C read
  void read(uint8_t* dst, const int16_t len, const bool nostop = false);
  const uint8_t read8(const bool nostop = false);
  const uint16_t read16(const bool nostop = false);

  // I2C write
  void write(const uint8_t* src, const int16_t len, const bool nostop = false);
  void write8(const uint8_t endian, const bool nostop = false);
  void write8write8(const uint8_t reg, const uint8_t value, const bool nostop = false);
  void write16(const uint16_t endian, const bool nostop = false);

  // I2C write and read
  void write8read(const uint8_t reg, uint8_t* dst, const int16_t len);
  const uint8_t write8read8(const uint8_t reg);

private:
  i2c_inst_t* _i2c;
  uint8_t _i2c_nr;
  uint8_t _addr;
};
