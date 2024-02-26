/*
  BufferedDisplay

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

*/

#pragma once

#include "TSD_SCREEN.h"
#include <stdlib.h>
#include <string.h>

class BufferedDisplay : public TSD_SCREEN {
public:

  BufferedDisplay(clip_t& aClip, const rgb_t aBackgroundColor = BLACK)
  : TSD_SCREEN(aClip.width(), aClip.height())
  {
    clip = aClip;
    initialize(aBackgroundColor);
  }

  BufferedDisplay(const int16_t x1, const int16_t y1, const int16_t x2, const int16_t y2, const rgb_t aBackgroundColor = BLACK)
  : TSD_SCREEN(x2 - x1, y2 - y1)
  {
    clip = {x1, y1, x2, y2};
    initialize(aBackgroundColor);
  }

  virtual ~BufferedDisplay()
  {
    free(buf);
    buf = 0;
  }

  void setPos(const int16_t x, const int16_t y)
  {
    clip.x2 = clip.x2 - clip.x1 + x;
    clip.x1 = x;
    clip.y2 = clip.y2 - clip.y1 + y;
    clip.y1 = y;
  }

  void startWrite() override {}

  void endWrite() override {}

  void writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h) override
  {
    addr_x = x - clip.x1;
    addr_y = y - clip.y1;
    addr_w = w;
    addr_h = h;
    ip = 0;
  }

  void sendMDTColor1(const mdt_t c) override
  {
    if (addr_y >= 0 && addr_y < clip.y2 ) {
      int x = addr_x + ip;
      if (x >= 0 && x < clip.x2) {
        int i = addr_y * clip.width() + x;
        uint8_t* p = &buf[i * MDT_SIZE];
        if (MDT_SIZE > 2) {
          *p++ = c >> 16;
        }
        *p++ = c >> 8;
        *p++ = c;
      }
    }
    if (++ip >= addr_w) {
      ++addr_y;
      ip = 0;
    }
  }

private:
  rgb_t bgcolor;
  int16_t addr_x, addr_y, addr_w, addr_h;   // set by writeAddrWindow
  int16_t ip;
  uint8_t* buf = 0;

  void initialize(rgb_t aBgColor)
  {
    bgcolor = aBgColor;
    buf = (uint8_t*)malloc(clip.width() * clip.height() * MDT_SIZE);
    clear(bgcolor);
  }

public:
  void clear(rgb_t color) {
    if (buf) {
      const mdt_t c = mdt_color(color);
      const uint8_t* e = &buf[clip.width() * clip.height() * MDT_SIZE];
      uint8_t* p = buf;
      while (p < e ) {
        if (MDT_SIZE > 2) {
          *p++ = c >> 16;
        }
        *p++ = c >> 8;
        *p++ = c;
      }
    }
  }

public:
  void adjust(clip_t& aClip) {
    if (
      clip.x1 == aClip.x1 &&
      clip.y1 == aClip.y1 &&
      clip.x2 == aClip.x2 &&
      clip.y2 == aClip.y2
    ) {}
    else {
      clip = aClip;
      buf = (uint8_t*)realloc(buf, clip.width() * clip.height() * MDT_SIZE);
      clear(bgcolor);
    }
  }

  void drawMDTBuffer(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const uint8_t* buffer) override
  {
    memcpy(&buf[(y * clip.width() + x) * MDT_SIZE], buffer, w * h * MDT_SIZE);
  }

  void push(TSD_SCREEN* screen)
  {
    screen->pushMDTBuffer(clip, (const uint8_t*)buf);
  }

};
