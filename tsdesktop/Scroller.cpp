/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "TSDesktop.h"


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

void draw_progress(const int16_t x1, const int16_t y1, const uint8_t* bmp)
{
  clip_t clip{x1, y1, (int16_t)(x1+16), (int16_t)(y1+16)};
  display.drawBitmap(&clip, x1, y1, bmp, 16, 16, SILVER);
}

void draw_btn(const int16_t x1, const int16_t y1, const uint8_t* bmp)
{
  int16_t w = 16;
  int16_t h = 16;
  clip_t clip{(int16_t)(x1-1), (int16_t)(y1-1), (int16_t)(x1+w+1), (int16_t)(y1+h+1)};
  display.fillRect(&clip, x1, y1, w, h, SILVER);
  display.drawBitmap(&clip, x1, y1, bmp, 16, 16, BLUE);
  display.drawRect(&clip, x1-1, y1-1, w+2, h+2, WHITE);
}

void click_effect(const int16_t x1, const int16_t y1)
{
  int16_t w = 16;
  int16_t h = 16;
  clip_t clip{(int16_t)(x1-1), (int16_t)(y1-1), (int16_t)(x1+w+1), (int16_t)(y1+h+1)};
  display.drawRect(&clip, x1-1, y1-1, w+2, h+2, RED);
  sleep_ms(50);
  display.drawRect(&clip, x1-1, y1-1, w+2, h+2, WHITE);
}

void Container::drawScroller()
{
  int16_t x = getAbsOuterLeft();
  int16_t y = getAbsOuterTop();
  int8_t ws = 16;
  int8_t hs = 16;
  clip_t clip;
  getOuterClip(clip);
  int16_t page_width = clip.x2 - clip.x1;
  int16_t page_height = clip.y2 - clip.y1;
  int8_t mgr = 0;
  if (clip.y2 < y + updHeight) {
    int16_t x1 = x + page_width - ws;
    int16_t y1 = y - (hs + 2);;

    if (scroll_btns & SCROLL_HOME) {
      draw_btn(x1, y1 += hs + 2, SCRLBMPT[6]);
    }
    if (scroll_btns & SCROLL_PAGE) {
      draw_btn(x1, y1 += hs + 2, SCRLBMPT[7]);
    }
    if (scroll_btns & SCROLL_STEP) {
      draw_btn(x1, y1 += hs + 2, SCRLBMPT[8]);
    }

    int16_t y_progress_top = y1 + (hs + 2);

    y1 = y + page_height + 2;
    if (scroll_btns & SCROLL_HOME) {
      draw_btn(x1, y1 -= hs + 2, SCRLBMPT[11]);
    }
    if (scroll_btns & SCROLL_PAGE) {
      draw_btn(x1, y1 -= hs + 2, SCRLBMPT[10]);
    }
    if (scroll_btns & SCROLL_STEP) {
      draw_btn(x1, y1 -= hs + 2, SCRLBMPT[ 9]);
    }

    if (scroll_btns & SCROLL_PROGRESS) {
      int8_t percent = offsetTop * 100 / (updHeight - page_height);
      draw_progress(x1, y_progress_top + (percent * (y1 - y_progress_top - 10) / 100 ) - 4, SCRLBMPT[12] );
    }
    mgr = 1;
  }
  if (clip.x2 < x + updWidth) {
    int16_t y1 = y + page_height - hs;
    int16_t x1 = x - (ws + 2);
    if (scroll_btns & SCROLL_HOME) {
      draw_btn(x1 += ws + 2, y1, SCRLBMPT[0]);
    }
    if (scroll_btns & SCROLL_PAGE) {
      draw_btn(x1 += ws + 2, y1, SCRLBMPT[1]);
    }
    if (scroll_btns & SCROLL_STEP) {
      draw_btn(x1 += ws + 2, y1, SCRLBMPT[2]);
    }

    int16_t x_progress_left = x1 + (ws + 2);

    x1 = x + page_width + 2 - mgr * (ws + 2);
    if (scroll_btns & SCROLL_HOME) {
      draw_btn(x1 -= ws + 2, y1, SCRLBMPT[5]);
    }
    if (scroll_btns & SCROLL_PAGE) {
      draw_btn(x1 -= ws + 2, y1, SCRLBMPT[4]);
    }
    if (scroll_btns & SCROLL_STEP) {
      draw_btn(x1 -= ws + 2, y1, SCRLBMPT[3]);
    }

    if (scroll_btns & SCROLL_PROGRESS) {
      int8_t percent = offsetLeft * 100 / (updWidth - page_width);
      draw_progress(x_progress_left + (percent * (x1 - x_progress_left - 10) / 100) - 4, y1, SCRLBMPT[12]);
    }
  }
}


