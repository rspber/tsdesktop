/*
  GfxObject

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include "Canvas.h"
#include <stdlib.h>

/// @GfxObject

GfxObject::GfxObject(const int16_t ax1, const int16_t ay1, rgb_t acolor)
{
  x1 = ax1;
  y1 = ay1;
  over.color = acolor;
  over.mode = 0;
  over.buf = 0;
  over.len = 0;
}

GfxObject::~GfxObject()
{
  if (over.buf) {
    free(over.buf);
    over.buf = 0;
    over.size = 0;
    over.len = 0;
    over.mode = 0;
  }
}


void GfxObject::setOverlaid(bool aOverlaid)
{
  over.mode = aOverlaid ? 1 : 0;
}

rgb_t GfxObject::getOver()
{
#ifdef OVERLAID
  if (over.mode) {
    over.len = 0;
    return (rgb_t)&over;
  }
#endif
  return over.color;
}

void GfxObject::setDefaultMaxs(clip_t& clip)
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

void GfxObject::animate1(clip_t& clip)
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

bool GfxObject::animated(clip_t& clip)
{
  if (animate == 1) {
    hide();
    setDefaultMaxs(clip);
    animate1(clip);
    return true;
  }
  return false;
}

void GfxObject::doDraw(clip_t& clip, int16_t left, int16_t top, bool redraw)
{
  if (animated(clip)) {
    redraw = true;
  }
  if (! wasDrawn || redraw) {
      dodraw(clip, left, top);
      wasDrawn = true;
  }
}

