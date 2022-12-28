
#pragma once

#include "hardware/i2c.h"

class TwoWire {
public:
  TwoWire(const uint8_t addr, i2c_inst_t* i2c = i2c0)
  {
    _i2c = i2c;
    _addr = addr;
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
  uint8_t _addr;
};
