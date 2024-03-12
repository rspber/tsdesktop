/*
  Canvas

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#pragma once

#include "GfxObject.h"
#include "BufferedDisplay.h"

class Canvas;

/// @CanvasGfxObject

class CanvasGfxObject : public GfxObject {

  friend class Canvas;

public:
  CanvasGfxObject(const int16_t ax1, const int16_t ay1, rgb_t acolor)
  : GfxObject(ax1, ay1, acolor) {}

  // parent canvas in which this GfxObject resides
  Canvas* getCanvas() { return canvas; }

  void hide() override;
  void draw() override;

  TSD_SCREEN* screen() override;

private:
  Canvas* canvas;   // set by Canvas.add
};


/// @Canvas

class Canvas : public GfxObject {
public:
  Canvas(const int16_t ax1, const int16_t ay1, rgb_t acolor = BLACK) : GfxObject(ax1, ay1, acolor) {}
  virtual ~Canvas();

  void setBuffered(const bool aBuffered);
  bool getBuffered() { return buffered; }

  CanvasGfxObject* remove(CanvasGfxObject* item);
  CanvasGfxObject* remove(int16_t index);

  int16_t index(CanvasGfxObject* item);

  void add(CanvasGfxObject* item);
  void add(CanvasGfxObject& item);

  void animate(CanvasGfxObject& item, const int16_t dx, int16_t dy);

  // absolute location of canvas in screen()
  virtual void getInnerClip(clip_t& clip) = 0;

  void dodraw(clip_t& clip, int16_t left, int16_t top) override;

  void drawObject(GfxObject* g);

  void hide() override;
  void draw() override;

  void innerDraw(bool redraw = false);

  BufferedDisplay* bf();

  virtual TSD_SCREEN* superScreen() = 0;
  TSD_SCREEN* screen() override;

private:
  CanvasGfxObject** list = NULL;
  int16_t len = 0, count = 0;
  bool buffered = false;
  BufferedDisplay* bf_ = 0;
};
