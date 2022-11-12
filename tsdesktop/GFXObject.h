/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <TSDesktop.h>

/// @GFXObject
class GFXObject {
public:
  GFXObject(Button* aparent, rgb_t acolor) : parent(aparent), color(acolor) {}

  void setVisible(const bool aVisible) { visible = aVisible; }
  const bool getVisible() { return visible; }

  void setColor(rgb_t acolor) { color = acolor; }
  rgb_t getColor() { return color; }

  void hide();
  void draw();

  virtual void dodraw(clip_t* clip, int16_t x, int16_t y, rgb_t color) = 0;
  void doDraw(clip_t* clip, int16_t x, int16_t y, const bool redraw);

private:
  void doDraw(rgb_t color);

private:
  Button* parent;
  rgb_t color;
  bool visible = true;
  bool wasDrawn = false;
};
