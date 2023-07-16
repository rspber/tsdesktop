/*
  Snakes

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
  WString.h - String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
  Copyright 2011, Paul Stoffregen, paul@pjrc.com

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

#ifdef __cplusplus

#include <env.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <avr/pgmspace.h>

#define String Snake

// When compiling programs with this class, the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency, typically
// with little or no increase in code size.
//     -felide-constructors
//     -std=c++0x

//class __FlashStringHelper;
//#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

// An inherited class for holding the result of a concatenation.  These
// result objects are assumed to be writable by subsequent concatenations.
class SnakeSumHelper;

// The string class
class Snake
{
	// use a function pointer to allow for "if (s)" without the
	// complications of an operator bool(). for more information, see:
	// http://www.artima.com/cppsource/safebool.html
	typedef void (Snake::*SnakeIfHelperType)() const;
	void SnakeIfHelper() const {}

public:
	// constructors
	// creates a copy of the initial value.
	// if the initial value is null or invalid, or if memory allocation
	// fails, the string will be marked as invalid (i.e. "if (s)" will
	// be false).
	Snake(const char *cstr = "");
	Snake(const Snake &str);
//	Snake(const __FlashStringHelper *str);
	#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
		Snake(Snake &&rval);
		Snake(SnakeSumHelper &&rval);
	#endif
	explicit Snake(char c);
	explicit Snake(unsigned char, unsigned char base=10);
	explicit Snake(int, unsigned char base=10);
	explicit Snake(unsigned int, unsigned char base=10);
	explicit Snake(long, unsigned char base=10);
	explicit Snake(unsigned long, unsigned char base=10);
	explicit Snake(float, unsigned char decimalPlaces=2);
	explicit Snake(double, unsigned char decimalPlaces=2);
	~Snake(void);

	// memory management
	// return true on success, false on failure (in which case, the string
	// is left unchanged).  reserve(0), if successful, will validate an
	// invalid string (i.e., "if (s)" will be true afterwards)
	unsigned char reserve(unsigned int size);
	inline unsigned int length(void) const {return len;}

	// creates a copy of the assigned value.  if the value is null or
	// invalid, or if the memory allocation fails, the string will be
	// marked as invalid ("if (s)" will be false).
	Snake & operator = (const Snake &rhs);
	Snake & operator = (const char *cstr);
//	Snake & operator = (const __FlashStringHelper *str);
	#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
		Snake & operator = (Snake &&rval);
		Snake & operator = (SnakeSumHelper &&rval);
	#endif

	// concatenate (works w/ built-in types)

	// returns true on success, false on failure (in which case, the string
	// is left unchanged).  if the argument is null or invalid, the
	// concatenation is considered unsucessful.
	unsigned char concat(const Snake &str);
	unsigned char concat(const char *cstr);
	unsigned char concat(char c);
	unsigned char concat(unsigned char c);
	unsigned char concat(int num);
	unsigned char concat(unsigned int num);
	unsigned char concat(long num);
	unsigned char concat(unsigned long num);
	unsigned char concat(float num);
	unsigned char concat(double num);
//	unsigned char concat(const __FlashSnakeHelper * str);

	// if there's not enough memory for the concatenated value, the string
	// will be left unchanged (but this isn't signalled in any way)
	Snake & operator += (const Snake &rhs)	{concat(rhs); return (*this);}
	Snake & operator += (const char *cstr)		{concat(cstr); return (*this);}
	Snake & operator += (char c)			{concat(c); return (*this);}
	Snake & operator += (unsigned char num)		{concat(num); return (*this);}
	Snake & operator += (int num)			{concat(num); return (*this);}
	Snake & operator += (unsigned int num)		{concat(num); return (*this);}
	Snake & operator += (long num)			{concat(num); return (*this);}
	Snake & operator += (unsigned long num)	{concat(num); return (*this);}
	Snake & operator += (float num)		{concat(num); return (*this);}
	Snake & operator += (double num)		{concat(num); return (*this);}
//	Snake & operator += (const __FlashStringHelper *str){concat(str); return (*this);}

	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, const Snake &rhs);
	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, const char *cstr);
	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, char c);
	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, unsigned char num);
	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, int num);
	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, unsigned int num);
	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, long num);
	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, unsigned long num);
	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, float num);
	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, double num);
//	friend SnakeSumHelper & operator + (const SnakeSumHelper &lhs, const __FlashStringHelper *rhs);

	// comparison (only works w/ Strings and "strings")
	operator SnakeIfHelperType() const { return buffer ? &Snake::SnakeIfHelper : 0; }
	int compareTo(const Snake &s) const;
	unsigned char equals(const Snake &s) const;
	unsigned char equals(const char *cstr) const;
	unsigned char operator == (const Snake &rhs) const {return equals(rhs);}
	unsigned char operator == (const char *cstr) const {return equals(cstr);}
	unsigned char operator != (const Snake &rhs) const {return !equals(rhs);}
	unsigned char operator != (const char *cstr) const {return !equals(cstr);}
	unsigned char operator <  (const Snake &rhs) const;
	unsigned char operator >  (const Snake &rhs) const;
	unsigned char operator <= (const Snake &rhs) const;
	unsigned char operator >= (const Snake &rhs) const;
	unsigned char equalsIgnoreCase(const Snake &s) const;
	unsigned char startsWith( const Snake &prefix) const;
	unsigned char startsWith(const Snake &prefix, unsigned int offset) const;
	unsigned char endsWith(const Snake &suffix) const;

	// character acccess
	char charAt(unsigned int index) const;
	void setCharAt(unsigned int index, char c);
	char operator [] (unsigned int index) const;
	char& operator [] (unsigned int index);
	void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index=0) const;
	void toCharArray(char *buf, unsigned int bufsize, unsigned int index=0) const
		{ getBytes((unsigned char *)buf, bufsize, index); }
	const char* c_str() const { return buffer; }
	char* begin() { return buffer; }
	char* end() { return buffer + length(); }
	const char* begin() const { return c_str(); }
	const char* end() const { return c_str() + length(); }

	// search
	int indexOf( char ch ) const;
	int indexOf( char ch, unsigned int fromIndex ) const;
	int indexOf( const Snake &str ) const;
	int indexOf( const Snake &str, unsigned int fromIndex ) const;
	int lastIndexOf( char ch ) const;
	int lastIndexOf( char ch, unsigned int fromIndex ) const;
	int lastIndexOf( const Snake &str ) const;
	int lastIndexOf( const Snake &str, unsigned int fromIndex ) const;
	Snake substring( unsigned int beginIndex ) const { return substring(beginIndex, len); };
	Snake substring( unsigned int beginIndex, unsigned int endIndex ) const;

	// modification
	void replace(char find, char replace);
	void replace(const Snake& find, const Snake& replace);
	void remove(unsigned int index);
	void remove(unsigned int index, unsigned int count);
	void toLowerCase(void);
	void toUpperCase(void);
	void trim(void);

	// parsing/conversion
	long toInt(void) const;
	float toFloat(void) const;
	double toDouble(void) const;

protected:
	char *buffer;	        // the actual char array
	unsigned int capacity;  // the array length minus one (for the '\0')
	unsigned int len;       // the String length (not counting the '\0')
protected:
	void init(void);
	void invalidate(void);
	unsigned char changeBuffer(unsigned int maxStrLen);
	unsigned char concat(const char *cstr, unsigned int length);

	// copy and move
	Snake & copy(const char *cstr, unsigned int length);
//	Snake & copy(const __FlashStringHelper *pstr, unsigned int length);
	#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
		void move(Snake &rhs);
	#endif
};

class SnakeSumHelper : public Snake
{
public:
	SnakeSumHelper(const Snake &s) : Snake(s) {}
	SnakeSumHelper(const char *p) : Snake(p) {}
	SnakeSumHelper(char c) : Snake(c) {}
	SnakeSumHelper(unsigned char num) : Snake(num) {}
	SnakeSumHelper(int num) : Snake(num) {}
	SnakeSumHelper(unsigned int num) : Snake(num) {}
	SnakeSumHelper(long num) : Snake(num) {}
	SnakeSumHelper(unsigned long num) : Snake(num) {}
	SnakeSumHelper(float num) : Snake(num) {}
	SnakeSumHelper(double num) : Snake(num) {}
};

#endif  // __cplusplus
