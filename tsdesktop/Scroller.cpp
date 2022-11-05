/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "TSDesktop.h"


/// @ScrollBtn

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

void ProgressBtn::init(const bool ver, const uint8_t* icon, const int16_t l, int16_t t, const int16_t siz)
{
  vert = ver;
  bmp = icon;
  x0 = l;
  y0 = t;
  x1 = x0;
  y1 = y0;
  if (vert) {
    y1 -= 4;
  }
  else {
    x1 -= 4;
  }
  size = siz;
}

void ProgressBtn::set(int8_t percent)
{
  if (vert) {
    y1 = y0 + (size * percent / 100) - 4;
  }
  else {
    x1 = x0 + (size * percent / 100) - 4;
  }
}

void ProgressBtn::draw()
{
  if (bmp) {
    clip_t clip{x1, y1, (int16_t)(x1+16), (int16_t)(y1+16)};
    display.drawBitmap(&clip, x1, y1, bmp, 16, 16, SILVER);
  }
}

void ScrollBtn::init(const uint8_t whch, const uint8_t* icon, const int16_t l, const int16_t t, const int16_t w, const int16_t h)
{
  which = whch;
  bmp = icon;
  x1 = l;
  y1 = t;
  x2 = l + w;
  y2 = t + h;
}

bool ScrollBtn::isSet()
{
  return x2 > x1 && y2 > y1;
}

void ScrollBtn::draw()
{
  if (isSet()) {
    clip_t clip{(int16_t)(x1-1), (int16_t)(y1-1), (int16_t)(x2+1), (int16_t)(y2+1)};
    display.fillRect(&clip, x1, y1, x2-x1, y2-y1, SILVER);
    display.drawBitmap(&clip, x1, y1, bmp, 16, 16, BLUE);
    display.drawRect(&clip, x1-1, y1-1, x2-x1+2, y2-y1+2, WHITE);
  }
}

void ScrollBtn::clickEffect()
{
  if (isSet()) {
    clip_t clip{(int16_t)(x1-1), (int16_t)(y1-1), (int16_t)(x2+1), (int16_t)(y2+1)};
    display.drawRect(&clip, x1-1, y1-1, x2-x1+2, y2-y1+2, RED);
    sleep_ms(50);
    display.drawRect(&clip, x1-1, y1-1, x2-x1+2, y2-y1+2, WHITE);
  }
}

const bool ScrollBtn::scroll(FieldSet* view, int8_t* percent)
{
  uint8_t tp = which & 0x03;
  bool up = (which & 0x04) == 0;
  bool vert = (which & 0x08) != 0;
  if (vert) {
    return view->vertScroll(tp, up, percent);
  }
  else {
    return view->horizScroll(tp, up, percent);
  }
}

bool ScrollBtn::pressed(FieldSet* view, const int16_t xScreen, const int16_t yScreen, ProgressBtn* progress)
{
  bool pressed = xScreen > x1 - 1 && xScreen < x2 + 1 && yScreen > y1 - 1 && yScreen < y2 + 1;
  if (pressed) {
    clickEffect();
//	view->hide();
    int8_t percent;
    if (scroll(view, &percent)) {
      progress->set(percent);
      view->draw(true);
    }
  }
  return pressed;
}


/// @Scroller

bool Scroller::init(FieldSet* view, int8_t mgr)
{
  int16_t l = view->getAbsOuterLeft();
  int16_t t = view->getAbsOuterTop();
  int16_t w = view->getUpdWidth();
  int16_t h = view->getUpdHeight();
  int8_t width = 16;
  int8_t height = 16;
  clip_t clip;
  view->getOuterClip(clip);
  if (orient.vertical) {
    if (clip.y2 < t + h) {
      int16_t left = clip.x2 - width;
      int16_t top = clip.y1;
      up.home.init(SCROLL_BTN_HOME | SCROLL_UP | SCROLL_VERT, SCRLBMPT[6], left, top, width, height); top += height + 2;
      up.page.init(SCROLL_BTN_PAGE | SCROLL_UP | SCROLL_VERT, SCRLBMPT[7], left, top, width, height); top += height + 2;
      up.step.init(SCROLL_BTN_STEP | SCROLL_UP | SCROLL_VERT, SCRLBMPT[8], left, top, width, height); top += height + 2;

      up.progress.init(true, SCRLBMPT[12], left, top, clip.y2 - (height + 2)*7);

      top = clip.y2; top -= height;
      dn.home.init(SCROLL_BTN_HOME | SCROLL_DN | SCROLL_VERT, SCRLBMPT[11], left, top, width, height); top -= height + 2;
      dn.page.init(SCROLL_BTN_PAGE | SCROLL_DN | SCROLL_VERT, SCRLBMPT[10], left, top, width, height); top -= height + 2;
      dn.step.init(SCROLL_BTN_STEP | SCROLL_DN | SCROLL_VERT, SCRLBMPT[ 9], left, top, width, height); top -= height + 2;
      active = true;
      return true;
    }
  }
  else {
    if (clip.x2 < l + w) {
      int16_t top = clip.y2 - height;
      int16_t left = clip.x1;
      up.home.init(SCROLL_BTN_HOME | SCROLL_UP | SCROLL_HORIZ, SCRLBMPT[0], left, top, width, height); left += width + 2;
      up.page.init(SCROLL_BTN_PAGE | SCROLL_UP | SCROLL_HORIZ, SCRLBMPT[1], left, top, width, height); left += width + 2;
      up.step.init(SCROLL_BTN_STEP | SCROLL_UP | SCROLL_HORIZ, SCRLBMPT[2], left, top, width, height); left += width + 2;

      up.progress.init(false, SCRLBMPT[12], left, top, clip.x2 - (width + 2)*(7 + mgr));

      left = clip.x2 - width - (mgr * 18);
      dn.home.init(SCROLL_BTN_HOME | SCROLL_DN | SCROLL_HORIZ, SCRLBMPT[5], left, top, width, height); left -= width + 2;
      dn.page.init(SCROLL_BTN_PAGE | SCROLL_DN | SCROLL_HORIZ, SCRLBMPT[4], left, top, width, height); left -= width + 2;
      dn.step.init(SCROLL_BTN_STEP | SCROLL_DN | SCROLL_HORIZ, SCRLBMPT[3], left, top, width, height); left -= width + 2;
      active = true;
      return true;
    }
  }
  active = false;
  return false;
}

void Scroller::draw(FieldSet* view, Scroller* hScr)
{
  if (!active) {
    init(view, hScr ? hScr->active : 0);
  }
  if (active) {
    up.home.draw();
    up.page.draw();
    up.step.draw();
    if (view->getShowScrollerProgress()) {
      up.progress.draw();
    }
    dn.home.draw();
    dn.page.draw();
    dn.step.draw();
  }
}

ScrollBtn* Scroller::pressed(FieldSet* view, const int16_t xScreen, const int16_t yScreen)
{
  if (active) {
    if (up.home.pressed(view, xScreen, yScreen, &up.progress)) {
        return &up.home;
    }
    if (up.page.pressed(view, xScreen, yScreen, &up.progress)) {
        return &up.page;
    }
    if (up.step.pressed(view, xScreen, yScreen, &up.progress)) {
        return &up.step;
    }
    if (dn.home.pressed(view, xScreen, yScreen, &up.progress)) {
        return &dn.home;
    }
    if (dn.page.pressed(view, xScreen, yScreen, &up.progress)) {
        return &dn.page;
    }
    if (dn.step.pressed(view, xScreen, yScreen, &up.progress)) {
        return &dn.step;
    }
  }
  return NULL;
}
