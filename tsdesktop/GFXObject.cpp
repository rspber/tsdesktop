/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "rgb.h"
#include "GFXObject.h"

/// @GFXObject

void GFXObject::setOverlaid(bool aOverlaid)
{
  over.mode = aOverlaid ? 1 : 0;
}

rgb_t GFXObject::getOver()
{
  if (over.mode == 1) {
    if (over.buf == 0) {
      over.size = over.size * MDT_SIZE;
      over.buf = (uint8_t*)malloc(over.size);
    }
    over.len = 0;
    return (rgb_t)&over;
  }
  if (over.mode == 2) {
    over.len = 0;
    return (rgb_t)&over;
  }
  return over.color;
}

void GFXObject::doDraw()
{
  clip_t clip;
  parent->getClip(clip);
  int16_t x = parent->getAbsInnerLeft(0);
  int16_t y = parent->getAbsInnerTop(0);
  dodraw(&clip, x, y);
}

void GFXObject::hide()
{
  if (wasDrawn) {
    rgb_t color = over.color;
    if (over.mode == 1) {
      over.mode = 2;
    }
    else {
      over.color = parent->getBackgroundColor();
    }
    doDraw();
    if (over.mode == 2) {
      over.mode = 1;
    }
    else {
      over.color = color;
    }
    wasDrawn = false;
  }
}

void GFXObject::draw()
{
  doDraw();
  wasDrawn = true;
}

void GFXObject::doDraw(clip_t* clip, int16_t x, int16_t y, const bool redraw)
{
  if (! wasDrawn || redraw) {
      dodraw(clip, x, y);
      wasDrawn = true;
  }
}

