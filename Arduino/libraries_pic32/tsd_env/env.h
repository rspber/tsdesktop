/*
  env

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#pragma once

#include <wiring.h>
#include <stdlib_noniso.h>

#ifndef min
#define min(a,b) (a <= b ? a : b)
#endif

#ifndef max
#define max(a,b) (a >= b ? a : b)
#endif

long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

extern "C" {

// yield
void yield(void);

}

#define PROGMEM

#undef pgm_read_byte
#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))

#undef pgm_read_word
#define pgm_read_word(addr) ({ typeof(addr) _addr = (addr); *(const unsigned short *)(_addr); })

#undef pgm_read_dword
#define pgm_read_dword(addr) ({ typeof(addr) _addr = (addr); *(const unsigned long *)(_addr); })
