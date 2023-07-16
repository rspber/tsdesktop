/*
  SnakeStampable

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
  Printable.h - Interface class that allows printing of complex types
  Copyright (c) 2011 Adrian McEwen.  All right reserved.

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

#include <stdlib.h>

class SnakeStamp;

/** The Printable class provides a way for new classes to allow themselves to be printed.
    By deriving from Printable and implementing the printTo method, it will then be possible
    for users to print out instances of this class by passing them into the usual
    Print::print and Print::println methods.
*/

class SnakeStampable
{
  public:
    virtual size_t stampTo(SnakeStamp& p) const = 0;
};
