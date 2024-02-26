/*
  Canvas

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include "GfxButton.h"
#include <string.h>

/// @Canvas

void Canvas::setBuffered(const bool aBuffered)
{
  if (!aBuffered && buffered && bf_) {
    delete bf_;
    bf_ = 0;
  }
  buffered = aBuffered;
}

BufferedDisplay* Canvas::bf()
{
  clip_t clip;
  getOuterClip(clip);
  if (!bf_) {
    bf_ = new BufferedDisplay(clip);
  }
  else {
    bf_->adjust(clip);
  }
  return bf_;
}

TSD_SCREEN* Canvas::screen()
{
  if (buffered) {
    return bf();
  }
  else {
    return superScreen();
  }
}

Canvas::~Canvas()
{
  if (list) {
    for (int16_t i = len; --i >= 0;) {
      GfxObject* g = list[i];
      if (g->alloc) {
        delete g;
      }
    }
    free(list);
  }
  if (bf_) {
    delete bf_;
  }
}

int16_t Canvas::index(GfxObject* item)
{
  for (int16_t i = len; --i >= 0;) {
    if (list[i] == item) {
      return i;
    }
  }
  return -1;
}

GfxObject* Canvas::remove(GfxObject* item)
{
  for (int16_t i = len; --i >= 0;) {
    if (list[i] == item) {
      remove(i);
    } 
  }
  return item;
}

GfxObject* Canvas::remove(int16_t index)
{
  if (index >= 0 && index < len) {
    GfxObject* item = list[index];
    memcpy(&list[index], &list[index+1], (--len - index) * sizeof(GfxObject*));
    return item;
  }
  return NULL;
}

void Canvas::add(GfxObject* item)
{
  remove(item);
  if (len >= count) {
    int16_t nc = count+16;
    list = (GfxObject**)realloc(list, nc * sizeof(GfxObject*));
    count = nc;
  }
  list[len++] = item;
  item->canvas = this;  // GfxObject can be placed on one canvas only
  item->alloc = true;
}

void Canvas::add(GfxObject& item)
{
  add(&item);
  item.alloc = false;
}

void Canvas::animate(GfxObject& item, const int16_t dx, int16_t dy)
{
  item.animate = 1;
  item.dx = dx;
  item.dy = dy;
  add(item);
}

void Canvas::innerDraw(bool redraw)
{
  clip_t clip;
  getOuterClip(clip);
  int16_t left = getX1();   // rather unused, default is 0
  int16_t top = getY1();   // rather unused, default is 0
  dodraw(clip, left, top);
}

void Canvas::dodraw(clip_t& clip, int16_t left, int16_t top)
{
  bool redraw = false;

  if (buffered && bf_) {
    bf_->clear(getBackgroundColor());
    redraw = true;
  }
  for (int i = 0; i < len; ++i) {
    GfxObject* g = list[i];
    if (g->getVisible()) {
      g->doDraw(clip, left, top, redraw);
    }
  }

  if (buffered && bf_) {
    bf_->push(superScreen());
  }
}

TSD_SCREEN* Canvas::superScreen()
{
  return canvas->screen();
}

/*
void GFXButton::drawAll(bool redraw)
{
  if (screenEnabled) {
    if (getAbsVisible()) {
      innerDraw(redraw);
    }
  }
}
*/

