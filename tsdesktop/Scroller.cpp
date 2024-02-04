/*
  TouchScreenDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include "TSDesktop.h"
#include <Arduino.h>

/// @Scroller

static const uint8_t SCRLBMPT[13][32]{
  {0x00, 0x00, 0x30, 0x18, 0x30, 0x38, 0x30, 0x68, 0x30, 0xc8, 0x31, 0x88, 0x33, 0x08, 0x36, 0x08,
   0x36, 0x08, 0x33, 0x08, 0x31, 0x88, 0x30, 0xc8, 0x30, 0x68, 0x30, 0x38, 0x30, 0x18, 0x00, 0x00},  // |<
  {0x00, 0x00, 0x01, 0x86, 0x03, 0x0c, 0x06, 0x18, 0x0c, 0x30, 0x18, 0x60, 0x30, 0xc0, 0x61, 0x80,
   0x61, 0x80, 0x30, 0xc0, 0x18, 0x60, 0x0c, 0x30, 0x06, 0x18, 0x03, 0x0c, 0x01, 0x86, 0x00, 0x00},  // <<
  {0x00, 0x00, 0x00, 0x30, 0x00, 0x60, 0x00, 0xc0, 0x01, 0x80, 0x03, 0x00, 0x06, 0x00, 0x0c, 0x00,
   0x0c, 0x00, 0x06, 0x00, 0x03, 0x00, 0x01, 0x80, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x30, 0x00, 0x00},  // <
  {0x00, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x03, 0x00, 0x01, 0x80, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x30,
   0x00, 0x30, 0x00, 0x60, 0x00, 0xc0, 0x01, 0x80, 0x03, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x00, 0x00},  // >
  {0x00, 0x00, 0x61, 0x80, 0x30, 0xc0, 0x18, 0x60, 0x0c, 0x30, 0x06, 0x18, 0x03, 0x0c, 0x01, 0x86,
   0x01, 0x86, 0x03, 0x0c, 0x06, 0x18, 0x0c, 0x30, 0x18, 0x60, 0x30, 0xc0, 0x61, 0x80, 0x00, 0x00},  // >>
  {0x00, 0x00, 0x18, 0x0c, 0x1c, 0x0c, 0x16, 0x0c, 0x13, 0x0c, 0x11, 0x8c, 0x10, 0xcc, 0x10, 0x6c,
   0x10, 0x6c, 0x10, 0xcc, 0x11, 0x8c, 0x13, 0x0c, 0x16, 0x0c, 0x1c, 0x0c, 0x18, 0x0c, 0x00, 0x00},  // >|
  {0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00, 0x01, 0x80, 0x03, 0xc0, 0x06, 0x60,
   0x0c, 0x30, 0x18, 0x18, 0x30, 0x0c, 0x60, 0x06, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // ^_
  {0x00, 0x00, 0x01, 0x80, 0x03, 0xc0, 0x06, 0x60, 0x0c, 0x30, 0x18, 0x18, 0x30, 0x0c, 0x61, 0x86,
   0x03, 0xc0, 0x06, 0x60, 0x0c, 0x30, 0x18, 0x18, 0x30, 0x0c, 0x60, 0x06, 0x40, 0x02, 0x00, 0x00},  // ^^
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0xc0, 0x06, 0x60, 0x0c, 0x30,
   0x18, 0x18, 0x30, 0x0c, 0x60, 0x06, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // ^
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x60, 0x06, 0x30, 0x0c,
   0x18, 0x18, 0x0c, 0x30, 0x06, 0x60, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // v
  {0x00, 0x00, 0x40, 0x02, 0x60, 0x06, 0x30, 0x0c, 0x18, 0x18, 0x0c, 0x30, 0x06, 0x60, 0x03, 0xc0,
   0x61, 0x86, 0x30, 0x0c, 0x18, 0x18, 0x0c, 0x30, 0x06, 0x60, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00},  // vv
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x60, 0x06, 0x30, 0x0c, 0x18, 0x18, 0x0c, 0x30,
   0x06, 0x60, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00},  // v_
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0xc0, 0x07, 0xe0,
   0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}   // o
};

void draw_progress(TSD_SCREEN* screen, const int16_t x1, const int16_t y1, const uint8_t* bmp, const rgb_t color)
{
  clip_t clip{(int16_t)x1, (int16_t)y1, (int16_t)(x1+16), (int16_t)(y1+16)};
  screen->drawBitmap(clip, x1, y1, bmp, 16, 16, color);
}

void draw_btn(TSD_SCREEN* screen, const int16_t x1, const int16_t y1, const uint8_t* bmp)
{
  int16_t w = 16;
  int16_t h = 16;
  clip_t clip{(int16_t)(x1-1), (int16_t)(y1-1), (int16_t)(x1+w+1), (int16_t)(y1+h+1)};
  screen->fillRect(clip, x1, y1, w, h, SILVER);
  screen->drawBitmap(clip, x1, y1, bmp, 16, 16, BLUE);
  screen->drawRect(clip, x1-1, y1-1, w+2, h+2, WHITE);
}

void click_effect(TSD_SCREEN* screen, const int16_t x1, const int16_t y1)
{
  int16_t w = 16;
  int16_t h = 16;
  clip_t clip{(int16_t)(x1-1), (int16_t)(y1-1), (int16_t)(x1+w+1), (int16_t)(y1+h+1)};
  screen->drawRect(clip, x1-1, y1-1, w+2, h+2, RED);
  delay(50);
  screen->drawRect(clip, x1-1, y1-1, w+2, h+2, WHITE);
}

class GoScroller {
public:
  GoScroller(Container* ac, const uint8_t abtns) : c(ac), btns(abtns) {}
  virtual bool btn(const int16_t x1, const int16_t y1, const uint8_t tp, const bool up, const bool vert) = 0;
  virtual void progress(const int16_t x1, const int16_t y1) = 0;
  bool walk();

  Container* c;
private:
  uint8_t btns;
};

bool GoScroller::walk()
{
  int8_t ws = 16;
  int8_t hs = 16;
  clip_t clip;
  c->getOuterClip(clip);
  int16_t pageWidth = clip.width();
  int16_t pageHeight = clip.height();
  int8_t mgr = 0;
  int16_t scrls = (btns & SCROLL_HOME ? 1 : 0) + (btns & SCROLL_PAGE ? 1 : 0) + (btns & SCROLL_STEP ? 1 : 0);
  if (pageHeight < c->getUpdHeight() && pageHeight >= 2 * scrls * (hs + 2)) {
    int16_t x1 = clip.x2 - ws;
    int16_t y1 = clip.y1 - (hs + 2);;

    if (btns & SCROLL_HOME) {
      if (btn(x1, y1 += hs + 2, SCROLL_HOME, true, true)) {
        return true;
      }
    }
    if (btns & SCROLL_PAGE) {
      if (btn(x1, y1 += hs + 2, SCROLL_PAGE, true, true)) {
        return true;
      }
    }
    if (btns & SCROLL_STEP) {
      if (btn(x1, y1 += hs + 2, SCROLL_STEP, true, true)) {
        return true;
      }
    }

    int16_t y_progress_top = y1 + (hs + 2);

    y1 = clip.y1 + pageHeight + 2;
    if (btns & SCROLL_HOME) {
      if (btn(x1, y1 -= hs + 2, SCROLL_HOME, false, true)) {
        return true;
      }
    }
    if (btns & SCROLL_PAGE) {
      if (btn(x1, y1 -= hs + 2, SCROLL_PAGE, false, true)) {
        return true;
      }
    }
    if (btns & SCROLL_STEP) {
      if (btn(x1, y1 -= hs + 2, SCROLL_STEP, false, true)) {
        return true;
      }
    }

    if (btns & SCROLL_PROGRESS) {
      int8_t percent = c->getOffsetTop() * 100 / (c->getUpdHeight() - pageHeight);
      progress(x1, y_progress_top + (percent * (y1 - y_progress_top - 10) / 100 ) - 4);
    }
    mgr = 1;
  }
  if (pageWidth < c->getUpdWidth() && pageWidth >= 2 * scrls * (ws + 2)) {
    int16_t y1 = clip.y2 - hs;
    int16_t x1 = clip.x1 - (ws + 2);
    if (btns & SCROLL_HOME) {
      if (btn(x1 += ws + 2, y1, SCROLL_HOME, true, false)) {
        return true;
      }
    }
    if (btns & SCROLL_PAGE) {
      if (btn(x1 += ws + 2, y1, SCROLL_PAGE, true, false)) {
        return true;
      }
    }
    if (btns & SCROLL_STEP) {
      if (btn(x1 += ws + 2, y1, SCROLL_STEP, true, false)) {
        return true;
      }
    }

    int16_t x_progress_left = x1 + (ws + 2);

    x1 = clip.x1 + pageWidth + 2 - mgr * (ws + 2);
    if (btns & SCROLL_HOME) {
      if (btn(x1 -= ws + 2, y1, SCROLL_HOME, false, false)) {
        return true;
      }
    }
    if (btns & SCROLL_PAGE) {
      if (btn(x1 -= ws + 2, y1, SCROLL_PAGE, false, false)) {
        return true;
      }
    }
    if (btns & SCROLL_STEP) {
      if (btn(x1 -= ws + 2, y1, SCROLL_STEP, false, false)) {
        return true;
      }
    }

    if (btns & SCROLL_PROGRESS) {
      int8_t percent = c->getOffsetLeft() * 100 / (c->getUpdWidth() - pageWidth);
      progress(x_progress_left + (percent * (x1 - x_progress_left - 10) / 100) - 4, y1);
    }
  }
  return false;
}

class DrawScroller : public GoScroller {
public:
  DrawScroller(Container* c, const uint8_t btns)
   : GoScroller(c, btns) {}

  virtual bool btn(const int16_t x1, const int16_t y1, const uint8_t tp, const bool up, const bool vert)
  {
    int8_t idx;
    switch (tp) {
      case SCROLL_HOME: idx = vert ? up ? 6 : 11 : up ? 0 : 5; break;
      case SCROLL_PAGE: idx = vert ? up ? 7 : 10 : up ? 1 : 4; break;
      case SCROLL_STEP: idx = vert ? up ? 8 :  9 : up ? 2 : 3; break;
    }
    draw_btn(c->screen(), x1, y1, SCRLBMPT[idx]);
    return false;
  }

  virtual void progress(const int16_t x1, const int16_t y1)
  {
    draw_progress(c->screen(), x1, y1, SCRLBMPT[12], SILVER);
  }
};

void Container::drawScroller()
{
  DrawScroller(this, scroll_btns).walk();
}


/// ScrollerPressed

class HideProgress : public GoScroller {
public:
  HideProgress(Container* c, const uint8_t btns)
   : GoScroller(c, btns) {}
  virtual bool btn(const int16_t x1, const int16_t y1, const uint8_t tp, const bool up, const bool vert)
  {
     return false;
  }
  virtual void progress(const int16_t x1, const int16_t y1)
  {
    draw_progress(c->screen(), x1, y1, SCRLBMPT[12], c->getBackground());
  }
};

const int16_t doScroll(const uint8_t tp, const bool up, int16_t offs, const int16_t size, const int16_t page)
{
  switch (tp) {
    case SCROLL_STEP:
      if (up) {
        offs -= 16;
        if (offs < 0) {
          offs = 0;
        }
      }
      else {
        offs += 16;
      }
      break;
    case SCROLL_PAGE:
      if (up) {
        offs -= page;
        if (offs < 0) {
          offs = 0;
        }
      }
      else {
        offs += page;
      }
      break;
    case SCROLL_HOME:
      offs = up ? 0 : 0x7fff;
      break;
  }
  int maxoffs = size - page;
  if (offs > maxoffs) {
    offs = maxoffs;
  }
  if (offs < 0) {
    offs = 0;
  }
  return offs;
}

void Container::scroll(const uint8_t tp, const bool up, const bool vert)
{
  clip_t clip;
  getOuterClip(clip);
  bool ok = false;
  if (vert) {
    int16_t offs = doScroll(tp, up, offsetTop, updHeight, clip.height());
    if (offs != offsetTop) {
      HideProgress(this, scroll_btns).walk();
      offsetTop = offs;
      ok = true;
    }
  }
  else {
    int16_t offs = doScroll(tp, up, offsetLeft, updWidth, clip.width());
    if (offs != offsetLeft) {
      HideProgress(this, scroll_btns).walk();
      offsetLeft = offs;
      ok = true;
    }
  }
  if (ok) {
    setChanged(); // maybe it is not needed
    Container* d = this;
    Container* p;
    while ((p = d->getParent())) {
      d = p;
    }
    d->draw(true);
  }
}

class ScrollerPressed : public GoScroller {
public:
  ScrollerPressed(Container* c, const uint8_t btns, const int16_t axScreen, const int16_t ayScreen)
   : GoScroller(c, btns), xScreen(axScreen), yScreen(ayScreen) {}
  virtual bool btn(const int16_t x1, const int16_t y1, const uint8_t tp, const bool up, const bool vert)
  {
    int16_t w = 16;
    int16_t h = 16;
    bool pressed = xScreen > x1 - 1 && xScreen < x1 + w + 1 && yScreen > y1 - 1 && yScreen < y1 + h + 1;
    if (pressed) {
      click_effect(c->screen(), x1, y1);
      c->scroll(tp, up, vert);
    }
    return pressed;
  }
  virtual void progress(const int16_t x1, const int16_t y1)
  {
  }

private:
  int16_t xScreen, yScreen;
};

const bool Container::scrollerPressed(const int16_t xScreen, const int16_t yScreen)
{
  return ScrollerPressed(this, scroll_btns, xScreen, yScreen).walk();
}
