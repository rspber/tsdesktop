/*
  BufferedDisplay

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

  ATTENTION

  It may happen that there is not enough memory on the system to allocate BufferedDisplay.
  And, for example, on ESP32 this will result in a stack trace print to Serial and a system restart.

*/

#include "BufferedDisplay.h"
#include <stdlib.h>
#include <string.h>

//#include <Arduino.h>

  /**
   * BufferedDisplay constructor
   * @aClip - init display position & sizes,
   *       this causes the memory (internal buffer) allocation width * height * MDT_SIZE
   */
  BufferedDisplay::BufferedDisplay(clip_t& aClip, const rgb_t aBackgroundColor)
    : TSD_SCREEN(aClip.width(), aClip.height())
  {
    clip = aClip;
    initialize(aBackgroundColor);
  }

  /**
   * BufferedDisplay constructor
   * @x1     - start x position
   * @y1     - start y position
   * @x2     - width = x2 - x1
   * @y2     - height = y2 - y1
   *      this causes the memory (internal buffer) allocation width * height * MDT_SIZE
   */
  BufferedDisplay::BufferedDisplay(const int16_t x1, const int16_t y1, const int16_t x2, const int16_t y2, const rgb_t aBackgroundColor)
    : TSD_SCREEN(x2 - x1, y2 - y1)
  {
    clip = {x1, y1, x2, y2};
    initialize(aBackgroundColor);
  }

  BufferedDisplay::~BufferedDisplay()
  {
    free(buf);
    buf = 0;
  }

  /**
   * called by constructor
   */
  void BufferedDisplay::initialize(const rgb_t aBgColor)
  {
    bgcolor = aBgColor;
    buf = (uint8_t*)malloc(clip.width() * clip.height() * MDT_SIZE);
/*
    if (!buf) {
      Serial.println("No memory for BufferedDisplay\");
    }
*/
    clear(bgcolor);
  }

  /**
   * adjust - can be used to resize internal buffer on the fly
   */
  void BufferedDisplay::adjust(clip_t& aClip)
  {
    if (
      clip.x1 == aClip.x1 &&
      clip.y1 == aClip.y1 &&
      clip.x2 == aClip.x2 &&
      clip.y2 == aClip.y2
    ) {}
    else {
      clip = aClip;
      buf = (uint8_t*)realloc(buf, clip.width() * clip.height() * MDT_SIZE);
/*
      if (!buf) {
        Serial.println("No memory for BufferedDisplay\");
      }
*/
      clear(bgcolor);
    }
  }

  /**
   * clear - speedup function to clear entire internal buffer with color
   */
  void BufferedDisplay::clear(const rgb_t color)
  {
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

  /**
   * set window (this BufferedDisplay object) position on absolute screen
   * x      - x (left)
   * y      - y (top)
   */
  void BufferedDisplay::setPos(const int16_t x, const int16_t y)
  {
    clip.x2 = clip.x2 - clip.x1 + x;
    clip.x1 = x;
    clip.y2 = clip.y2 - clip.y1 + y;
    clip.y1 = y;
  }

  /**
   * writeAddrWindow - from tft interface, not to use by user
   */
  void BufferedDisplay::writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
  {
    addr_x = x - clip.x1;
    addr_y = y - clip.y1;
    addr_w = w;
    addr_h = h;
    ip = 0;
  }

  /**
   *  sendMDTColor1 - from tft interface, not to use by user
   */
  void BufferedDisplay::sendMDTColor1(const mdt_t c)
  {
//    if (!buf) {
//    return;
//    }
    if (addr_y >= 0 && addr_y < clip.height() ) {
      int x = addr_x + ip;
      if (x >= 0 && x < clip.width()) {
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

  /**
   * getMDTColor - local manipulation function
   *   get mdt color from internal buffer at (x,y) position in this buffer
   *   color is in display transer mode format
   */
  mdt_t BufferedDisplay::getMDTColor(const int x, const int y)
  {
//    if (!buf) {
//    return 0;
//    }

    mdt_t c = 0;
    int i = (y * clip.width() + x) * MDT_SIZE;
    c = buf[i];
    c |= buf[i+1] << 8;
    if (MDT_SIZE > 2) {
      c |= buf[i+2] << 16;
    }
    return c;
  }

  /**
   * setMDTColor - local manipulation function
   *   set mdt color in internal buffer at (x,y) position in this buffer
   *   color is in display transer mode format
   */
  void BufferedDisplay::setMDTColor(const int x, const int y, const mdt_t c)
  {
//    if (!buf) {
//    return;
//    }

    int i = (y * clip.width() + x) * MDT_SIZE;
    buf[i] = c;
    buf[i+1] = c >> 8;
    if (MDT_SIZE > 2) {
      buf[i+2] = c >> 16;
    }
  }

  /**
   * rotateRight - simple square bit to bit rotation in internal buffer
   *  from (x,y) position, d pixels size
   */
  void BufferedDisplay::rotateRight(const int16_t x, const int16_t y, int16_t d)
  {
    if (x < 0 || y < 0) {
      return;
    }
    if (x + d > clip.width() ) {
      d = clip.width() - x;
    }
    if (y + d > clip.height() ) {
      d = clip.height() - y;
    }
    int h = 0;
    while (d > 2) {
      --d;
      for (int i = h; i < d; ++i) {
        mdt_t tmp                       = getMDTColor(x + i, y + h);
        setMDTColor(x + i, y + h,         getMDTColor(x + h, y + d - (i-h))); 
        setMDTColor(x + h, y + d - (i-h), getMDTColor(x + d - (i-h), y + d));
        setMDTColor(x + d - (i-h), y + d, getMDTColor(x + d, y + i));
        setMDTColor(x + d, y + i, tmp);
      }
      ++h;
    }
  }

  /**
   *  rotateLeft - simple square bit to bit rotation in internal buffer
   *   from (x,y) position, d pixels size
   */
  void BufferedDisplay::rotateLeft(const int16_t x, const int16_t y, int16_t d)
  {
    if (x < 0 || y < 0) {
      return;
    }
    if (x + d > clip.width() ) {
      d = clip.width() - x;
    }
    if (y + d > clip.height() ) {
      d = clip.height() - y;
    }
    int h = 0;
    while (d > 2) {
      --d;
      for (int i = h; i < d; ++i) {
        mdt_t tmp                       = getMDTColor(x + i, y + h);
        setMDTColor(x + i, y + h,         getMDTColor(x + d, y + i)); 
        setMDTColor(x + d, y + i,         getMDTColor(x + d - (i-h), y + d));
        setMDTColor(x + d - (i-h), y + d, getMDTColor(x + h, y + d - (i-h)) );
        setMDTColor(x + h, y + d - (i-h), tmp);
      }
      ++h;
    }
  }

  /**
   *  flipHorizontal - simple bit to bit flip horizontal in internal buffer
   *   from (x,y) position, (w, h) pixels size
   */
  void BufferedDisplay::flipHorizontal(const int16_t x, const int16_t y, int16_t w, int16_t h)
  {
    if (x < 0 || y < 0) {
      return;
    }
    if (x + w > clip.width() ) {
      w = clip.width() - x;
    }
    if (y + h > clip.height() ) {
      h = clip.height() - y;
    }
    for (int j = 0; j < h; ++j ) {
      int w2 = w/2;
      for (int i = 0; i <= w2; ++i) {
        mdt_t tmp               = getMDTColor(x + i, y + j);
        setMDTColor(x + i, y + j, getMDTColor(x + w - i - 1, y + j)); 
        setMDTColor(x + w - i - 1, y + j, tmp);
      }
    }
  }

  /**
   * flipVertical - simple bit to bit flip vertical in internal buffer
   *  from (x,y) position, (w, h) pixels size
   */
  void BufferedDisplay::flipVertical(const int16_t x, const int16_t y, int16_t w, int16_t h)
  {
    if (x < 0 || y < 0) {
      return;
    }
    if (x + w > clip.width() ) {
      w = clip.width() - x;
    }
    if (y + h > clip.height() ) {
      h = clip.height() - y;
    }
    for (int i = 0; i < w; ++i) {
      int h2 = h/2;
      for (int j = 0; j <= h2; ++j ) {
        mdt_t tmp               = getMDTColor(x + i, y + j);
        setMDTColor(x + i, y + j, getMDTColor(x + i, y + h - j - 1)); 
        setMDTColor(x + i, y + h - j - 1, tmp);
      }
    }
  }

  /**
   * drawMDTBuffer - from tft interface, not to use by user
   */
  void BufferedDisplay::drawMDTBuffer(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const uint8_t* buffer)
  {
//    if (buf) {
      memcpy(&buf[((y - clip.y1) * clip.width() + (x - clip.x1)) * MDT_SIZE], buffer, w * h * MDT_SIZE);
//    }
  }

  /**
   * push, draw this object (BufferedDisplay) on screen
   *  at absolute setPos(x,y) position
   */
  void BufferedDisplay::push(TSD_SCREEN* screen)
  {
//    if (buf) {
      screen->pushMDTBuffer(clip, (const uint8_t*)buf);
//    }
  }

  /**
   * pushTransp, push transparent, 'transparent' color pixels from this object (BufferedDisplay) will be omitted
   */
  void BufferedDisplay::pushTransp(TSD_SCREEN* screen, const rgb_t transparent)
  {
//    if (buf) {
      screen->pushMDTBuffer(clip, (const uint8_t*)buf, transparent);
//    }
  }
