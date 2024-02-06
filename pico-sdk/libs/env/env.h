/*
  env

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#pragma once

#include <api/Common.h>
#include <api/itoa.h>
#include <avr/dtostrf.h>

#define ARDUINO_ARCH_RP2040

#undef pgm_read_byte
#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))

#undef pgm_read_word
#define pgm_read_word(addr) ({ typeof(addr) _addr = (addr); *(const unsigned short *)(_addr); })

#undef pgm_read_dword
#define pgm_read_dword(addr) ({ typeof(addr) _addr = (addr); *(const unsigned long *)(_addr); })

#ifndef PROGMEM
  #define PROGMEM
#endif
