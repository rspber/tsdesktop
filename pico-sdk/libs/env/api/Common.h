/*
  Interface to pico-sdk

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

*/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <cstring>

#include <pico/stdio.h>
#include <pico/printf.h>
#include <pico/time.h>

#define yield()

typedef enum {
  LOW     = 0,
  HIGH    = 1,
  CHANGE  = 2,
  FALLING = 3,
  RISING  = 4,
} PinStatus;

typedef enum {
  INPUT           = 0x0,
  OUTPUT          = 0x1,
  INPUT_PULLUP    = 0x2,
  INPUT_PULLDOWN  = 0x3,
  OUTPUT_2MA      = 0x4,
  OUTPUT_4MA      = 0x5,
  OUTPUT_8MA      = 0x6,
  OUTPUT_12MA     = 0x7,
} PinMode;

typedef enum {
  LSBFIRST = 0,
  MSBFIRST = 1,
} BitOrder;

//void debug(const char *buf);

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

#define bit(b) (1UL << (b))

typedef bool boolean;
typedef uint8_t byte;
typedef uint16_t word;

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))

// ---------------------------------- Math ------------------------------------

void randomSeed(unsigned long seed);
long random(long howbig);
long random(long howsmall, long howbig);

long inline map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (long)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// ------------------------------- Interrupts ---------------------------------

uint32_t noInterrupts(void);
void interrupts(uint32_t);

// ---------------------------------- GPIO ------------------------------------

void pinMode(const int16_t pin, const uint8_t mode);
void digitalWrite(const int16_t pin, uint8_t data);
bool digitalRead(uint8_t pin);
int16_t analogRead(uint8_t pin);      // 0 .. 1023
int16_t analogRead12(uint8_t pin);    // 0 .. 4095

// ---------------------------------- time ------------------------------------

unsigned long millis(void);
unsigned long micros(void);

void delay(unsigned long ms);

