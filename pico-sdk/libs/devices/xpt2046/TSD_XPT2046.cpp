/*
  Touch Screen handler for TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)


  Code comes from https://github.com/Bodmer/TFT_eSPI/Extensions:
  /Touch.cpp, /Touch.h and was stripped down to essential functions

  Originally notes below:
*/

/*
// The following touch screen support code by maxpautsch was merged 1/10/17
// https://github.com/maxpautsch

// Define TOUCH_CS is the user setup file to enable this code

// A demo is provided in examples Generic folder

// Additions by Bodmer to double sample, use Z value to improve detection reliability
// and to correct rotation handling

// See license in root directory.

Bodmer's licence.txt in file /licenses/Bodmer_license.txt
*/

#include <Setup.h>

#if defined(TOUCH_SPI) || defined(TOUCH_PIO_SPI)

#include "TSD_XPT2046.h"
//#include "SerialUSB.h"

#include <TOUCH.hh>
#include <Arduino.h>

#define startReading touch_startReading
#define endReading   touch_endReading
#define transfer     touch_transfer
#define transfer16   touch_transfer16

bool TSD_XPT2046::begin()
{
  touch_begin();

  tirq = -1;

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
bool TSD_XPT2046::getTouchRaw(int16_t& x, int16_t& y)
{
  startReading();

#if defined(TOUCH_PIO_SPI)
  transfer16(0xd1);
  transfer16(0xd1);
  transfer16(0xd1);
  y = transfer16(0xd1) >> 3;

  transfer16(0x91);
  transfer16(0x91);
  transfer16(0x91);
  x = transfer16(0x90) >> 3;
#else
  int16_t tmp;

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

  y = tmp;

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

  x = tmp;
#endif

  endReading();

  return true;
}

/***************************************************************************************
** Function name:           getTouchRawZ
** Description:             read raw pressure on touchpad and return Z value.
***************************************************************************************/
int16_t TSD_XPT2046::getTouchRawZ(void)
{
  startReading();

#if defined(TOUCH_PIO_SPI)
  int16_t z1 = transfer16(0xb0) >> 3;  // Z1
  int16_t z2 = transfer16(0xc0) >> 3;  // Z2
#else
  transfer(0xb0);               // Start new Z1 conversion
  int16_t z1 = transfer16(0xc0) >> 3;  // Read Z1 and start Z2 conversion
  int16_t z2 = transfer16(0x00) >> 3;  // Read Z2
#endif

  endReading();

  // Z sample request
  int16_t tz = 0xFFF + z1 -z2;

  if (tz == 4095) tz = 0;

  return tz;
}

/***************************************************************************************
** Function name:           validTouch
** Description:             read validated position. Return false if not pressed.
***************************************************************************************/
#define _RAWERR 20 // Deadband error allowed in successive position samples

inline uint16_t int16abs(int16_t x) { return x >= 0 ? x : -x; }

bool TSD_XPT2046::validTouch(int16_t& x, int16_t& y, uint16_t threshold)
{
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

  getTouchRaw(x_tmp, y_tmp);

  //  Serial.print("Sample 1 x,y = "); Serial.print(x_tmp);Serial.print(",");Serial.print(y_tmp);
  //  Serial.print(", Z = ");Serial.println(z1);

  delay(1); // Small delay to the next sample
  if (getTouchRawZ() <= threshold) return false;

  delay(2); // Small delay to the next sample
  getTouchRaw(x_tmp2, y_tmp2);

  //  Serial.print("Sample 2 x,y = "); Serial.print(x_tmp2);Serial.print(",");Serial.println(y_tmp2);
  //  Serial.print("Sample difference = ");Serial.print(abs(x_tmp - x_tmp2));Serial.print(",");Serial.println(abs(y_tmp - y_tmp2));

  if (int16abs(x_tmp - x_tmp2) > _RAWERR) return false;
  if (int16abs(y_tmp - y_tmp2) > _RAWERR) return false;

  x = x_tmp;
  y = y_tmp;

  return true;
}

/***************************************************************************************
** Function name:           getTouch
** Description:             read callibrated position. Return false if not pressed.
***************************************************************************************/
bool TSD_XPT2046::IamHere(int16_t& x, int16_t& y, uint16_t threshold)
{
  int16_t x_tmp, y_tmp;

  if (threshold < 20) threshold = 20;
  if (_pressTime > (int)millis()) threshold = 20;

  int8_t n = 5;
  int8_t valid = 0;

  while (n--)
  {
    if (validTouch(x_tmp, y_tmp, threshold)) valid++;;
  }

  if (valid < 1) { _pressTime = 0; return false; }

  _pressTime = millis() + 50;

  x = x_tmp;
  y = y_tmp;
  return valid > 0;
}

#endif
