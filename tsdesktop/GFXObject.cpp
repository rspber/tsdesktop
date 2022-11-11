/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "GFXObject.h"

/// @GFXObject

bool GFXObject::doDraw(rgb_t color)
{
  if (screenEnabled) {
    if (parent->getAbsVisible()) {
      clip_t clip;
      parent->getClip(clip);
      int16_t x = parent->getAbsInnerLeft(0);
      int16_t y = parent->getAbsInnerTop(0);
      dodraw(&clip, x, y, color);
      return true;
    }
  }
  return false;
}

void GFXObject::hide()
{
  if (wasDrawn) {
    doDraw(parent->getBackgroundColor());
    wasDrawn = false;
  }
}

void GFXObject::draw()
{
  if (doDraw(color)) {
    wasDrawn = true;
  }
}

void GFXObject::doDraw(clip_t* clip, int16_t x, int16_t y, const bool redraw)
{
  if (! wasDrawn || redraw) {
      dodraw(clip, x, y, color);
      wasDrawn = true;
  }
}
