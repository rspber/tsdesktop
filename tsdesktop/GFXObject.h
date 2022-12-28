/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <TSDesktop.h>

/// @GFXObject
class GFXObject {
public:
  GFXObject(Button* aparent, rgb_t acolor) : parent(aparent)
  {
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

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y) = 0;
  void doDraw(clip_t* clip, int16_t x, int16_t y, const bool redraw);

private:
  void doDraw();

private:
  Button* parent;
  bool visible = true;
  bool wasDrawn = false;

protected:
  over_t over;
};

