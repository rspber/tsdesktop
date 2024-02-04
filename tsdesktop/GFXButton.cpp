/*
  GFXButton

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include "GFXButton.h"
#include "BufferedDisplay.h"
#include <string.h>

/// @GFXButton

GFXButton::GFXButton()
 : GFXButton(-1, -1, ALIGN_CLIENT, ALIGN_CLIENT)
{
}

GFXButton::GFXButton(
  const int16_t aLeft,
  const int16_t aTop,
  const int16_t aWidth,
  const int16_t aHeight,
  rgb_t aBackgroundColor,
  rgb_t aBorderColor)
  : Button(TYPE_GFXBUTTON, aLeft, aTop, aWidth, aHeight, aBackgroundColor, aBorderColor)
{
}

void GFXButton::setBuffered(const bool aBuffered)
{
  if (!aBuffered && buffered && bf_) {
    delete bf_;
    bf_ = 0;
  }
  buffered = aBuffered;
}

BufferedDisplay* GFXButton::bf()
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

TSD_SCREEN* GFXButton::screen()
{
  if (buffered) {
    return bf();
  }
  else {
    return Button::screen();
  }
}

GFXButton::~GFXButton()
{
  if (list) {
    for (int16_t i = len; --i >= 0;) {
      GFXObject* g = list[i];
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

int16_t GFXButton::index(GFXObject* item)
{
  for (int16_t i = len; --i >= 0;) {
    if (list[i] == item) {
      return i;
    }
  }
  return -1;
}

GFXObject* GFXButton::remove(GFXObject* item)
{
  for (int16_t i = len; --i >= 0;) {
    if (list[i] == item) {
      remove(i);
    } 
  }
  return item;
}

GFXObject* GFXButton::remove(int16_t index)
{
  if (index >= 0 && index < len) {
    GFXObject* item = list[index];
    memcpy(&list[index], &list[index+1], (--len - index) * sizeof(GFXObject*));
    return item;
  }
  return NULL;
}

void GFXButton::add(GFXObject* item)
{
  remove(item);
  if (len >= count) {
    int16_t nc = count+16;
    list = (GFXObject**)realloc(list, nc * sizeof(GFXObject*));
    count = nc;
  }
  list[len++] = item;
  item->gfxbtnparent = this;
  item->alloc = true;
}

void GFXButton::add(GFXObject& item)
{
  add(&item);
  item.alloc = false;
}

void GFXButton::animate(GFXObject& item, const int16_t dx, int16_t dy)
{
  item.animate = 1;
  item.dx = dx;
  item.dy = dy;
  add(item);
}

void GFXButton::innerDraw(bool redraw)
{
  if (buffered) {
//    BufferedDisplay* bf = static_cast<BufferedDisplay*>(screen());
//    memset(bf->buf, 0, bf->w * bf->h * MDT_SIZE);
    redraw = true;
  }

  clip_t clip;
  getOuterClip(clip);
  int16_t x = getAbsInnerLeft(0);
  int16_t y = getAbsInnerTop(0);
  for (int16_t i = 0; i < len; ++i) {
    GFXObject* g = list[i];
    if (g->getVisible()) {
      g->doDraw(clip, x, y, redraw);
    }
  }

  if (buffered && bf_) {
//    BufferedDisplay* bf = static_cast<BufferedDisplay*>(screen());
    bf_->outMeTo(Button::screen());
  }
}

void GFXButton::drawAll(bool redraw)
{
  if (screenEnabled) {
    if (getAbsVisible()) {
      innerDraw(redraw);
    }
  }
}
