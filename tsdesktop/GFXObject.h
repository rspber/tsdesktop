/*
  GFXObject

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TSDesktop.h>
#include <overlaid.h>

/// @GFXObject
class GFXObject {
public:
  GFXObject(const int16_t ax1, const int16_t ay1, rgb_t acolor)
  {
    x1 = ax1;
    y1 = ay1;
    over.color = acolor;
    over.mode = 0;
    over.buf = 0;
    over.len = 0;
  }
  virtual ~GFXObject()
  {
    if (over.buf) {
      free(over.buf);
      over.buf = 0;
      over.size = 0;
      over.len = 0;
      over.mode = 0;
    }
  }

  void setVisible(const bool aVisible) { visible = aVisible; }
  const bool getVisible() { return visible; }

  void setColor(rgb_t acolor) { over.color = acolor; }
  rgb_t getColor() { return over.color; }

  void setOverlaid(bool aOverlaid);
  rgb_t getOver();

  void hide();
  void draw();

  virtual void dodraw(clip_t& clip, int16_t x, int16_t y) = 0;
  void doDraw(clip_t& clip, int16_t x, int16_t y, bool redraw);

  TSD_SCREEN* writer();

  void setX1(int16_t x) { x1 = x; }
  int16_t getX1() { return x1; }

  void setY1(int16_t y) { y1 = y; }
  int16_t getY1() { return y1; }

  virtual int16_t lW() { return 0; }
  virtual int16_t rW() { return 0; }
  virtual int16_t tH() { return 0; }
  virtual int16_t bH() { return 0; }

protected:
  virtual void setDefaultMaxs(clip_t& clip);
  virtual void animate1(clip_t& clip);
  virtual bool animated(clip_t& clip);

  int16_t x1, y1;

// animate
  uint8_t animate = 0;
  int16_t dx, dy;
  int16_t minx = 0, miny = 0, maxx = 0, maxy = 0;

  friend class GFXButton;
private:
  Button* gfxbtnparent;   // set by GFXButton add
  bool visible = true;
  bool wasDrawn = false;
  bool alloc = false;

  over_t over;
};

