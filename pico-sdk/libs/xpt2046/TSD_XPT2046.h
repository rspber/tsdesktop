/*
  Touch Screen handler for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

  More information in TSD_XPT2046.cpp

*/

#pragma once

#include <Picoino.h>

#define Z_THRESHOLD 350 // Touch pressure threshold for validating touches

class TSD_XPT2046 {
public:
  TSD_XPT2046() {}

  void setRotation(uint8_t n) { rotation = n % 4; }
  int8_t getRotation() { return rotation; }

  bool begin(PicoSPI* spi, const int16_t tirq = -1);

  // Get raw x,y ADC values from touch controller
  bool getTouchRaw(int16_t* x, int16_t* y);

  // Get raw z (i.e. pressure) ADC value from touch controller
  int16_t getTouchRawZ(void);

  // Get the screen touch coordinates, returns true if screen has been touched
  // if the touch coordinates are off screen then x and y are not updated
  // The returned value can be treated as a bool type, false or 0 means touch not detected
  // In future the function may return an 8 "quality" (jitter) value.
  bool getTouch(int16_t* x, int16_t* y, uint16_t threshold = Z_THRESHOLD);

private:
  // Private function to validate a touch, allow settle time and reduce spurious coordinates
  bool validTouch(int16_t* x, int16_t* y, uint16_t threshold = Z_THRESHOLD);

private:
  PicoSPI* _spi;
  int16_t _tirq;
  int _pressTime;
  int8_t rotation = 1;
};
