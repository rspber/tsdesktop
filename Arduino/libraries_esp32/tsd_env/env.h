/*
  env

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#pragma once

#include <stddef.h>
#include <inttypes.h>
#include <esp32-hal-psram.h>
#include <stdlib_noniso.h>

// can't define max() / min() because of conflicts with C++
#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))
#define _abs(x) ((x)>0?(x):-(x))  // abs() comes from STL
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define _round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))  // round() comes from STL
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

unsigned long micros();
unsigned long millis();
void delay(uint32_t);
void delayMicroseconds(uint32_t us);

#ifdef __cplusplus

long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

#endif

extern "C" {

// yield
void yield(void);

}

#include <algorithm>
#include <cmath>

// Use float-compatible stl abs() and round(), we don't use Arduino macros to avoid issues with the C++ libraries
using std::abs;
using std::isinf;
using std::isnan;
using std::max;
using std::min;
using std::round;

#define PROGMEM

#undef pgm_read_byte
#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))

#undef pgm_read_word
#define pgm_read_word(addr) ({ typeof(addr) _addr = (addr); *(const unsigned short *)(_addr); })

#undef pgm_read_dword
#define pgm_read_dword(addr) ({ typeof(addr) _addr = (addr); *(const unsigned long *)(_addr); })
