/*
  BufferedDisplay

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include "TSD_SCREEN.h"

class BufferedDisplay : public TSD_SCREEN {
public:

  BufferedDisplay(clip_t& clip) : TSD_SCREEN(clip.width(), clip.height())
  {
    init(clip);
  }

  virtual ~BufferedDisplay()
  {
    free(buf);
    buf = 0;
  }

  void startWrite() override {}
  void endWrite() override {}
  void drawMDTBuffer(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const uint8_t* buffer) override {}

  void writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h) override
  {
    ax = x - x1;
    ay = y - y1;
    aw = w;
    ah = h;
    ip = 0;
  }

  void sendMDTColor1(const mdt_t c) override
  {
    uint8_t* p = &buf[(ay * w + ax + ip) * MDT_SIZE];
    if (MDT_SIZE > 2) {
      *p++ = c >> 16;
    }
    *p++ = c >> 8;
    *p++ = c;
    if (++ip >= aw) {
      ++ay;
      ip = 0;
    }
  }

private:
  int16_t x1, y1, w, h;
  int16_t ax, ay, aw, ah;   // set by writeAddrWindow
  int16_t ip;
  uint8_t* buf = 0;

public:
  void setSizes(clip_t& clip) {
    x1 = clip.x1;
    y1 = clip.y1;
    w = clip.width();
    h = clip.height();
  }

  void init(clip_t& clip) {
    setSizes(clip);
    buf = (uint8_t*)malloc(w * h * MDT_SIZE);
  }

public:
  void adjust(clip_t& clip) {
    if (
      x1 == clip.x1 &&
      y1 == clip.y1 &&
      w == clip.width() &&
      h == clip.height()
    ) {}
    else {
      setSizes(clip);
      buf = (uint8_t*)realloc(buf, w * h * MDT_SIZE);
    }
  }

  void outMeTo(TSD_SCREEN* screen)
  {
    screen->drawMDTBuffer(x1, y1, w, h, (const uint8_t*)buf);
  }

};
