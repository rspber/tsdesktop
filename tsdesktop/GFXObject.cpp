/*
  GFXObject

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include "rgb.h"
#include "GFXButton.h"

/// @GFXObject

void GFXObject::setOverlaid(bool aOverlaid)
{
  over.mode = aOverlaid ? 1 : 0;
}

rgb_t GFXObject::getOver()
{
#ifdef OVERLAID
  if (over.mode) {
    over.len = 0;
    return (rgb_t)&over;
  }
#endif
  return over.color;
}

void GFXObject::draw()
{
  clip_t clip;
  gfxbtnparent->getInnerClip(clip);
  int16_t x = gfxbtnparent->getAbsInnerLeft(0);
  int16_t y = gfxbtnparent->getAbsInnerTop(0);
  dodraw(clip, x, y);
  wasDrawn = true;
}

void GFXObject::hide()
{
  const bool buffered = static_cast<GFXButton*>(gfxbtnparent)->getBuffered();
  if (!buffered && wasDrawn) {
    rgb_t color = over.color;
    if (over.mode == 1) {
      over.mode = 2;
    }
    else {
      over.color = gfxbtnparent->getBackgroundColor();
    }
    draw();
    if (over.mode == 2) {
      over.mode = 1;
    }
    else {
      over.color = color;
    }
    wasDrawn = false;
  }
}

void GFXObject::setDefaultMaxs(clip_t& clip)
{
  if (maxx == 0) {
    maxx = clip.x2 - clip.x1 - rW();
    minx = lW();
  }
  if (maxy == 0) {
    maxy = clip.y2 - clip.y1 - bH();
    miny = tH();
  }
}

void GFXObject::animate1(clip_t& clip)
{
  x1 = x1 + dx;
  if (x1 < minx) {
    x1 = minx;
    dx = -dx;
  }
  if (x1 >= maxx) {
    x1 = maxx;
    dx = -dx;
  }
  y1 = y1 + dy;
  if (y1 < miny) {
    y1 = miny;
    dy = -dy;
  }
  if (y1 >= maxy) {
    y1 = maxy;
    dy = -dy;
  }
}

bool GFXObject::animated(clip_t& clip)
{
  if (animate == 1) {
    hide();
    setDefaultMaxs(clip);
    animate1(clip);
    return true;
  }
  return false;
}

void GFXObject::doDraw(clip_t& clip, int16_t x, int16_t y, bool redraw)
{
  if (animated(clip)) {
    redraw = true;
  }
  if (! wasDrawn || redraw) {
      dodraw(clip, x, y);
      wasDrawn = true;
  }
}


TSD_SCREEN* GFXObject::writer()
{
  return static_cast<GFXButton*>(gfxbtnparent)->screen();
}