/// ScrollerPressed

bool btn_pressed(const int16_t x1, const int16_t y1, const int16_t xScreen, const int16_t yScreen)
{
  int16_t w = 16;
  int16_t h = 16;
  bool pressed = xScreen > x1 - 1 && xScreen < x1 + w + 1 && yScreen > y1 - 1 && yScreen < y1 + h + 1;
  if (pressed) {
    click_effect(x1, y1);
  }
  return pressed;
}

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
  getClip(clip);
  bool ok = false;
  if (vert) {
    int16_t offs = doScroll(tp, up, offsetTop, updHeight, clip.y2 - clip.y1);
    if (offs != offsetTop) {
      offsetTop = offs;
      ok = true;
    }
  }
  else {
    int16_t offs = doScroll(tp, up, offsetLeft, updWidth, clip.x2 - clip.x1);
    if (offs != offsetLeft) {
      offsetLeft = offs;
      ok = true;
    }
  }
  if (ok) {
    setChanged(); // maybe it is not needed
    draw(true);
  }
}

const bool Container::scrollerPressed(const int16_t xScreen, const int16_t yScreen)
{
  int16_t x = getAbsOuterLeft();
  int16_t y = getAbsOuterTop();
  int8_t ws = 16;
  int8_t hs = 16;
  clip_t clip;
  getOuterClip(clip);
  int16_t page_width = clip.x2 - clip.x1;
  int16_t page_height = clip.y2 - clip.y1;
  int8_t mgr = 0;
  if (clip.y2 < y + updHeight) {
    int16_t x1 = x + page_width - ws;
    int16_t y1 = y - (hs + 2);
    if (scroll_btns & SCROLL_HOME && btn_pressed(x1, y1 += hs + 2, xScreen, yScreen)) {
      scroll(SCROLL_HOME, true, true);
      return true;
    }
    if (scroll_btns & SCROLL_PAGE && btn_pressed(x1, y1 += hs + 2, xScreen, yScreen)) {
      scroll(SCROLL_PAGE, true, true);
      return true;
    }
    if (scroll_btns & SCROLL_STEP && btn_pressed(x1, y1 += hs + 2, xScreen, yScreen)) {
      scroll(SCROLL_STEP, true, true);
      return true;
    }

    y1 = y + page_height + 2;
    if (scroll_btns & SCROLL_HOME && btn_pressed(x1, y1 -= hs + 2, xScreen, yScreen)) {
      scroll(SCROLL_HOME, false, true);
      return true;
    }
    if (scroll_btns & SCROLL_PAGE && btn_pressed(x1, y1 -= hs + 2, xScreen, yScreen)) {
      scroll(SCROLL_PAGE, false, true);
      return true;
    }
    if (scroll_btns & SCROLL_STEP && btn_pressed(x1, y1 -= hs + 2, xScreen, yScreen)) {
      scroll(SCROLL_STEP, false, true);
      return true;
    }
    mgr = 1;
  }
  if (clip.x2 < x + updWidth) {
    int16_t y1 = y + page_height - hs;
    int16_t x1 = x - (ws + 2);
    if (scroll_btns & SCROLL_HOME && btn_pressed(x1 += ws + 2, y1, xScreen, yScreen)) {
      scroll(SCROLL_HOME, true, false);
      return true;
    }
    if (scroll_btns & SCROLL_PAGE && btn_pressed(x1 += ws + 2, y1, xScreen, yScreen)) {
      scroll(SCROLL_PAGE, true, false);
      return true;
    }
    if (scroll_btns & SCROLL_STEP && btn_pressed(x1 += ws + 2, y1, xScreen, yScreen)) {
      scroll(SCROLL_STEP, true, false);
      return true;
    }

    x1 = x + page_width + 2 - mgr * (ws + 2);
    if (scroll_btns & SCROLL_HOME && btn_pressed(x1 -= ws + 2, y1, xScreen, yScreen)) {
      scroll(SCROLL_HOME, false, false);
      return true;
    }
    if (scroll_btns & SCROLL_PAGE && btn_pressed(x1 -= ws + 2, y1, xScreen, yScreen)) {
      scroll(SCROLL_PAGE, false, false);
      return true;
    }
    if (scroll_btns & SCROLL_STEP && btn_pressed(x1 -= ws + 2, y1, xScreen, yScreen)) {
      scroll(SCROLL_STEP, false, false);
      return true;
    }
  }
  return false;
}
