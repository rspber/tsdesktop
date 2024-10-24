/*
  BufferedDisplay

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

  ATTENTION

  It may happen that there is not enough memory on the system to allocate BufferedDisplay.
  And, for example, on ESP32 this will result in a stack trace print to Serial and a system restart.

*/

#pragma once

#include "TSD_SCREEN.h"

class BufferedDisplay : public TSD_SCREEN {
public:

  /**
   * BufferedDisplay constructor
   * @aClip - init display position & sizes,
   *       this causes the memory (internal buffer) allocation width * height * MDT_SIZE
   */
  BufferedDisplay(clip_t& aClip, const rgb_t aBgColor = BLACK);

  /**
   * BufferedDisplay constructor
   * @x1     - start x position
   * @y1     - start y position
   * @x2     - width = x2 - x1
   * @y2     - height = y2 - y1
   *      this causes the memory (internal buffer) allocation width * height * MDT_SIZE
   */
  BufferedDisplay(const int16_t x1, const int16_t y1, const int16_t x2, const int16_t y2, const rgb_t aBgColor = BLACK);

  virtual ~BufferedDisplay();

  void createPalette(const rgb_t aColorMap[], const int8_t aMapLen);
  void setColorDepth(int8_t b);

  /**
   * color index in palette 
   */
  uint8_t paletteIdx(rgb_t color);

  /**
   * set window (this BufferedDisplay object) position on absolute screen
   * @x     - x (left)
   * @y     - y (top)
   */
  void setPos(const int16_t x, const int16_t y);

  /**
   * clear - speedup function to clear entire internal buffer with color
   */
  void clear(const rgb_t aBgColor = BLACK);

  /**
   * adjust - can be used to resize internal buffer on the fly
   */
  void adjust(clip_t& aClip);

  /**
   * getColor - local manipulation function
   *   get color from internal buffer at (x,y) position in this buffer
   */
  rgb_t getColor(const int x, const int y);

  /**
   * setColor - local manipulation function
   *   set color in internal buffer at (x,y) position in this buffer
   */
  void setColor(const int x, const int y, const rgb_t color);

  /**
   * rotateRight - simple square bit to bit rotation in internal buffer
   *  from (x,y) position, d pixels size
   */
  void rotateRight(const int16_t x, const int16_t y, int16_t d);

  /**
   *  rotateLeft - simple square bit to bit rotation in internal buffer
   *   from (x,y) position, d pixels size
   */
  void rotateLeft(const int16_t x, const int16_t y, int16_t d);

  /**
   *  flipHorizontal - simple bit to bit flip horizontal in internal buffer
   *   from (x,y) position, (w, h) pixels size
   */
  void flipHorizontal(const int16_t x, const int16_t y, int16_t w, int16_t h);

  /**
   * flipVertical - simple bit to bit flip vertical in internal buffer
   *  from (x,y) position, (w, h) pixels size
   */
  void flipVertical(const int16_t x, const int16_t y, int16_t w, int16_t h);

  /**
   * push, draw this object (BufferedDisplay) on screen
   *  at setPos(x,y) absolute position
   */
  void push(TSD_SCREEN* screen);

  /**
   * pushTransp, push transparent, 'transparent' color pixels from this object (BufferedDisplay) will be omitted
   */
  void pushTransp(TSD_SCREEN* screen, const rgb_t transparent);


public:
  /**
   * startWrite - from tft interface, not to use by user
   */
  void startWrite() override {}

  /**
   *  endWrite - from tft interface, not to use by user
   */
  void endWrite() override {}

  /**
   * writeAddrWindow - from tft interface, not to use by user
   */
  void writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h) override;

  /**
   * writeMDTBuffer - from tft interface, not to use by user
   */
  void writeMDTBuffer(const uint8_t* buffer, const int32_t len) override;

  /**
   *  sendMDTColor1 - from tft interface, not to use by user
   */
  void sendMDTColor1(const mdt_t c) override;

  /**
   * drawMDTBuffer - from tft interface, not to use by user
   */
  void drawMDTBuffer(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const uint8_t* buffer) override;

  rgb_t readPixel(clip_t& clip, int16_t x, int16_t y) override;

private:
  void initialize(const rgb_t aBgColor);
  void recreate();

  int16_t addr_x, addr_y, addr_w, addr_h;   // set by writeAddrWindow
  int16_t ip;
  uint8_t* buf = 0;

  const rgb_t* colorMap;
  int8_t cMapLen;

  int8_t bpp;
};
