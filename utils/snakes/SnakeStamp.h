/*
  SnakeStamp

  Copyright (c) 2023, rspber (https://github.com/rspber)

  These are hidden Print and String classes from Arduino.

  Reasons why it was necessary to do this class:

  1. The Arduino Print class does not inherit from anything,
     which is necessary in this solution.

  2. The Arduino String class refers to: avr/pgmspace.h and __FlashStringHelper
     which requires redefinition.

  3. The competing String class collides with the String class carried by Arduino.h
     and causes an error: String class is ambiguous.

  4. In order not to create a rival Print class, its name has also been changed.

  5. In the future, it is also planned to change the names of the methods:
       print to streak,
       println to squiggle
     with:
       #define print streak
       #define println squiggle
     so that the any possible artificial intelligence built into the compiler
     does not realize that it is a hidden Print class, and finally String class.

     originally notes below:
*/

/*
  Print.h - Base class that provides print() and println()
  Copyright (c) 2008 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <inttypes.h>
#include <stdio.h> // for size_t

#include "Snakes.h"
#include "SnakeStampable.h"

#include <SnakeStampAncestor.h>

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2

class SnakeStamp : public SnakeStampAncestor
{
  private:
    int write_error;
    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t);
  protected:
    void setWriteError(int err = 1) { write_error = err; }
  public:
    SnakeStamp() : SnakeStampAncestor(), write_error(0) {}
  
    int getWriteError() { return write_error; }
    void clearWriteError() { setWriteError(0); }
  
    virtual size_t write(uint8_t) = 0;
    size_t write(const char *str) {
      if (str == NULL) return 0;
      return write((const uint8_t *)str, strlen(str));
    }
    virtual size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *buffer, size_t size) {
      return write((const uint8_t *)buffer, size);
    }

    // default to zero, meaning "a single write may block"
    // should be overriden by subclasses with buffering
    virtual int availableForWrite() { return 0; }

//    size_t print(const __FlashStringHelper *);
    size_t print(const Snake &);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);
    size_t print(const SnakeStampable&);

//    size_t println(const __FlashStringHelper *);
    size_t println(const Snake &s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(const SnakeStampable&);
    size_t println(void);

    virtual void flush() { /* Empty implementation for backward compatibility */ }
};
