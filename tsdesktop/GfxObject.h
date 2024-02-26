/*
  GfxObject

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#pragma once

#include <overlaid.h>
#include "Canvas.h"
#include "TSD_SCREEN.h"

/// @GfxObject

class Canvas;

class GfxObject {

  friend class Canvas;

public:
  GfxObject(const int16_t ax1, const int16_t ay1, rgb_t acolor);

  virtual ~GfxObject();

  void setVisible(const bool aVisible) { visible = aVisible; }
  const bool getVisible() { return visible; }

  void setColor(rgb_t acolor) { over.color = acolor; }
  rgb_t getColor() { return over.color; }

  void setOverlaid(bool aOverlaid);
  rgb_t getOver();

  virtual rgb_t getBackgroundColor() { return BLACK; }

  void hide();
  void draw();

  virtual void dodraw(clip_t& clip, int16_t left, int16_t top) = 0;
  void doDraw(clip_t& clip, int16_t left, int16_t top, bool redraw);

  virtual TSD_SCREEN* screen();

  // parent canvas in which this GfxObject resides
  Canvas* getCanvas() { return canvas; }

  // this GfxObject dislocation in canvas
  void setX1(int16_t x) { x1 = x; }
  int16_t getX1() { return x1; }

  // this GfxObject dislocation in canvas
  void setY1(int16_t y) { y1 = y; }
  int16_t getY1() { return y1; }

  virtual int16_t lW() = 0;
  virtual int16_t rW() = 0;
  virtual int16_t tH() = 0;
  virtual int16_t bH() = 0;

protected:
  virtual void setDefaultMaxs(clip_t& clip);
  virtual void animate1(clip_t& clip);
  virtual bool animated(clip_t& clip);

  int16_t x1, y1;

// animate
  uint8_t animate = 0;
  int16_t dx, dy;
  int16_t minx = 0, miny = 0, maxx = 0, maxy = 0;

private:
  Canvas* canvas;   // set by Canvas.add

  bool visible = true;
  bool wasDrawn = false;
  bool alloc = false;

  over_t over;
};

