/*
  PCF8575 for TSDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

  code comes from https://github.com/RobTillaart/PCF8575

  RobTillaart's license in file /licenses/RobTillaart_license.txt
*/

#include "TSD_PCF8575.h"

TwoWire* init_i2c(const uint8_t i2c_nr);

const bool TSD_PCF8575::begin(const uint16_t out16bits)
{
  _wire = init_i2c(_i2c_nr);

  if (!isConnected()) {
    return false;
  }
  return write16(out16bits);
}

const bool TSD_PCF8575::isConnected()
{
  _wire->beginTransmission(_addr);
  return _wire->endTransmission() == 0;
}

const bool TSD_PCF8575::read16(uint16_t& in16bits)
{
  if (_wire->requestFrom(_addr, (uint8_t)2) == 2) {
    uint8_t b0 = _wire->read();        // low 8 bits
    uint8_t b1 = _wire->read();        // high 8 bits
    in16bits = (b1 << 8) | b0;
    return true;
  }
  else {
    return false;
  }
}

const bool TSD_PCF8575::write16(const uint16_t out16bits)
{
  _wire->beginTransmission(_addr);
  _wire->write(out16bits & 0xFF);    // low 8 bits
  _wire->write(out16bits >> 8);      // high 8 bits
  return _wire->endTransmission() == 0;
}
