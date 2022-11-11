/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "GFXObject.h"

/// @GFXObject

void GFXObject::hide()
{
  if (wasDrawn) {
    if (screenEnabled) {
      if (parent->getAbsVisible()) {
        clip_t clip;
        parent->getClip(clip);
        int16_t x = parent->getAbsInnerLeft(0);
        int16_t y = parent->getAbsInnerTop(0);
        dodraw(&clip, x, y, parent->getBackgroundColor());
      }
    }
    wasDrawn = false;
  }
}

void GFXObject::draw()
{
  if (screenEnabled) {
    if (parent->getAbsVisible()) {
      clip_t clip;
      parent->getClip(clip);
      int16_t x = parent->getAbsInnerLeft(0);
      int16_t y = parent->getAbsInnerTop(0);
      dodraw(&clip, x, y, color);
      wasDrawn = true;
    }
  }
}

void GFXObject::dodraw(clip_t* clip, int16_t x, int16_t y, const bool redraw)
{
  if (! wasDrawn || redraw) {
      dodraw(clip, x, y, color);
      wasDrawn = true;
  }
}
