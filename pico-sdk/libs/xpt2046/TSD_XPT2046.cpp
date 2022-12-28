/*
  Touch Screen handler for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
Code comes from https://github.com/Bodmer/TFT_eSPI/Extensions:
/Touch.cpp, /Touch.h and was stripped down to essential functions

notes:
- only hardware spi is supported

Original notes below
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvStartvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// The following touch screen support code by maxpautsch was merged 1/10/17
// https://github.com/maxpautsch

// Define TOUCH_CS is the user setup file to enable this code

// A demo is provided in examples Generic folder

// Additions by Bodmer to double sample, use Z value to improve detection reliability
// and to correct rotation handling

// See license in root directory.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^End^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Bodmer's root directory essencials from licence.txt below
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvStartvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Software License Agreement (FreeBSD License)

Copyright (c) 2022 Bodmer (https://github.com/Bodmer)

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^End^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
*/

#include "TSD_XPT2046.h"
#include <Picoino.h>

#define beginTransaction() spi->spiBegin()
#define endTransaction() spi->spiEnd()
#define transfer(cmd) spi->transfer(cmd)
#define transfer16(cmd) spi->transfer16(cmd)

bool TSD_XPT2046::begin(TFT_SPI* aspi, const int16_t atirq)
{
  spi = aspi;
  spi->begin();
  tirq = atirq;

  if (tirq >= 0) {
    //    pinMode( tirqPin, INPUT );
    //    attachInterrupt(digitalPinToInterrupt(tirqPin), isrPin, FALLING);
    //    isrPinptr = this;
  }

  return true;
}


/***************************************************************************************
** Function name:           getTouchRaw
** Description:             read raw touch position.  Always returns true.
***************************************************************************************/
bool TSD_XPT2046::getTouchRaw(int16_t* x, int16_t* y) {
  int16_t tmp;

  beginTransaction();

  // Start YP sample request for y position, read 4 times and keep last sample
  transfer(0xd0);                    // Start new YP conversion
  transfer(0);                       // Read first 8 bits
  transfer(0xd0);                    // Read last 8 bits and start new YP conversion
  transfer(0);                       // Read first 8 bits
  transfer(0xd0);                    // Read last 8 bits and start new YP conversion
  transfer(0);                       // Read first 8 bits
  transfer(0xd0);                    // Read last 8 bits and start new YP conversion

  tmp = transfer(0);                   // Read first 8 bits
  tmp = tmp << 5;
  tmp |= 0x1f & (transfer(0x90) >> 3);   // Read last 8 bits and start new XP conversion

  *y = tmp;

  // Start XP sample request for x position, read 4 times and keep last sample
  transfer(0);                       // Read first 8 bits
  transfer(0x90);                    // Read last 8 bits and start new XP conversion
  transfer(0);                       // Read first 8 bits
  transfer(0x90);                    // Read last 8 bits and start new XP conversion
  transfer(0);                       // Read first 8 bits
  transfer(0x90);                    // Read last 8 bits and start new XP conversion

  tmp = transfer(0);                 // Read first 8 bits
  tmp = tmp << 5;
  tmp |= 0x1f & (transfer(0) >> 3);    // Read last 8 bits

  *x = tmp;

  endTransaction();

  return true;
}

/***************************************************************************************
** Function name:           getTouchRawZ
** Description:             read raw pressure on touchpad and return Z value.
***************************************************************************************/
int16_t TSD_XPT2046::getTouchRawZ(void) {
  beginTransaction();

  // Z sample request
  int16_t tz = 0xFFF;
  transfer(0xb0);               // Start new Z1 conversion
  tz += transfer16(0xc0) >> 3;  // Read Z1 and start Z2 conversion
  tz -= transfer16(0x00) >> 3;  // Read Z2

  endTransaction();

  if (tz == 4095) tz = 0;

  return tz;
}

/***************************************************************************************
** Function name:           validTouch
** Description:             read validated position. Return false if not pressed.
***************************************************************************************/
#define _RAWERR 20 // Deadband error allowed in successive position samples

bool TSD_XPT2046::validTouch(int16_t* x, int16_t* y, uint16_t threshold) {
  int16_t x_tmp, y_tmp, x_tmp2, y_tmp2;

  // Wait until pressure stops increasing to debounce pressure
  int16_t z1 = 1;
  int16_t z2 = 0;
  while (z1 > z2)
  {
    z2 = z1;
    z1 = getTouchRawZ();
    delay(1);
  }

  //  Serial.print("Z = ");Serial.println(z1);

  if (z1 <= threshold) return false;

  getTouchRaw(&x_tmp, &y_tmp);

  //  Serial.print("Sample 1 x,y = "); Serial.print(x_tmp);Serial.print(",");Serial.print(y_tmp);
  //  Serial.print(", Z = ");Serial.println(z1);

  delay(1); // Small delay to the next sample
  if (getTouchRawZ() <= threshold) return false;

  delay(2); // Small delay to the next sample
  getTouchRaw(&x_tmp2, &y_tmp2);

  //  Serial.print("Sample 2 x,y = "); Serial.print(x_tmp2);Serial.print(",");Serial.println(y_tmp2);
  //  Serial.print("Sample difference = ");Serial.print(abs(x_tmp - x_tmp2));Serial.print(",");Serial.println(abs(y_tmp - y_tmp2));

  if (abs(x_tmp - x_tmp2) > _RAWERR) return false;
  if (abs(y_tmp - y_tmp2) > _RAWERR) return false;

  *x = x_tmp;
  *y = y_tmp;

  return true;
}

/***************************************************************************************
** Function name:           getTouch
** Description:             read callibrated position. Return false if not pressed.
***************************************************************************************/
bool TSD_XPT2046::getTouch(int16_t* x, int16_t* y, uint16_t threshold) {
  int16_t x_tmp, y_tmp;

  if (threshold < 20) threshold = 20;
  if (_pressTime > (int)millis()) threshold = 20;

  int8_t n = 5;
  int8_t valid = 0;

  while (n--)
  {
    if (validTouch(&x_tmp, &y_tmp, threshold)) valid++;;
  }

  if (valid < 1) { _pressTime = 0; return false; }

  _pressTime = millis() + 50;

  switch (abs(rotation)) {
  case 0:
    *x = 4095 - y_tmp;
    *y = x_tmp;
    break;
  case 1:
    *x = x_tmp;
    *y = y_tmp;
    break;
  case 2:
    *x = y_tmp;
    *y = 4095 - x_tmp;
    break;
  default: // 3
    *x = 4095 - x_tmp;
    *y = 4095 - y_tmp;
  }
  return valid > 0;
}
