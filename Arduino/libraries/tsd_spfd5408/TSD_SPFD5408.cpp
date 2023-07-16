/*
  RP2040 RESISTIVE TOUCH pinned into 8BITP pins

  Copyright (c) 2023, rspber (https://github.com/rspber)

  Based on:
  https://github.com/adafruit/Adafruit_TouchScreen/TouchScreen.cpp

  License in file LimorFried_license.txt

  Originally notes below:
*/

/*
// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License
*/

#include <Setup.h>

#if defined(TOUCH_RESISTIVE)

#include "TSD_SPFD5408.h"
#include <api/Common.h>
//#include "SerialUSB.h"

#ifdef ARDUINO
  #define analogRead12(pin) (analogRead(pin) << 2)
#endif

// increase or decrease the touchscreen oversampling. This is a little different
// than you make think: 1 is no oversampling, whatever data we get is
// immediately returned 2 is double-sampling and we only return valid data if
// both points are the same 3+ uses insert sort to get the median value. We
// found 2 is precise yet not too slow so we suggest sticking with it!

#define NUMSAMPLES 5

static void start_read_x()
{
  pinMode(TOUCH_Y_PLUS, INPUT);
  pinMode(TOUCH_Y_MINUS, INPUT);
  digitalWrite(TOUCH_Y_PLUS, LOW);
  digitalWrite(TOUCH_Y_MINUS, LOW);

  pinMode(TOUCH_X_PLUS, OUTPUT);
  digitalWrite(TOUCH_X_PLUS, HIGH);
  pinMode(TOUCH_X_MINUS, OUTPUT);
  digitalWrite(TOUCH_X_MINUS, LOW);
}
/*
int readTouchX(void)
{
  start_read_x();
  return (4095 - analogRead12(TOUCH_Y_PLUS));
}
*/

static void start_read_y()
{
  pinMode(TOUCH_X_PLUS, INPUT);
  pinMode(TOUCH_X_MINUS, INPUT);
  digitalWrite(TOUCH_X_PLUS, LOW);
  digitalWrite(TOUCH_X_MINUS, LOW);

  pinMode(TOUCH_Y_PLUS, OUTPUT);
  digitalWrite(TOUCH_Y_PLUS, HIGH);
  pinMode(TOUCH_Y_MINUS, OUTPUT);
  digitalWrite(TOUCH_Y_MINUS, LOW);
}
/*
int readTouchY(void)
{
  start_read_y();
  return (4095 - analogRead12(TOUCH_X_MINUS));
}
*/

static void start_read_z()
{
  pinMode(TOUCH_X_PLUS, OUTPUT);
  digitalWrite(TOUCH_X_PLUS, LOW);

  pinMode(TOUCH_Y_MINUS, OUTPUT);
  digitalWrite(TOUCH_Y_MINUS, HIGH);

  digitalWrite(TOUCH_X_MINUS, LOW);
  pinMode(TOUCH_X_MINUS, INPUT);
  digitalWrite(TOUCH_Y_PLUS, LOW);
  pinMode(TOUCH_Y_PLUS, INPUT);
}

static int16_t readTouchZ(const int16_t x)
{
  start_read_z();
  int z1 = analogRead12(TOUCH_X_MINUS);
  int z2 = analogRead12(TOUCH_Y_PLUS);

  if (TOUCH_RX != 0) {
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= x;
    rtouch /= 4096;
    rtouch *= TOUCH_RX;
    rtouch /= 4096;

    return rtouch;
  } else {
    return (4095 - (z2 - z1));
  }
}
/*
int preasure(void)
{
  return readTouchZ(readTouchX());
}
*/

void TSD_SPFD5408::begin()
{
}

int16_t read_coord(const int16_t pin)
{
  int samples[NUMSAMPLES];

  for (int i = 0; i < NUMSAMPLES; i++) {
    samples[i] = analogRead12(pin);
  }

  // sort
  for (int j = NUMSAMPLES; --j >= 1; ) {
    for (int i = j; --i >= 0; ) {
      if (samples[i] > samples[j]) {
        int16_t tmp = samples[i];
        samples[i] = samples[j];
        samples[j] = tmp;
      }
    }
  }
  return 4095 - samples[NUMSAMPLES / 2];
}

bool TSD_SPFD5408::IamHere(int16_t& x, int16_t& y)
{
  start_read_x();
  x = read_coord(TOUCH_Y_PLUS);

  start_read_y();
  y = read_coord(TOUCH_X_MINUS);

  int16_t z = readTouchZ(x);

//  Serial.printf("%d, %d, %d\n", xtmp, ytmp, ztmp);

  pinMode(TOUCH_X_PLUS, OUTPUT);
  pinMode(TOUCH_X_MINUS, OUTPUT);
  pinMode(TOUCH_Y_PLUS, OUTPUT);
  pinMode(TOUCH_Y_MINUS, OUTPUT);

  return z >= 0 && z <= 1;
}

#endif
