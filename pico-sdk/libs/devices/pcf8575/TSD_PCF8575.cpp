/*
  PCF8575 for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "TSD_PCF8575.h"

const bool TSD_PCF8575::begin(const uint16_t out16bits)
{
  TwoWire::begin();
  return write16(out16bits);
}

const bool TSD_PCF8575::read16(uint16_t& in16bits)
{
  TwoWire::read((uint8_t*)&in16bits, 2);
  return true;  // TODO
}

const bool TSD_PCF8575::write16(const uint16_t out16bits)
{
  TwoWire::write16(out16bits);
  return true;  // TODO
}
