/*
  Wire for pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "Wire.h"

void TwoWire::read(uint8_t* dst, const int16_t len, const bool nostop)
{
  i2c_read_blocking(_i2c, _addr, dst, len, nostop);
}

const uint8_t TwoWire::read8(const bool nostop)
{
  uint8_t b;
  i2c_read_blocking(_i2c, _addr, &b, 1, nostop);
  return b;    // endian
}

const uint16_t TwoWire::read16(const bool nostop)
{
  uint8_t b[2];
  i2c_read_blocking(_i2c, _addr, b, 2, nostop);
  return (b[1] << 8) | b[0];    // endian
}

void TwoWire::write(const uint8_t* src, const int16_t len, const bool nostop)
{
  i2c_write_blocking(_i2c, _addr, src, len, nostop);
}

void TwoWire::write8(const uint8_t endian, const bool nostop)
{
  i2c_write_blocking(_i2c, _addr, &endian, 1, nostop);
}

void TwoWire::write8write8(const uint8_t reg, const uint8_t value, const bool nostop)
{
  write16(value << 8 | reg, nostop);
}

void TwoWire::write16(const uint16_t endian, const bool nostop)
{
  i2c_write_blocking(_i2c, _addr, (uint8_t*)&endian, 2, nostop);
}

void TwoWire::write8read(const uint8_t reg, uint8_t* dst, const int16_t len)
{
  write8(reg, true);
  read(dst, len);
}

const uint8_t TwoWire::write8read8(const uint8_t reg)
{
  write8(reg, true);
  return read8();
}
