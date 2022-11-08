/*
  PCF8575 for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

  code comes from https://github.com/RobTillaart/PCF8575
*/

/*
MIT License

Copyright (c) 2020-2022 Rob Tillaart

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
Footer
*/

#include "TSD_PCF8575.h"

const bool TSD_PCF8575::begin(const uint16_t out16bits)
{
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
  if (_wire->requestFrom(_addr, 2) == 2) {
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
