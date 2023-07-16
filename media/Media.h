/*
  Media

  Display & Touch

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <Setup.h>
#include <Display.h>
#include <Touch.h>

extern bool screenEnabled;
extern Display display;

extern Touch touch;

void media_begin(uint8_t rotation);
