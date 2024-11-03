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

// Default palette for 4-bit colour sprites
static const rgb_t default_4bit_palette[] = {
  BLACK,    //  0  ^
  BROWN,    //  1  |
  RED,      //  2  |
  ORANGE,   //  3  |
  YELLOW,   //  4  Colours 0-9 follow the resistor colour code!
  GREEN,    //  5  |
  BLUE,     //  6  |
  PURPLE,   //  7  |
  GREY,     //  8  |
  WHITE,    //  9  v
  CYAN,     // 10  Blue+green mix
  MAGENTA,  // 11  Blue+red mix
  MAROON,   // 12  Darker red colour
  DARK_GREEN,// 13  Darker green colour
  NAVY,     // 14  Darker blue colour
  PINK      // 15
};

//#include <Arduino.h>

  /**
   * BufferedDisplay constructor
   * @aClip - init display position & sizes,
   *       this causes the memory (internal buffer) allocation width * height * MDT_SIZE
   */
  BufferedDisplay::BufferedDisplay(clip_t& aClip, const rgb_t aBgColor)
    : TSD_SCREEN(aClip.width(), aClip.height())
  {
    clip = aClip;
    recreate();
    clear(aBgColor);
  }

  /**
   * BufferedDisplay constructor
   * @x1     - start x position
   * @y1     - start y position
   * @x2     - width = x2 - x1
   * @y2     - height = y2 - y1
   *      this causes the memory (internal buffer) allocation width * height * MDT_SIZE
   */
  BufferedDisplay::BufferedDisplay(const int32_t x1, const int32_t y1, const int32_t x2, const int32_t y2, const rgb_t aBgColor)
    : TSD_SCREEN(x2 - x1, y2 - y1)
  {
    clip = {x1, y1, x2, y2};
    recreate();
    clear(aBgColor);
  }

  BufferedDisplay::~BufferedDisplay()
  {
    free(buf);
    buf = 0;
  }

  void BufferedDisplay::recreate()
  {
    buf = (uint8_t*)realloc(buf, clip.width() * clip.height() * MDT_SIZE);
/*
      if (!buf) {
        Serial.println("No memory for BufferedDisplay\");
      }
*/
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
      recreate();
//      clear(bgcolor);
    }
  }

  /**
   * clear - speedup function to clear entire internal buffer with color
   */
  void BufferedDisplay::clear(const rgb_t color)
  {
    if (!buf) {
      return;
    }
    mdt_t c = mdt_color(color);
    uint8_t*p = buf;
    for (int i = width() * height(); --i >= 0; ) {
      if (MDT_SIZE > 2) {
        *p++ = c >> 16;
      }
      *p++ = c >> 8;
      *p++ = c;
    }
  }

  /**
   * set window (this BufferedDisplay object) position on absolute screen
   * x      - x (left)
   * y      - y (top)
   */
  void BufferedDisplay::setPos(const int32_t x, const int32_t y)
  {
    clip.x2 = clip.x2 - clip.x1 + x;
    clip.x1 = x;
    clip.y2 = clip.y2 - clip.y1 + y;
    clip.y1 = y;
  }

  /**
   * writeAddrWindow - from tft interface, not to use by user
   */
  void BufferedDisplay::writeAddrWindow(const int32_t x, const int32_t y, const int32_t w, const int32_t h)
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
    if (!buf) {
      return;
    }
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
  rgb_t BufferedDisplay::getColor(const int x, const int y)
  {
    if (!buf) {
      return 0;
    }

    mdt_t c = 0;
    int i = (y * clip.width() + x) * MDT_SIZE;
    c = buf[i];
    c |= buf[i+1] << 8;
    if (MDT_SIZE > 2) {
      c |= buf[i+2] << 16;
    }
    return rgb(c);
  }

  /**
   * setMDTColor - local manipulation function
   *   set mdt color in internal buffer at (x,y) position in this buffer
   *   color is in display transer mode format
   */
  void BufferedDisplay::setColor(const int x, const int y, const rgb_t color)
  {
    if (!buf) {
      return;
    }

    int i = (y * clip.width() + x) * MDT_SIZE;
    mdt_t c = mdt_color(color);
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
  void BufferedDisplay::rotateRight(const int32_t x, const int32_t y, int32_t d)
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
        rgb_t tmp                    = getColor(x + i, y + h);
        setColor(x + i, y + h,         getColor(x + h, y + d - (i-h))); 
        setColor(x + h, y + d - (i-h), getColor(x + d - (i-h), y + d));
        setColor(x + d - (i-h), y + d, getColor(x + d, y + i));
        setColor(x + d, y + i, tmp);
      }
      ++h;
    }
  }

  /**
   *  rotateLeft - simple square bit to bit rotation in internal buffer
   *   from (x,y) position, d pixels size
   */
  void BufferedDisplay::rotateLeft(const int32_t x, const int32_t y, int32_t d)
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
        rgb_t tmp                    = getColor(x + i, y + h);
        setColor(x + i, y + h,         getColor(x + d, y + i)); 
        setColor(x + d, y + i,         getColor(x + d - (i-h), y + d));
        setColor(x + d - (i-h), y + d, getColor(x + h, y + d - (i-h)) );
        setColor(x + h, y + d - (i-h), tmp);
      }
      ++h;
    }
  }

  /**
   *  flipHorizontal - simple bit to bit flip horizontal in internal buffer
   *   from (x,y) position, (w, h) pixels size
   */
  void BufferedDisplay::flipHorizontal(const int32_t x, const int32_t y, int32_t w, int32_t h)
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
        rgb_t tmp            = getColor(x + i, y + j);
        setColor(x + i, y + j, getColor(x + w - i - 1, y + j)); 
        setColor(x + w - i - 1, y + j, tmp);
      }
    }
  }

  /**
   * flipVertical - simple bit to bit flip vertical in internal buffer
   *  from (x,y) position, (w, h) pixels size
   */
  void BufferedDisplay::flipVertical(const int32_t x, const int32_t y, int32_t w, int32_t h)
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
        rgb_t tmp            = getColor(x + i, y + j);
        setColor(x + i, y + j, getColor(x + i, y + h - j - 1)); 
        setColor(x + i, y + h - j - 1, tmp);
      }
    }
  }

  /**
   * drawMDTBuffer - from tft interface, not to use by user
   */
  void BufferedDisplay::drawMDTBuffer(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t* buffer)
  {
    if (!buf) {
      return;
    }
    const uint8_t* p = buffer;
    int16_t dy = y - clip.y1;
    if (dy < 0) {
      p += -dy * MDT_SIZE;
      h += dy;
      dy = 0;
    }
    int16_t dx = x - clip.x1;
    if (dx < 0) {
      w += dx;
      for (int j = 0; j < addr_h; ++j) {
        int32_t offs = dy * clip.width();
        memcpy(&buf[offs * MDT_SIZE], p - dx * MDT_SIZE, w * MDT_SIZE);
        p += addr_w * MDT_SIZE;
      } 
    }
    else {
      int32_t offs = dy * clip.width() + dx;
      memcpy(&buf[offs * MDT_SIZE], p, w * h * MDT_SIZE);
    }
  }

  /**
   * writeMDTBuffer - from tft interface, not to use by user
   */
  void BufferedDisplay::writeMDTBuffer(const uint8_t* buffer, const int32_t len)
  {
    drawMDTBuffer(addr_x + clip.x1, addr_y + clip.y1, addr_w, addr_h, buffer);
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

  rgb_t BufferedDisplay::readPixel(clip_t& cli, int16_t x, int16_t y)
  {
    if (x >= clip.x1 && x < clip.x2 && y >= clip.y1 && y < clip.y2 ) {
      return getColor(x, y);
    }
    return BLACK;
  }
