/*
  Graphics library for TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

  Primary code comes from Adafruit's core graphics library: Adafruit_GFX,
  but was significantly modified or replaced by those from TFT_eSP library.

  Originally Adafruit's license in file /licenses/Adafruit_gfx_license.txt

  Originally Bodmers's license in file /licenses/Bodmer_license.txt

*/

#include "TSD_GFX.h"
#include "overlaid.h"
#include <stdlib.h>
#include <math.h>

#ifndef abs
#define abs(x) ((x)>=0?(x):-(x))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define SWAP_INT(a, b) \
  {            \
    int32_t t = a; \
    a = b;     \
    b = t;     \
  }

#define CLIP_X \
  if (x < clip.x1) { w -= clip.x1 - x; x = clip.x1; } \
  if (x + w > clip.x2) { w = clip.x2 - x; }

#define CLIP_Y \
  if (y < clip.y1) { h -= clip.y1 - y; y = clip.y1; } \
  if (y + h > clip.y2) { h = clip.y2 - y; }

#define IF_CLIP_X x >= clip.x1 && x < clip.x2

#define IF_CLIP_Y y >= clip.y1 && y < clip.y2

// Clipping macro for pushImage
#define PI_CLIP                                        \
  if (x >= clip.x2 || y >= clip.y2) return;            \
                                                       \
  int32_t dx = 0;                                      \
  int32_t dy = 0;                                      \
  int32_t dw = w;                                      \
  int32_t dh = h;                                      \
                                                       \
  if (x < clip.x1) { dx = clip.x1 - x; dw -= dx; x = clip.x1; } \
  if (y < clip.y1) { dy = clip.y1 - y; dh -= dy; y = clip.y1; } \
                                                       \
  if ((x + dw) > clip.x2 ) dw = clip.x2 - x;                 \
  if ((y + dh) > clip.y2 ) dh = clip.y2 - y;                 \
                                                       \
  if (dw < 1 || dh < 1) return;

// Swap any type
template <typename T> static inline void
transpose(T& a, T& b) { T t = a; a = b; b = t; }



/***************************************************************************************
** Function name:           color24to16
** Description:             convert 24-bit colour to a 16-bit 565 colour value
***************************************************************************************/
inline uint16_t color24to16(rgb_t color888)
{
  uint16_t r = (color888 >> 8) & 0xF800;
  uint16_t g = (color888 >> 5) & 0x07E0;
  uint16_t b = (color888 >> 3) & 0x001F;

  return (r | g | b);
}

/**************************************************************************/

/***************************************************************************************
** Function name:           pushImage
** Description:             plot 16-bit colour sprite or image onto TFT
***************************************************************************************/
void TSD_GFX::pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data)
{
  PI_CLIP;

//  begin_tft_write();
//  inTransaction = true;
  startWrite();

  writeAddrWindow(x, y, dw, dh);

  data += dx + dy * w;

  // Check if whole image can be pushed
  // pushPixels
  if (dw == w) {
    writeMDTBuffer((const uint8_t*)data, dw * dh);
  }
  else {
    // Push line segments to crop image
    while (dh--) {
      // pushPixels
      writeMDTBuffer((const uint8_t*)data, dw);
      data += w;
    }
  }

//  inTransaction = lockTransaction;
//  end_tft_write();
  endWrite();
}

/***************************************************************************************
** Function name:           pushImage
** Description:             plot 16-bit sprite or image with 1 colour being transparent
***************************************************************************************/
void TSD_GFX::pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, rgb_t transp)
{
  PI_CLIP;

//  begin_tft_write();
//  inTransaction = true;
  startWrite();

  data += dx + dy * w;


  uint16_t  lineBuf[dw]; // Use buffer to minimise setWindow call count

  // The little endian transp color must be byte swapped if the image is big endian
//  if (!_swapBytes) transp = transp >> 8 | transp << 8;

  while (dh--) {
    int32_t len = dw;
    uint16_t* ptr = data;
    int32_t px = x, sx = x;
    bool move = true;
    uint16_t np = 0;

    while (len--) {
      if (transp != *ptr) {
        if (move) { move = false; sx = px; }
        lineBuf[np] = *ptr;
        np++;
      }
      else {
        move = true;
        if (np) {
          writeAddrWindow(sx, y, np, 1);
//          pushPixels((uint16_t*)lineBuf, np);
          writeMDTBuffer((const uint8_t*)lineBuf, np);
          np = 0;
        }
      }
      px++;
      ptr++;
    }
    if (np) {
      writeAddrWindow(sx, y, np, 1);
//      pushPixels((uint16_t*)lineBuf, np);
      writeMDTBuffer((const uint8_t*)lineBuf, np);
    }
    y++;
    data += w;
  }

//  inTransaction = lockTransaction;
//  end_tft_write();
  endWrite();
}


/***************************************************************************************
** Function name:           pushImage - for FLASH (PROGMEM) stored images
** Description:             plot 16-bit image
*************************************************************************************** /
void TSD_GFX::pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data)
{
  // Requires 32-bit aligned access, so use PROGMEM 16-bit word functions
  PI_CLIP;

//  begin_tft_write();
//  inTransaction = true;
  startWrite();

  data += dx + dy * w;

  uint16_t  buffer[dw];

  writeAddrWindow(x, y, dw, dh);

  // Fill and send line buffers to TFT
  for (int32_t i = 0; i < dh; i++) {
    for (int32_t j = 0; j < dw; j++) {
      buffer[j] = pgm_read_word(&data[i * w + j]);
    }
//    pushPixels(buffer, dw);
    writeMDTBuffer((const uint8_t*)buffer, dw);
  }

//  inTransaction = lockTransaction;
//  end_tft_write();
  endWrite();
}
*/
/***************************************************************************************
** Function name:           pushImage - for FLASH (PROGMEM) stored images
** Description:             plot 16-bit image with 1 colour being transparent
*************************************************************************************** /
void TSD_GFX::pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, rgb_t transp)
{
  // Requires 32-bit aligned access, so use PROGMEM 16-bit word functions
  PI_CLIP;

//  begin_tft_write();
//  inTransaction = true;
  startWrite();

  data += dx + dy * w;


  uint16_t  lineBuf[dw];

  // The little endian transp color must be byte swapped if the image is big endian
//  if (!_swapBytes) transp = transp >> 8 | transp << 8;

  while (dh--) {
    int32_t len = dw;
    uint16_t* ptr = (uint16_t*)data;
    int32_t px = x, sx = x;
    bool move = true;

    uint16_t np = 0;

    while (len--) {
      uint16_t color = pgm_read_word(ptr);
      if (transp != color) {
        if (move) { move = false; sx = px; }
        lineBuf[np] = color;
        np++;
      }
      else {
        move = true;
        if (np) {
          setWindow(sx, y, sx + np - 1, y);
          pushPixels(lineBuf, np);
          np = 0;
        }
      }
      px++;
      ptr++;
    }
    if (np) {
      setWindow(sx, y, sx + np - 1, y);
      pushPixels(lineBuf, np);
    }
    y++;
    data += w;
  }

//  inTransaction = lockTransaction;
//  end_tft_write();
  endWrite();
}
*/

/***************************************************************************************
** Function name:           pushImage
** Description:             plot 8-bit or 4-bit or 1 bit image or sprite using a line buffer
*************************************************************************************** /
void TSD_GFX::pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8, cmap_t& cmap)
{
  PI_CLIP;

//  begin_tft_write();
//  inTransaction = true;
  startWrite();

//  bool swap = _swapBytes;

  setWindow(x, y, x + dw - 1, y + dh - 1); // Sets CS low and sent RAMWR

  // Line buffer makes plotting faster
  uint16_t  lineBuf[dw];

  if (bpp8) {
//    _swapBytes = false;

    uint8_t  blue[] = {0, 11, 21, 31}; // blue 2 to 5-bit colour lookup table

    int _lastColor = -1; // Set to illegal value

    // Used to store last shifted colour
    uint8_t msbColor = 0;
    uint8_t lsbColor = 0;

    data += dx + dy * w;
    while (dh--) {
      uint32_t len = dw;
      uint8_t* ptr = (uint8_t*)data;
      uint8_t* linePtr = (uint8_t*)lineBuf;

      while(len--) {
        uint8_t c8 = pgm_read_byte(ptr++);

        // Shifts are slow so check if colour has changed first
        if (c8 != _lastColor) {
          //          =====Green=====     ===============Red==============
          msbColor = (c8 & 0x1C)>>2 | (c8 & 0xC0)>>3 | (c8 & 0xE0);
          //          =====Green=====    =======Blue======
          lsbColor = (c8 & 0x1C)<<3 | blue[c8 & 0x03];
          _lastColor = c8;
        }

       *linePtr++ = msbColor;
       *linePtr++ = lsbColor;
      }

      pushPixels(lineBuf, dw);

      data += w;
    }
//    _swapBytes = swap; // Restore old value
  }
  else if (cmap.cMap != nullptr) { // Must be 4bpp
//    _swapBytes = true;

    w = (w+1) & 0xFFFE;   // if this is a sprite, w will already be even; this does no harm.
    bool splitFirst = (dx & 0x01) != 0; // split first means we have to push a single px from the left of the sprite / image

    if (splitFirst) {
      data += ((dx - 1 + dy * w) >> 1);
    }
    else {
      data += ((dx + dy * w) >> 1);
    }

    while (dh--) {
      uint32_t len = dw;
      uint8_t * ptr = (uint8_t*)data;
      uint16_t *linePtr = lineBuf;
      uint8_t colors; // two colors in one byte
      uint16_t index;

      if (splitFirst) {
        colors = pgm_read_byte(ptr);
        index = colors & 0x0F;
        *linePtr++ = color24to16(cmap.cMap[index]);
        len--;
        ptr++;
      }

      while (len--) {
        colors = pgm_read_byte(ptr);
        index = (colors >> 4) & 0x0F;
        *linePtr++ = color24to16(cmap.cMap[index]);

        if (len--) {
          index = colors & 0x0F;
          *linePtr++ = color24to16(cmap.cMap[index]);
        } else {
          break;  // nothing to do here
        }

        ptr++;
      }

      pushPixels(lineBuf, dw);
      data += (w >> 1);
    }
//    _swapBytes = swap; // Restore old value
  }
  else { // Must be 1bpp
//    _swapBytes = false;
    uint8_t * ptr = (uint8_t*)data;
    uint32_t ww =  (w+7)>>3; // Width of source image line in bytes
    for (int32_t yp = dy;  yp < dy + dh; yp++) {
      uint8_t* linePtr = (uint8_t*)lineBuf;
      for (int32_t xp = dx; xp < dx + dw; xp++) {
        uint16_t col = (pgm_read_byte(ptr + (xp>>3)) & (0x80 >> (xp & 0x7)) );
        if (col) {*linePtr++ = bitmap_fg>>8; *linePtr++ = (uint8_t) bitmap_fg;}
        else     {*linePtr++ = bitmap_bg>>8; *linePtr++ = (uint8_t) bitmap_bg;}
      }
      ptr += ww;
      pushPixels(lineBuf, dw);
    }
  }

//  _swapBytes = swap; // Restore old value
//  inTransaction = lockTransaction;
//  end_tft_write();
  endWrite();
}
*/

/***************************************************************************************
** Function name:           pushImage
** Description:             plot 8-bit or 4-bit or 1 bit image or sprite using a line buffer
***************************************************************************************/
void TSD_GFX::pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t *data, bool bpp8, cmap_t& cmap)
{
  PI_CLIP;

  startWrite();

  writeAddrWindow(x, y, dw, dh); // Sets CS low and sent RAMWR

  // Line buffer makes plotting faster
  uint16_t  lineBuf[dw];

  if (bpp8) {
//    _swapBytes = false;

    uint8_t  blue[] = {0, 11, 21, 31}; // blue 2 to 5-bit colour lookup table

    int _lastColor = -1; // Set to illegal value

    // Used to store last shifted colour
    uint8_t msbColor = 0;
    uint8_t lsbColor = 0;

    data += dx + dy * w;
    while (dh--) {
      uint32_t len = dw;
      uint8_t* ptr = data;
      uint8_t* linePtr = (uint8_t*)lineBuf;

      while(len--) {
        uint8_t c8 = *ptr++;

        // Shifts are slow so check if colour has changed first
        if (c8 != _lastColor) {
          //          =====Green=====     ===============Red==============
          msbColor = (c8 & 0x1C)>>2 | (c8 & 0xC0)>>3 | (c8 & 0xE0);
          //          =====Green=====    =======Blue======
          lsbColor = (c8 & 0x1C)<<3 | blue[c8 & 0x03];
          _lastColor = c8;
        }

       *linePtr++ = msbColor;
       *linePtr++ = lsbColor;
      }

//      pushPixels(lineBuf, dw);
      writeMDTBuffer((const uint8_t*)lineBuf, dw);

      data += w;
    }
  }
  else if (cmap.cMap != nullptr) { // Must be 4bpp
//    _swapBytes = true;

    w = (w+1) & 0xFFFE;   // if this is a sprite, w will already be even; this does no harm.
    bool splitFirst = (dx & 0x01) != 0; // split first means we have to push a single px from the left of the sprite / image

    if (splitFirst) {
      data += ((dx - 1 + dy * w) >> 1);
    }
    else {
      data += ((dx + dy * w) >> 1);
    }

    while (dh--) {
      uint32_t len = dw;
      uint8_t * ptr = data;
      uint16_t *linePtr = lineBuf;
      uint8_t colors; // two colors in one byte
      uint16_t index;

      if (splitFirst) {
        colors = *ptr;
        index = (colors & 0x0F);
        *linePtr++ = color24to16(cmap.cMap[index]);
        len--;
        ptr++;
      }

      while (len--) {
        colors = *ptr;
        index = ((colors & 0xF0) >> 4) & 0x0F;
        *linePtr++ = color24to16(cmap.cMap[index]);

        if (len--) {
          index = colors & 0x0F;
          *linePtr++ = color24to16(cmap.cMap[index]);
        } else {
          break;  // nothing to do here
        }

        ptr++;
      }

//      pushPixels(lineBuf, dw);
      writeMDTBuffer((const uint8_t*)lineBuf, dw);
      data += (w >> 1);
    }
  }
  else { // Must be 1bpp
//    _swapBytes = false;
    uint32_t ww =  (w+7)>>3; // Width of source image line in bytes
    for (int32_t yp = dy;  yp < dy + dh; yp++) {
      uint8_t* linePtr = (uint8_t*)lineBuf;
      for (int32_t xp = dx; xp < dx + dw; xp++) {
        uint16_t col = (data[(xp>>3)] & (0x80 >> (xp & 0x7)) );
        if (col) {*linePtr++ = _bitmap_fg>>8; *linePtr++ = (uint8_t) _bitmap_fg;}
        else     {*linePtr++ = _bitmap_bg>>8; *linePtr++ = (uint8_t) _bitmap_bg;}
      }
      data += ww;
//      pushPixels(lineBuf, dw);
      writeMDTBuffer((const uint8_t*)lineBuf, dw);
    }
  }

  endWrite();
}


/***************************************************************************************
** Function name:           pushImage
** Description:             plot 8 or 4 or 1 bit image or sprite with a transparent colour
***************************************************************************************/
void TSD_GFX::pushImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t *data, rgb_t transp, bool bpp8, cmap_t& cmap)
{
  PI_CLIP;

  startWrite();

  // Line buffer makes plotting faster
  uint16_t  lineBuf[dw];

  if (bpp8) { // 8 bits per pixel
//    _swapBytes = false;

    data += dx + dy * w;

    uint8_t  blue[] = {0, 11, 21, 31}; // blue 2 to 5-bit colour lookup table

    uint16_t _lastColor = -1; // Set to illegal value

    // Used to store last shifted colour
    uint8_t msbColor = 0;
    uint8_t lsbColor = 0;

    while (dh--) {
      int32_t len = dw;
      uint8_t* ptr = data;
      uint8_t* linePtr = (uint8_t*)lineBuf;

      int32_t px = x, sx = x;
      bool move = true;
      uint16_t np = 0;

      while (len--) {
        if (transp != *ptr) {
          if (move) { move = false; sx = px; }
          uint8_t color = *ptr;

          // Shifts are slow so check if colour has changed first
          if (color != _lastColor) {
            //          =====Green=====     ===============Red==============
            msbColor = (color & 0x1C)>>2 | (color & 0xC0)>>3 | (color & 0xE0);
            //          =====Green=====    =======Blue======
            lsbColor = (color & 0x1C)<<3 | blue[color & 0x03];
            _lastColor = color;
          }
          *linePtr++ = msbColor;
          *linePtr++ = lsbColor;
          np++;
        }
        else {
          move = true;
          if (np) {
            writeAddrWindow(sx, y, np, 1);
//            pushPixels(lineBuf, np);
            writeMDTBuffer((const uint8_t*)lineBuf, np);
            linePtr = (uint8_t*)lineBuf;
            np = 0;
          }
        }
        px++;
        ptr++;
      }

      if (np) {
        writeAddrWindow(sx, y, np, 1);
//        pushPixels(lineBuf, np);
        writeMDTBuffer((const uint8_t*)lineBuf, np);
      }
      y++;
      data += w;
    }
  }
  else if (cmap.cMap != nullptr) { // 4bpp with color map
    uint8_t transpidx = cmap.getMapColor(transp);

//    _swapBytes = true;

    w = (w+1) & 0xFFFE; // here we try to recreate iwidth from dwidth.
    bool splitFirst = ((dx & 0x01) != 0);
    if (splitFirst) {
      data += ((dx - 1 + dy * w) >> 1);
    }
    else {
      data += ((dx + dy * w) >> 1);
    }

    while (dh--) {
      uint32_t len = dw;
      uint8_t * ptr = data;

      int32_t px = x, sx = x;
      bool move = true;
      uint16_t np = 0;

      uint8_t index;  // index into cmap.

      if (splitFirst) {
        index = (*ptr & 0x0F);  // odd = bits 3 .. 0
        if (index != transpidx) {
          move = false; sx = px;
          lineBuf[np] = color24to16(cmap.cMap[index]);
          np++;
        }
        px++; ptr++;
        len--;
      }

      while (len--) {
        uint8_t color = *ptr;

        // find the actual color you care about.  There will be two pixels here!
        // but we may only want one at the end of the row
        uint16_t index = ((color & 0xF0) >> 4) & 0x0F;  // high bits are the even numbers
        if (index != transpidx) {
          if (move) {
            move = false; sx = px;
          }
          lineBuf[np] = color24to16(cmap.cMap[index]);
          np++; // added a pixel
        }
        else {
          move = true;
          if (np) {
            writeAddrWindow(sx, y, np, 1);
//            pushPixels(lineBuf, np);
            writeMDTBuffer((const uint8_t*)lineBuf, np);
            np = 0;
          }
        }
        px++;

        if (len--) {
          index = color & 0x0F; // the odd number is 3 .. 0
          if (index != transpidx) {
            if (move) {
              move = false; sx = px;
            }
            lineBuf[np] = color24to16(cmap.cMap[index]);
            np++;
          }
          else {
            move = true;
            if (np) {
              writeAddrWindow(sx, y, np, 1);
//              pushPixels(lineBuf, np);
              writeMDTBuffer((const uint8_t*)lineBuf, np);
              np = 0;
            }
          }
          px++;
        }
        else {
          break;  // we are done with this row.
        }
        ptr++;  // we only increment ptr once in the loop (deliberate)
      }

      if (np) {
        writeAddrWindow(sx, y, np, 1);
//        pushPixels(lineBuf, np);
        writeMDTBuffer((const uint8_t*)lineBuf, np);
        np = 0;
      }
      data += (w>>1);
      y++;
    }
  }
  else { // 1 bit per pixel
//    _swapBytes = false;

    uint32_t ww =  (w+7)>>3; // Width of source image line in bytes
    uint16_t np = 0;

    for (int32_t yp = dy;  yp < dy + dh; yp++) {
      int32_t px = x, sx = x;
      bool move = true;
      for (int32_t xp = dx; xp < dx + dw; xp++) {
        if (data[(xp>>3)] & (0x80 >> (xp & 0x7))) {
          if (move) {
            move = false;
            sx = px;
          }
          np++;
        }
        else {
          move = true;
          if (np) {
            writeAddrWindow(sx, y, np, 1);
//            pushBlock(bitmap_fg, np);
            sendMDTColor(mdt_color(_bitmap_fg), np);
            np = 0;
          }
        }
        px++;
      }
      if (np) {
        writeAddrWindow(sx, y, np, 1);
//        pushBlock(bitmap_fg, np);
        sendMDTColor(mdt_color(_bitmap_fg), np);
        np = 0;
      }
      y++;
      data += ww;
    }
  }
  endWrite();
}


/***************************************************************************************
** Function name:           pushMaskedImage
** Description:             Render a 16-bit colour image to TFT with a 1bpp mask
***************************************************************************************/
// Can be used with a 16bpp sprite and a 1bpp sprite for the mask
void TSD_GFX::pushMaskedImage(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *img, uint8_t *mask)
{
//  if (_vpOoB ) return;
  if (w < 1 || h < 1) return;

  // To simplify mask handling the window clipping is done by the pushImage function
  // Each mask image line assumed to be padded to an integer number of bytes & padding bits are 0

//  begin_tft_write();
//  inTransaction = true;
  startWrite();

  uint8_t  *mptr = mask;
  uint8_t  *eptr = mask + ((w + 7) >> 3);
  uint16_t *iptr = img;
  uint32_t setCount = 0;

  // For each line in the image
  while (h--) {
    uint32_t xp = 0;
    uint32_t clearCount = 0;
    uint8_t  mbyte= *mptr++;
    uint32_t bits  = 8;
    // Scan through each byte of the bitmap and determine run lengths
    do {
      setCount = 0;

      //Get run length for clear bits to determine x offset
      while ((mbyte & 0x80) == 0x00) {
        // Check if remaining bits in byte are clear (reduce shifts)
        if (mbyte == 0) {
          clearCount += bits;      // bits not always 8 here
          if (mptr >= eptr) break; // end of line
          mbyte = *mptr++;
          bits  = 8;
          continue;
        }
        mbyte = mbyte << 1; // 0's shifted in
        clearCount ++;
        if (--bits) continue;;
        if (mptr >= eptr) break;
        mbyte = *mptr++;
        bits  = 8;
      }

      //Get run length for set bits to determine render width
      while ((mbyte & 0x80) == 0x80) {
        // Check if all bits are set (reduces shifts)
        if (mbyte == 0xFF) {
          setCount += bits;
          if (mptr >= eptr) break;
          mbyte = *mptr++;
          //bits  = 8; // NR, bits always 8 here unless 1's shifted in
          continue;
        }
        mbyte = mbyte << 1; //or mbyte += mbyte + 1 to shift in 1's
        setCount ++;
        if (--bits) continue;
        if (mptr >= eptr) break;
        mbyte = *mptr++;
        bits  = 8;
      }

      // A mask boundary or mask end has been found, so render the pixel line
      if (setCount) {
        xp += clearCount;
        clearCount = 0;
        pushImage(clip, x + xp, y, setCount, 1, iptr + xp);      // pushImage handles clipping
        if (mptr >= eptr) break;
        xp += setCount;
      }
    } while (setCount || mptr < eptr);

    y++;
    iptr += w;
    eptr += ((w + 7) >> 3);
  }

//  inTransaction = lockTransaction;
//  end_tft_write();
  endWrite();
}


/***************************************************************************************
** Function name:           drawCircle
** Description:             Draw a circle outline
***************************************************************************************/
// Optimised midpoint circle algorithm
void TSD_GFX::drawCircle(clip_t& clip, int32_t x0, int32_t y0, int32_t r, rgb_t color)
{
  if ( r <= 0 ) return;

    startWrite();

    int32_t f     = 1 - r;
    int32_t ddF_y = -2 * r;
    int32_t ddF_x = 1;
    int32_t xs    = -1;
    int32_t xe    = 0;
    int32_t len   = 0;

    bool first = true;
    do {
      while (f < 0) {
        ++xe;
        f += (ddF_x += 2);
      }
      f += (ddF_y += 2);

      if (xe-xs>1) {
        if (first) {
          len = 2*(xe - xs)-1;
          drawFastHLine(clip, x0 - xe, y0 + r, len, color);
          drawFastHLine(clip, x0 - xe, y0 - r, len, color);
          drawFastVLine(clip, x0 + r, y0 - xe, len, color);
          drawFastVLine(clip, x0 - r, y0 - xe, len, color);
          first = false;
        }
        else {
          len = xe - xs++;
          drawFastHLine(clip, x0 - xe, y0 + r, len, color);
          drawFastHLine(clip, x0 - xe, y0 - r, len, color);
          drawFastHLine(clip, x0 + xs, y0 - r, len, color);
          drawFastHLine(clip, x0 + xs, y0 + r, len, color);

          drawFastVLine(clip, x0 + r, y0 + xs, len, color);
          drawFastVLine(clip, x0 + r, y0 - xe, len, color);
          drawFastVLine(clip, x0 - r, y0 - xe, len, color);
          drawFastVLine(clip, x0 - r, y0 + xs, len, color);
        }
      }
      else {
        ++xs;
        drawPixel(clip, x0 - xe, y0 + r, color);
        drawPixel(clip, x0 - xe, y0 - r, color);
        drawPixel(clip, x0 + xs, y0 - r, color);
        drawPixel(clip, x0 + xs, y0 + r, color);

        drawPixel(clip, x0 + r, y0 + xs, color);
        drawPixel(clip, x0 + r, y0 - xe, color);
        drawPixel(clip, x0 - r, y0 - xe, color);
        drawPixel(clip, x0 - r, y0 + xs, color);
      }
      xs = xe;
    } while (xe < --r);

    endWrite();
}


/***************************************************************************************
** Function name:           drawCircleHelper
** Description:             Support function for drawRoundRect()
***************************************************************************************/
void TSD_GFX::drawCircleHelper(clip_t& clip, int32_t x0, int32_t y0, int32_t rr, uint8_t corners, rgb_t color)
{
  if (rr <= 0) return;

  int32_t f     = 1 - rr;
  int32_t ddF_x = 1;
  int32_t ddF_y = -2 * rr;
  int32_t xe    = 0;
  int32_t xs    = 0;
  int32_t len   = 0;

  while (xe < rr--)
  {
    while (f < 0) {
      ++xe;
      f += (ddF_x += 2);
    }
    f += (ddF_y += 2);

    if (xe-xs==1) {
      if (corners & 0x1) { // left top
        drawPixel(clip, x0 - xe, y0 - rr, color);
        drawPixel(clip, x0 - rr, y0 - xe, color);
      }
      if (corners & 0x2) { // right top
        drawPixel(clip, x0 + rr    , y0 - xe, color);
        drawPixel(clip, x0 + xs + 1, y0 - rr, color);
      }
      if (corners & 0x4) { // right bottom
        drawPixel(clip, x0 + xs + 1, y0 + rr    , color);
        drawPixel(clip, x0 + rr, y0 + xs + 1, color);
      }
      if (corners & 0x8) { // left bottom
        drawPixel(clip, x0 - rr, y0 + xs + 1, color);
        drawPixel(clip, x0 - xe, y0 + rr    , color);
      }
    }
    else {
      len = xe - xs++;
      if (corners & 0x1) { // left top
        drawFastHLine(clip, x0 - xe, y0 - rr, len, color);
        drawFastVLine(clip, x0 - rr, y0 - xe, len, color);
      }
      if (corners & 0x2) { // right top
        drawFastVLine(clip, x0 + rr, y0 - xe, len, color);
        drawFastHLine(clip, x0 + xs, y0 - rr, len, color);
      }
      if (corners & 0x4) { // right bottom
        drawFastHLine(clip, x0 + xs, y0 + rr, len, color);
        drawFastVLine(clip, x0 + rr, y0 + xs, len, color);
      }
      if (corners & 0x8) { // left bottom
        drawFastVLine(clip, x0 - rr, y0 + xs, len, color);
        drawFastHLine(clip, x0 - xe, y0 + rr, len, color);
      }
    }
    xs = xe;
  }
}

void TSD_GFX::drawCircleFragment(clip_t& clip, int32_t x0, int32_t y0, int32_t rr, uint8_t corners, rgb_t color)
{
  startWrite();
  drawCircleHelper(clip, x0, y0, rr, corners, color);
  endWrite();
}

/***************************************************************************************
** Function name:           fillCircle
** Description:             draw a filled circle
***************************************************************************************/
// Optimised midpoint circle algorithm, changed to horizontal lines (faster in sprites)
// Improved algorithm avoids repetition of lines
void TSD_GFX::fillCircle(clip_t& clip, int32_t x0, int32_t y0, int32_t r, rgb_t color)
{
  startWrite();

  int32_t  x  = 0;
  int32_t  dx = 1;
  int32_t  dy = r+r;
  int32_t  p  = -(r>>1);

  drawFastHLine(clip, x0 - r, y0, dy+1, color);

  while(x<r){

    if(p>=0) {
      drawFastHLine(clip, x0 - x, y0 + r, dx, color);
      drawFastHLine(clip, x0 - x, y0 - r, dx, color);
      dy-=2;
      p-=dy;
      r--;
    }

    dx+=2;
    p+=dx;
    x++;

    drawFastHLine(clip, x0 - r, y0 + x, dy+1, color);
    drawFastHLine(clip, x0 - r, y0 - x, dy+1, color);

  }
  endWrite();
}

/***************************************************************************************
** Function name:           fillCircleHelper
** Description:             Support function for fillRoundRect()
***************************************************************************************/
// Support drawing roundrects, changed to horizontal lines (faster in sprites)
void TSD_GFX::fillCircleHelper(clip_t& clip, int32_t x0, int32_t y0, int32_t r, uint8_t corners, int32_t delta, rgb_t color)
{
  int32_t f     = 1 - r;
  int32_t ddF_x = 1;
  int32_t ddF_y = -r - r;
  int32_t y     = 0;

  delta++;

  while (y < r) {
    if (f >= 0) {
      if (corners & 0x1) drawFastHLine(clip, x0 - y, y0 + r, y + y + delta, color);
      if (corners & 0x2) drawFastHLine(clip, x0 - y, y0 - r, y + y + delta, color);
      r--;
      ddF_y += 2;
      f     += ddF_y;
    }

    y++;
    ddF_x += 2;
    f     += ddF_x;

    if (corners & 0x1) drawFastHLine(clip, x0 - r, y0 + y, r + r + delta, color);
    if (corners & 0x2) drawFastHLine(clip, x0 - r, y0 - y, r + r + delta, color);
  }
}


/***************************************************************************************
** Function name:           drawEllipse
** Description:             Draw a ellipse outline
***************************************************************************************/
void TSD_GFX::drawEllipse(clip_t& clip, int32_t x0, int32_t y0, int32_t rx, int32_t ry, rgb_t color)
{
  startWrite();

  int a2 = rx * rx;
  int b2 = ry * ry;
  int a2b2 = a2 * b2;
  int dx = 0;

  drawPixel(clip, x0 - rx, y0, color);
  drawPixel(clip, x0 + rx, y0, color);

  int px = rx;
  for (int x = rx, y = 1; y <= ry; y++) {
    int x1 = x - (dx - 1);  // try slopes of dx - 1 or more
    for ( ; x1 > 0; x1--)
      if (x1*x1*b2 + a2*y*y <= a2b2)
        break;
    dx = x - x1;
    x = x1;
    if (y < ry) {
      drawFastHLine(clip, x0 - px, y0 - y, px - x, color);
      drawFastHLine(clip, x0 + x, y0 - y, px - x, color);
      drawFastHLine(clip, x0 - px, y0 + y, px - x, color);
      drawFastHLine(clip, x0 + x, y0 + y, px - x, color);
    }
    else {
      drawFastHLine(clip, x0 - px, y0 - y, 2*(px-x), color);
      drawFastHLine(clip, x0 - px, y0 + y, 2*(px-x), color);
    }
    px = x;
  }
  endWrite();
}


/***************************************************************************************
** Function name:           fillEllipse
** Description:             draw a filled ellipse
***************************************************************************************/
void TSD_GFX::fillEllipse(clip_t& clip, int32_t x0, int32_t y0, int32_t rx, int32_t ry, rgb_t color)
{
  startWrite();

  int a2 = rx * rx;
  int b2 = ry * ry;
  int a2b2 = a2 * b2;
  int x = rx;
  int dx = 0;

  drawFastHLine(clip, x0 - x, y0, 2*x, color);

  for (int y = 1; y <= ry; y++) {
    int x1 = x - (dx - 1);  // try slopes of dx - 1 or more
    for ( ; x1 > 0; x1--)
      if (x1*x1*b2 + a2*y*y <= a2b2)
        break;
    dx = x - x1;
    x = x1;
    drawFastHLine(clip, x0 - x, y0 - y, 2*x, color);
    drawFastHLine(clip, x0 - x, y0 + y, 2*x, color);
  }
  endWrite();
}


/***************************************************************************************
** Function name:           drawRect
** Description:             Draw a rectangle outline
***************************************************************************************/
// Draw a rectangle
void TSD_GFX::drawRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color)
{
  startWrite();

  drawFastHLine(clip, x, y, w, color);
  drawFastHLine(clip, x, y + h - 1, w, color);
  // Avoid drawing corner pixels twice
  drawFastVLine(clip, x, y+1, h-2, color);
  drawFastVLine(clip, x + w - 1, y+1, h-2, color);

  endWrite();
}


/***************************************************************************************
** Function name:           drawRoundRect
** Description:             Draw a rounded corner rectangle outline
***************************************************************************************/
// Draw a rounded rectangle
void TSD_GFX::drawRoundRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, rgb_t color)
{
  startWrite();

  // smarter version
  drawFastHLine(clip, x + r  , y    , w - r - r, color); // Top
  drawFastHLine(clip, x + r  , y + h - 1, w - r - r, color); // Bottom
  drawFastVLine(clip, x    , y + r  , h - r - r, color); // Left
  drawFastVLine(clip, x + w - 1, y + r  , h - r - r, color); // Right
  // draw four corners
  drawCircleHelper(clip, x + r    , y + r    , r, 1, color);
  drawCircleHelper(clip, x + w - r - 1, y + r    , r, 2, color);
  drawCircleHelper(clip, x + w - r - 1, y + h - r - 1, r, 4, color);
  drawCircleHelper(clip, x + r    , y + h - r - 1, r, 8, color);

  endWrite();
}


/***************************************************************************************
** Function name:           fillRoundRect
** Description:             Draw a rounded corner filled rectangle
***************************************************************************************/
// Fill a rounded rectangle, changed to horizontal lines (faster in sprites)
void TSD_GFX::fillRoundRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, rgb_t color)
{
  startWrite();

  // smarter version
  fillRectHelper(clip, x, y + r, w, h - r - r, color);

  // draw four corners
  fillCircleHelper(clip, x + r, y + h - r - 1, r, 1, w - r - r - 1, color);
  fillCircleHelper(clip, x + r    , y + r, r, 2, w - r - r - 1, color);

  endWrite();
}


void TSD_GFX::fillCircleFragment(clip_t& clip, int32_t x0, int32_t y0, int32_t r, uint8_t corners, int32_t delta, rgb_t color)
{
  startWrite();
  fillCircleHelper(clip, x0, y0, r, corners, delta, color);
  endWrite();
}


/***************************************************************************************
** Function name:           drawTriangle
** Description:             Draw a triangle outline using 3 arbitrary points
***************************************************************************************/
// Draw a triangle
void TSD_GFX::drawTriangle(clip_t& clip, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgb_t color)
{
  drawLine(clip, x0, y0, x1, y1, color);
  drawLine(clip, x1, y1, x2, y2, color);
  drawLine(clip, x2, y2, x0, y0, color);
}


/***************************************************************************************
** Function name:           fillTriangle
** Description:             Draw a filled triangle using 3 arbitrary points
***************************************************************************************/
// Fill a triangle - original Adafruit function works well and code footprint is small
void TSD_GFX::fillTriangle(clip_t& clip, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, rgb_t color)
{
  int32_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    SWAP_INT(y0, y1);
    SWAP_INT(x0, x1);
  }
  if (y1 > y2) {
    SWAP_INT(y2, y1);
    SWAP_INT(x2, x1);
  }
  if (y0 > y1) {
    SWAP_INT(y0, y1);
    SWAP_INT(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)      a = x1;
    else if (x1 > b) b = x1;
    if (x2 < a)      a = x2;
    else if (x2 > b) b = x2;

    startWrite();
    drawFastHLine(clip, a, y0, b - a + 1, color);
    endWrite();

    return;
  }

  startWrite();

  int32_t
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1,
  sa   = 0,
  sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2) last = y1;  // Include y1 scanline
  else         last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if (a > b) SWAP_INT(a, b);
    drawFastHLine(clip, a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for (; y <= y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if (a > b) SWAP_INT(a, b);
    drawFastHLine(clip, a, y, b - a + 1, color);
  }

  endWrite();
}




// BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------

/***************************************************************************************
** Function name:           drawBitmap
** Description:             Draw an image stored in an array on the TFT
***************************************************************************************/
void TSD_GFX::drawBitmap(clip_t& clip, int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h, rgb_t color)
{
  startWrite();

  int32_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  for (int32_t j = 0; j < h; j++, y++) {
    for (int32_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = bitmap[j * byteWidth + i / 8];
      if (b & 0x80)
        drawPixel(clip, x + i, y, color);
    }
  }

  endWrite();
}


/***************************************************************************************
** Function name:           drawBitmap
** Description:             Draw an image stored in an array on the TFT
***************************************************************************************/
void TSD_GFX::drawBitmap(clip_t& clip, int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h, rgb_t color, rgb_t bg)
{
  startWrite();

  int32_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  for (int32_t j = 0; j < h; j++, y++) {
    for (int32_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = bitmap[j * byteWidth + i / 8];
      drawPixel(clip, x + i, y, (b & 0x80) ? color : bg);
    }
  }

  endWrite();
}

/***************************************************************************************
** Function name:           drawXBitmap
** Description:             Draw an image stored in an XBM array onto the TFT
***************************************************************************************/
void TSD_GFX::drawGrayscaleBitmap(clip_t& clip, int32_t x, int32_t y, const uint8_t* bitmap, int32_t w, int32_t h)
{
  startWrite();

  for (int32_t j = 0; j < h; j++, y++) {
    for (int32_t i = 0; i < w; i++) {
      drawPixel(clip, x + i, y, bitmap[j * w + i] ? WHITE : BLACK);
    }
  }
  endWrite();
}


/***************************************************************************************
** Function name:           drawXBitmap
** Description:             Draw an XBM image with foreground and background colors
***************************************************************************************/
void TSD_GFX::drawGrayscaleBitmap(clip_t& clip, int32_t x, int32_t y, const uint8_t* bitmap, const uint8_t* mask, int32_t w, int32_t h)
{
  startWrite();

  int32_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t b = 0;
  for (int32_t j = 0; j < h; j++, y++) {
    for (int32_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = mask[j * bw + i / 8];
      if (b & 0x80) {
        drawPixel(clip, x + i, y, bitmap[j * w + i] ? WHITE : BLACK);
      }
    }
  }
  endWrite();
}


/***************************************************************************************
** Function name:           setBitmapColor
** Description:             Set the foreground foreground and background colour
***************************************************************************************/
void TSD_GFX::drawRGBBitmap(clip_t& clip, int32_t x, int32_t y, const uint16_t* bitmap, int32_t w, int32_t h)
{
  int dx, dy, dw, dh;

  if (x >= clip.x1) {dx = 0; dw = w; } else { dx = clip.x1 - x; dw = w - dx; x = clip.x1; }
  if (y >= clip.y1) {dy = 0; dh = h; } else { dy = clip.y1 - y; dh = h - dy; y = clip.y1; }

  if (x + dw > clip.x2) { dw = clip.x2 - x; }
  if (y + dh > clip.y2) { dh = clip.y2 - y; }

  if (dw < 1 || dh < 1) return;

  startWrite();

  writeAddrWindow(x, y, dw, dh);

  if (dx != 0) {
    const uint16_t* p = bitmap + dw * dy + dx;
    if (MDT_SIZE == 2) {
      for (int j = 0; j < dh; ++j) {
        writeMDTBuffer((const uint8_t*)p, dw);
        p += w;
      }
    }
    else {   // translate 565 to 666
      const uint16_t* p = bitmap + dw * dy + dx;
      const uint8_t* buff = (const uint8_t*)malloc(dw * MDT_SIZE + 3);
      uint8_t* q = (uint8_t*)buff;
      for (int j = 0; j < dh; ++j) {
        for (int i = 0; i < dw; ++i) {
          *(rgb_t*)q = rgb(*p++);
          q += 3;
        }
        writeMDTBuffer((const uint8_t*)buff, dw);
        p += w;
      }
      free((void*)buff);
    }
  }
  else {
    writeMDTBuffer((const uint8_t*)(bitmap + w * dy), w * dh);
  }

  endWrite();
}

void TSD_GFX::drawRGBBitmap(clip_t& clip, int32_t x, int32_t y, const uint16_t* bitmap, const uint8_t* mask, int32_t w, int32_t h)
{
  startWrite();

  int32_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t b = 0;
  for (int32_t j = 0; j < h; j++, y++) {
    for (int32_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = mask[j * bw + i / 8];
      if (b & 0x80) {
        drawPixel(clip, x + i, y, rgb(bitmap[j * w + i]));
      }
    }
  }
  endWrite();
}

void TSD_GFX::drawRGBBitmap(clip_t& clip, int32_t x, int32_t y, const uint32_t* bitmap, int32_t w, int32_t h)
{
  startWrite();

  for (int32_t j = 0; j < h; j++, y++) {
    for (int32_t i = 0; i < w; i++) {
      drawPixel(clip, x + i, y, bitmap[j * w + i] | 0xFF000000);
    }
  }
  endWrite();
}

void TSD_GFX::drawRGBBitmap(clip_t& clip, int32_t x, int32_t y, const uint32_t* bitmap, const uint8_t* mask, int32_t w, int32_t h)
{
  startWrite();

  int32_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t b = 0;
  for (int32_t j = 0; j < h; j++, y++) {
    for (int32_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = mask[j * bw + i / 8];
      if (b & 0x80) {
        drawPixel(clip, x + i, y, bitmap[j * w + i] | 0xFF000000);
      }
    }
  }
  endWrite();
}


/***************************************************************************************
** Function name:           drawLine
** Description:             draw a line between 2 arbitrary points
***************************************************************************************/
// Bresenham's algorithm - thx wikipedia - speed enhanced by Bodmer to use
// an efficient FastH/V Line draw routine for line segments of 2 pixels or more
void TSD_GFX::drawLine(clip_t& clip, int32_t x0, int32_t y0, int32_t x1, int32_t y1, rgb_t color)
{
  startWrite();

  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    SWAP_INT(x0, y0);
    SWAP_INT(x1, y1);
  }

  if (x0 > x1) {
    SWAP_INT(x0, x1);
    SWAP_INT(y0, y1);
  }

  int32_t dx = x1 - x0;
  int32_t dy = abs(y1 - y0);

  int32_t err = dx >> 1;
  int32_t xs = x0;
  int32_t dlen = 0;
  int32_t ystep = y0 < y1 ? 1 : -1;

  // Split into steep and not steep for FastH/V separation
  if (steep) {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        if (dlen == 1)
          drawPixel(clip, y0, xs, color);
        else
          drawFastVLine(clip, y0, xs, dlen, color);
        dlen = 0;
        y0 += ystep; xs = x0 + 1;
        err += dx;
      }
    }
    if (dlen) drawFastVLine(clip, y0, xs, dlen, color);
  }
  else
  {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        if (dlen == 1)
          drawPixel(clip, xs, y0, color);
        else
          drawFastHLine(clip, xs, y0, dlen, color);
        dlen = 0;
        y0 += ystep; xs = x0 + 1;
        err += dx;
      }
    }
    if (dlen) drawFastHLine(clip, xs, y0, dlen, color);
  }
  endWrite();
}


/***************************************************************************************
** Description:  Constants for anti-aliased line drawing on TFT and in Sprites
***************************************************************************************/
constexpr float PixelAlphaGain   = 255.0;
constexpr float LoAlphaTheshold  = 1.0/32.0;
constexpr float HiAlphaTheshold  = 1.0 - LoAlphaTheshold;
constexpr float deg2rad      = 3.14159265359/180.0;


/***************************************************************************************
** Function name:           drawPixel (alpha blended)
** Description:             Draw a pixel blended with the screen or bg pixel colour
***************************************************************************************/
uint16_t TSD_GFX::drawPixel(clip_t& clip, int32_t x, int32_t y, rgb_t color, uint8_t alpha, rgb_t bg_color)
{
  if (bg_color == 0x00FFFFFF) bg_color = readPixel(clip, x, y);
  color = alphaBlend(alpha, color, bg_color);
  drawPixel(clip, x, y, color);
  return color;
}


/***************************************************************************************
** Function name:           drawSmoothArc
** Description:             Draw a smooth arc clockwise from 6 o'clock
***************************************************************************************/
void TSD_GFX::drawSmoothArc(clip_t& clip, int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool roundEnds)
// Centre at x,y
// r = arc outer radius, ir = arc inner radius. Inclusive so arc thickness = r - ir + 1
// Angles in range 0-360
// Arc foreground colour anti-aliased with background colour at edges
// anti-aliased roundEnd is optional, default is anti-aliased straight end
// Note: rounded ends extend the arc angle so can overlap, user sketch to manage this.
{
//???  inTransaction = true;
  startWrite();

  if (endAngle != startAngle && (startAngle != 0 || endAngle != 360))
  {
    float sx = -sinf(startAngle * deg2rad);
    float sy = +cosf(startAngle * deg2rad);
    float ex = -sinf(  endAngle * deg2rad);
    float ey = +cosf(  endAngle * deg2rad);

    if (roundEnds)
    { // Round ends
      sx = sx * (r + ir)/2.0 + x;
      sy = sy * (r + ir)/2.0 + y;
      drawSpot(clip, sx, sy, (r - ir)/2.0, fg_color, bg_color);

      ex = ex * (r + ir)/2.0 + x;
      ey = ey * (r + ir)/2.0 + y;
      drawSpot(clip, ex, ey, (r - ir)/2.0, fg_color, bg_color);
    }
    else
    { // Square ends
      float asx = sx * ir + x;
      float asy = sy * ir + y;
      float aex = sx *  r + x;
      float aey = sy *  r + y;
      drawWedgeLine(clip, asx, asy, aex, aey, 0.3, 0.3, fg_color, bg_color);

      asx = ex * ir + x;
      asy = ey * ir + y;
      aex = ex *  r + x;
      aey = ey *  r + y;
      drawWedgeLine(clip, asx, asy, aex, aey, 0.3, 0.3, fg_color, bg_color);
    }

    // Draw arc
    drawArc(clip, x, y, r, ir, startAngle, endAngle, fg_color, bg_color);

  }
  else // Draw full 360
  {
    drawArc(clip, x, y, r, ir, 0, 360, fg_color, bg_color);
  }

//???  inTransaction = lockTransaction;
//???  end_tft_write();
  endWrite();
}


/***************************************************************************************
** Function name:           sqrt_fraction (private function)
** Description:             Smooth graphics support function for alpha derivation
***************************************************************************************/
// Compute the fixed point square root of an integer and
// return the 8 MS bits of fractional part.
// Quicker than sqrt() for processors that do not have an FPU (e.g. RP2040)
inline uint8_t TSD_GFX::sqrt_fraction(uint32_t num) {
  if (num > (0x40000000)) return 0;
  uint32_t bsh = 0x00004000;
  uint32_t fpr = 0;
  uint32_t osh = 0;

  // Auto adjust from U8:8 up to U15:16
  while (num>bsh) {bsh <<= 2; osh++;}

  do {
    uint32_t bod = bsh + fpr;
    if(num >= bod)
    {
      num -= bod;
      fpr = bsh + bod;
    }
    num <<= 1;
  } while(bsh >>= 1);

  return fpr>>osh;
}

/***************************************************************************************
** Function name:           drawArc
** Description:             Draw an arc clockwise from 6 o'clock position
***************************************************************************************/
// Centre at x,y
// r = arc outer radius, ir = arc inner radius. Inclusive, so arc thickness = r-ir+1
// Angles MUST be in range 0-360
// Arc foreground fg_color anti-aliased with background colour along sides
// smooth is optional, default is true, smooth=false means no antialiasing
// Note: Arc ends are not anti-aliased (use drawSmoothArc instead for that)
void TSD_GFX::drawArc(clip_t& clip, int32_t x, int32_t y, int32_t r, int32_t ir,
                       uint32_t startAngle, uint32_t endAngle,
                       rgb_t fg_color, rgb_t bg_color,
                       bool smooth)
{
  if (endAngle   > 360)   endAngle = 360;
  if (startAngle > 360) startAngle = 360;
  if (startAngle == endAngle) return;
  if (r < ir) transpose(r, ir);  // Required that r > ir
  if (r <= 0 || ir < 0) return;  // Invalid r, ir can be zero (circle sector)

  if (endAngle < startAngle) {
    // Arc sweeps through 6 o'clock so draw in two parts
    if (startAngle < 360) drawArc(clip, x, y, r, ir, startAngle, 360, fg_color, bg_color, smooth);
    if (endAngle == 0) return;
    startAngle = 0;
  }
// inTransaction = true;
  startWrite();

  int32_t xs = 0;        // x start position for quadrant scan
  uint8_t alpha = 0;     // alpha value for blending pixels

  uint32_t r2 = r * r;   // Outer arc radius^2
  if (smooth) r++;       // Outer AA zone radius
  uint32_t r1 = r * r;   // Outer AA radius^2
  int16_t w  = r - ir;   // Width of arc (r - ir + 1)
  uint32_t r3 = ir * ir; // Inner arc radius^2
  if (smooth) ir--;      // Inner AA zone radius
  uint32_t r4 = ir * ir; // Inner AA radius^2

  //     1 | 2
  //    ---¦---    Arc quadrant index
  //     0 | 3
  // Fixed point U16.16 slope table for arc start/end in each quadrant
  uint32_t startSlope[4] = {0, 0, 0xFFFFFFFF, 0};
  uint32_t   endSlope[4] = {0, 0xFFFFFFFF, 0, 0};

  // Ensure maximum U16.16 slope of arc ends is ~ 0x8000 0000
  constexpr float minDivisor = 1.0f/0x8000;

  // Fill in start slope table and empty quadrants
  float fabscos = fabsf(cosf(startAngle * deg2rad));
  float fabssin = fabsf(sinf(startAngle * deg2rad));

  // U16.16 slope of arc start
  uint32_t slope = (fabscos/(fabssin + minDivisor)) * (float)(1UL<<16);

  // Update slope table, add slope for arc start
  if (startAngle <= 90) {
    startSlope[0] =  slope;
  }
  else if (startAngle <= 180) {
    startSlope[1] =  slope;
  }
  else if (startAngle <= 270) {
    startSlope[1] = 0xFFFFFFFF;
    startSlope[2] = slope;
  }
  else {
    startSlope[1] = 0xFFFFFFFF;
    startSlope[2] =  0;
    startSlope[3] = slope;
  }

  // Fill in end slope table and empty quadrants
  fabscos  = fabsf(cosf(endAngle * deg2rad));
  fabssin  = fabsf(sinf(endAngle * deg2rad));

  // U16.16 slope of arc end
  slope   = (uint32_t)((fabscos/(fabssin + minDivisor)) * (float)(1UL<<16));

  // Work out which quadrants will need to be drawn and add slope for arc end
  if (endAngle <= 90) {
    endSlope[0] = slope;
    endSlope[1] =  0;
    startSlope[2] =  0;
  }
  else if (endAngle <= 180) {
    endSlope[1] = slope;
    startSlope[2] =  0;
  }
  else if (endAngle <= 270) {
    endSlope[2] =  slope;
  }
  else {
    endSlope[3] =  slope;
  }

  // Scan quadrant
  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    uint32_t len[4] = { 0,  0,  0,  0}; // Pixel run length
    int32_t  xst[4] = {-1, -1, -1, -1}; // Pixel run x start
    uint32_t dy2 = (r - cy) * (r - cy);

    // Find and track arc zone start point
    while ((r - xs) * (r - xs) + dy2 >= r1) xs++;

    for (int32_t cx = xs; cx < r; cx++)
    {
      // Calculate radius^2
      uint32_t hyp = (r - cx) * (r - cx) + dy2;

      // If in outer zone calculate alpha
      if (hyp > r2) {
        alpha = ~sqrt_fraction(hyp); // Outer AA zone
      }
      // If within arc fill zone, get line start and lengths for each quadrant
      else if (hyp >= r3) {
        // Calculate U16.16 slope
        slope = ((r - cy) << 16)/(r - cx);
        if (slope <= startSlope[0] && slope >= endSlope[0]) { // slope hi -> lo
          xst[0] = cx; // Bottom left line end
          len[0]++;
        }
        if (slope >= startSlope[1] && slope <= endSlope[1]) { // slope lo -> hi
          xst[1] = cx; // Top left line end
          len[1]++;
        }
        if (slope <= startSlope[2] && slope >= endSlope[2]) { // slope hi -> lo
          xst[2] = cx; // Bottom right line start
          len[2]++;
        }
        if (slope <= endSlope[3] && slope >= startSlope[3]) { // slope lo -> hi
          xst[3] = cx; // Top right line start
          len[3]++;
        }
        continue; // Next x
      }
      else {
        if (hyp <= r4) break;  // Skip inner pixels
        alpha = sqrt_fraction(hyp); // Inner AA zone
      }

      if (alpha < 16) continue;  // Skip low alpha pixels

      // If background is read it must be done in each quadrant
      uint16_t pcol = alphaBlend(alpha, fg_color, bg_color);
      // Check if an AA pixels need to be drawn
      slope = ((r - cy)<<16)/(r - cx);
      if (slope <= startSlope[0] && slope >= endSlope[0]) // BL
        drawPixel(clip, x + cx - r, y - cy + r, pcol);
      if (slope >= startSlope[1] && slope <= endSlope[1]) // TL
        drawPixel(clip, x + cx - r, y + cy - r, pcol);
      if (slope <= startSlope[2] && slope >= endSlope[2]) // TR
        drawPixel(clip, x - cx + r, y + cy - r, pcol);
      if (slope <= endSlope[3] && slope >= startSlope[3]) // BR
        drawPixel(clip, x - cx + r, y - cy + r, pcol);
    }
    // Add line in inner zone
    if (len[0]) drawFastHLine(clip, x + xst[0] - len[0] + 1 - r, y - cy + r, len[0], fg_color); // BL
    if (len[1]) drawFastHLine(clip, x + xst[1] - len[1] + 1 - r, y + cy - r, len[1], fg_color); // TL
    if (len[2]) drawFastHLine(clip, x - xst[2] + r, y + cy - r, len[2], fg_color); // TR
    if (len[3]) drawFastHLine(clip, x - xst[3] + r, y - cy + r, len[3], fg_color); // BR
  }

  // Fill in centre lines
  if (startAngle ==   0 || endAngle == 360) drawFastVLine(clip, x, y + r - w, w, fg_color); // Bottom
  if (startAngle <=  90 && endAngle >=  90) drawFastHLine(clip, x - r + 1, y, w, fg_color); // Left
  if (startAngle <= 180 && endAngle >= 180) drawFastVLine(clip, x, y - r + 1, w, fg_color); // Top
  if (startAngle <= 270 && endAngle >= 270) drawFastHLine(clip, x + r - w, y, w, fg_color); // Right

//???  inTransaction = lockTransaction;
//???  end_tft_write();
  endWrite();
}


/***************************************************************************************
** Function name:           fillSmoothCircle
** Description:             Draw a filled anti-aliased circle
***************************************************************************************/
void TSD_GFX::fillSmoothCircle(clip_t& clip, int32_t x, int32_t y, int32_t r, rgb_t color, rgb_t bg_color)
{
  if (r <= 0) return;

//  inTransaction = true;
  startWrite();

  drawFastHLine(clip, x - r, y, 2 * r + 1, color);
  int32_t xs = 1;
  int32_t cx = 0;

  int32_t r1 = r * r;
  r++;
  int32_t r2 = r * r;

  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t dy2 = (r - cy) * (r - cy);
    for (cx = xs; cx < r; cx++)
    {
      int32_t hyp2 = (r - cx) * (r - cx) + dy2;
      if (hyp2 <= r1) break;
      if (hyp2 >= r2) continue;

      uint8_t alpha = ~sqrt_fraction(hyp2);
      if (alpha > 246) break;
      xs = cx;
      if (alpha < 9) continue;

      if (bg_color == 0x00FFFFFF) {
        drawPixel(clip, x + cx - r, y + cy - r, color, alpha, bg_color);
        drawPixel(clip, x - cx + r, y + cy - r, color, alpha, bg_color);
        drawPixel(clip, x - cx + r, y - cy + r, color, alpha, bg_color);
        drawPixel(clip, x + cx - r, y - cy + r, color, alpha, bg_color);
      }
      else {
        uint16_t pcol = drawPixel(clip, x + cx - r, y + cy - r, color, alpha, bg_color);
        drawPixel(clip, x - cx + r, y + cy - r, pcol);
        drawPixel(clip, x - cx + r, y - cy + r, pcol);
        drawPixel(clip, x + cx - r, y - cy + r, pcol);
      }
    }
    drawFastHLine(clip, x + cx - r, y + cy - r, 2 * (r - cx) + 1, color);
    drawFastHLine(clip, x + cx - r, y - cy + r, 2 * (r - cx) + 1, color);
  }
//???  inTransaction = lockTransaction;
//???  end_tft_write();
  endWrite();
}


/***************************************************************************************
** Function name:           drawSmoothRoundRect
** Description:             Draw a rounded rectangle
***************************************************************************************/
// x,y is top left corner of bounding box for a complete rounded rectangle
// r = arc outer corner radius, ir = arc inner radius. Arc thickness = r-ir+1
// w and h are width and height of the bounding rectangle
// If w and h are < radius (e.g. 0,0) a circle will be drawn with centre at x+r,y+r
// Arc foreground fg_color anti-aliased with background colour at edges
// A subset of corners can be drawn by specifying a quadrants mask. A bit set in the
// mask means draw that quadrant (all are drawn if parameter missing):
//   0x1 | 0x2
//    ---¦---    Arc quadrant mask select bits (as in drawCircleHelper fn)
//   0x8 | 0x4
void TSD_GFX::drawSmoothRoundRect(clip_t& clip, int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, rgb_t fg_color, rgb_t bg_color, uint8_t quadrants)
{
  if (r < ir) transpose(r, ir); // Required that r > ir
  if (r <= 0 || ir < 0) return; // Invalid

  w -= 2*r;
  h -= 2*r;

  if (w < 0) w = 0;
  if (h < 0) h = 0;

//???  inTransaction = true;
  startWrite();

  x += r;
  y += r;

  uint16_t t = r - ir + 1;
  int32_t xs = 0;
  int32_t cx = 0;

  int32_t r2 = r * r;   // Outer arc radius^2
  r++;
  int32_t r1 = r * r;   // Outer AA zone radius^2

  int32_t r3 = ir * ir; // Inner arc radius^2
  ir--;
  int32_t r4 = ir * ir; // Inner AA zone radius^2

  uint8_t alpha = 0;

  // Scan top left quadrant x y r ir fg_color  bg_color
  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t len = 0;  // Pixel run length
    int32_t lxst = 0; // Left side run x start
    int32_t rxst = 0; // Right side run x start
    int32_t dy2 = (r - cy) * (r - cy);

    // Find and track arc zone start point
    while ((r - xs) * (r - xs) + dy2 >= r1) xs++;

    for (cx = xs; cx < r; cx++)
    {
      // Calculate radius^2
      int32_t hyp = (r - cx) * (r - cx) + dy2;

      // If in outer zone calculate alpha
      if (hyp > r2) {
        alpha = ~sqrt_fraction(hyp); // Outer AA zone
      }
      // If within arc fill zone, get line lengths for each quadrant
      else if (hyp >= r3) {
        rxst = cx; // Right side start
        len++;     // Line segment length
        continue;  // Next x
      }
      else {
        if (hyp <= r4) break;  // Skip inner pixels
        alpha = sqrt_fraction(hyp); // Inner AA zone
      }

      if (alpha < 16) continue;  // Skip low alpha pixels

      // If background is read it must be done in each quadrant - TODO
      uint16_t pcol = alphaBlend(alpha, fg_color, bg_color);
      if (quadrants & 0x8) drawPixel(clip, x + cx - r, y - cy + r + h, pcol);     // BL
      if (quadrants & 0x1) drawPixel(clip, x + cx - r, y + cy - r, pcol);         // TL
      if (quadrants & 0x2) drawPixel(clip, x - cx + r + w, y + cy - r, pcol);     // TR
      if (quadrants & 0x4) drawPixel(clip, x - cx + r + w, y - cy + r + h, pcol); // BR
    }
    // Fill arc inner zone in each quadrant
    lxst = rxst - len + 1; // Calculate line segment start for left side
    if (quadrants & 0x8) drawFastHLine(clip, x + lxst - r, y - cy + r + h, len, fg_color);     // BL
    if (quadrants & 0x1) drawFastHLine(clip, x + lxst - r, y + cy - r, len, fg_color);         // TL
    if (quadrants & 0x2) drawFastHLine(clip, x - rxst + r + w, y + cy - r, len, fg_color);     // TR
    if (quadrants & 0x4) drawFastHLine(clip, x - rxst + r + w, y - cy + r + h, len, fg_color); // BR
  }

  // Draw sides
  if ((quadrants & 0xC) == 0xC) fillRect(clip, x, y + r - t + h, w + 1, t, fg_color); // Bottom
  if ((quadrants & 0x9) == 0x9) fillRect(clip, x - r + 1, y, t, h + 1, fg_color);     // Left
  if ((quadrants & 0x3) == 0x3) fillRect(clip, x, y - r + 1, w + 1, t, fg_color);     // Top
  if ((quadrants & 0x6) == 0x6) fillRect(clip, x + r - t + w, y, t, h + 1, fg_color); // Right

//  inTransaction = lockTransaction;
//  end_tft_write();
  endWrite();
}

/***************************************************************************************
** Function name:           fillSmoothRoundRect
** Description:             Draw a filled anti-aliased rounded corner rectangle
***************************************************************************************/
void TSD_GFX::fillSmoothRoundRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, rgb_t color, rgb_t bg_color)
{
//  inTransaction = true;
  startWrite();

  int32_t xs = 0;
  int32_t cx = 0;

  // Limit radius to half width or height
  if (r < 0)   r = 0;
  if (r > w/2) r = w/2;
  if (r > h/2) r = h/2;

  y += r;
  h -= 2*r;
  fillRect(clip, x, y, w, h, color);

  h--;
  x += r;
  w -= 2*r+1;

  int32_t r1 = r * r;
  r++;
  int32_t r2 = r * r;

  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t dy2 = (r - cy) * (r - cy);
    for (cx = xs; cx < r; cx++)
    {
      int32_t hyp2 = (r - cx) * (r - cx) + dy2;
      if (hyp2 <= r1) break;
      if (hyp2 >= r2) continue;

      uint8_t alpha = ~sqrt_fraction(hyp2);
      if (alpha > 246) break;
      xs = cx;
      if (alpha < 9) continue;

      drawPixel(clip, x + cx - r, y + cy - r, color, alpha, bg_color);
      drawPixel(clip, x - cx + r + w, y + cy - r, color, alpha, bg_color);
      drawPixel(clip, x - cx + r + w, y - cy + r + h, color, alpha, bg_color);
      drawPixel(clip, x + cx - r, y - cy + r + h, color, alpha, bg_color);
    }
    drawFastHLine(clip, x + cx - r, y + cy - r, 2 * (r - cx) + 1 + w, color);
    drawFastHLine(clip, x + cx - r, y - cy + r + h, 2 * (r - cx) + 1 + w, color);
  }
//???  inTransaction = lockTransaction;
//???  end_tft_write();
  endWrite();
}


/***************************************************************************************
** Function name:           drawWedgeLine - background colour specified or pixel read
** Description:             draw an anti-aliased line with different width radiused ends
***************************************************************************************/
void TSD_GFX::drawWedgeLine(clip_t& clip, float ax, float ay, float bx, float by, float ar, float br, rgb_t fg_color, rgb_t bg_color)
{
  if ( (ar < 0.0) || (br < 0.0) )return;
  if ( (fabsf(ax - bx) < 0.01f) && (fabsf(ay - by) < 0.01f) ) bx += 0.01f;  // Avoid divide by zero

  // Find line bounding box
  int32_t x0 = (int32_t)floorf(fminf(ax-ar, bx-br));
  int32_t x1 = (int32_t) ceilf(fmaxf(ax+ar, bx+br));
  int32_t y0 = (int32_t)floorf(fminf(ay-ar, by-br));
  int32_t y1 = (int32_t) ceilf(fmaxf(ay+ar, by+br));

//???  if (!_clip.clipClip(x0, y0, x1, y1)) return;

  // Establish x start and y start
  int32_t ys = ay;
  if ((ax-ar)>(bx-br)) ys = by;

  float rdt = ar - br; // Radius delta
  float alpha = 1.0f;
  ar += 0.5;

  uint16_t bg = bg_color;
  float xpax, ypay, bax = bx - ax, bay = by - ay;

//  begin_nin_write();
//  inTransaction = true;
  startWrite();

  int32_t xs = x0;
  // Scan bounding box from ys down, calculate pixel intensity from distance to line
  for (int32_t yp = ys; yp <= y1; yp++) {
    bool swin = true;  // Flag to start new window area
    bool endX = false; // Flag to skip pixels
    ypay = yp - ay;
    for (int32_t xp = xs; xp <= x1; xp++) {
      if (endX) if (alpha <= LoAlphaTheshold) break;  // Skip right side
      xpax = xp - ax;
      alpha = ar - wedgeLineDistance(xpax, ypay, bax, bay, rdt);
      if (alpha <= LoAlphaTheshold ) continue;
      // Track edge to minimise calculations
      if (!endX) { endX = true; xs = xp; }
      if (alpha > HiAlphaTheshold) {
        #ifdef GC9A01_DRIVER
          drawPixel(_clip, xp, yp, fg_color);
        #else
          if (swin) { writeAddrWindow(xp, yp, x1-xp+1, 1); swin = false; }
          sendMDTColor1(fg_color);
        #endif
        continue;
      }
      //Blend color with background and plot
      if (bg_color == 0x00FFFFFF) {
        bg = readPixel(clip, xp, yp); swin = true;
      }
      #ifdef GC9A01_DRIVER
        uint16_t pcol = alphaBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg);
        drawPixel(_clip, xp, yp, pcol);
        swin = swin;
      #else
        if (swin) { writeAddrWindow(xp, yp, x1-xp+1, 1); swin = false; }
        sendMDTColor1(alphaBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg));
      #endif
    }
  }

  // Reset x start to left side of box
  xs = x0;
  // Scan bounding box from ys-1 up, calculate pixel intensity from distance to line
  for (int32_t yp = ys-1; yp >= y0; yp--) {
    bool swin = true;  // Flag to start new window area
    bool endX = false; // Flag to skip pixels
    ypay = yp - ay;
    for (int32_t xp = xs; xp <= x1; xp++) {
      if (endX) if (alpha <= LoAlphaTheshold) break;  // Skip right side of drawn line
      xpax = xp - ax;
      alpha = ar - wedgeLineDistance(xpax, ypay, bax, bay, rdt);
      if (alpha <= LoAlphaTheshold ) continue;
      // Track line boundary
      if (!endX) { endX = true; xs = xp; }
      if (alpha > HiAlphaTheshold) {
        #ifdef GC9A01_DRIVER
          drawPixel(_clip, xp, yp, fg_color);
        #else
          if (swin) { writeAddrWindow(xp, yp, x1-xp+1, 1); swin = false; }
          sendMDTColor1(fg_color);
        #endif
        continue;
      }
      //Blend colour with background and plot
      if (bg_color == 0x00FFFFFF) {
        bg = readPixel(clip, xp, yp); swin = true;
      }
      #ifdef GC9A01_DRIVER
        uint16_t pcol = alphaBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg);
        drawPixel(xp, yp, pcol);
        swin = swin;
      #else
        if (swin) { writeAddrWindow(xp, yp, x1-xp+1, 1); swin = false; }
        sendMDTColor1(alphaBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg));
      #endif
    }
  }

//???  inTransaction = lockTransaction;
//???  end_nin_write();
  endWrite();
}


/***************************************************************************************
** Function name:           lineDistance - private helper function for drawWedgeLine
** Description:             returns distance of px,py to closest part of a to b wedge
***************************************************************************************/
inline float TSD_GFX::wedgeLineDistance(float xpax, float ypay, float bax, float bay, float dr)
{
  float h = fmaxf(fminf((xpax * bax + ypay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
  float dx = xpax - bax * h, dy = ypay - bay * h;
  return sqrtf(dx * dx + dy * dy) + h * dr;
}





void TSD_GFX::drawPixel(clip_t& clip, int32_t x, int32_t y, rgb_t color)
{
  if (IF_CLIP_X && IF_CLIP_Y) {
    drawClippedPixel(x, y, color);
  }
}


/***************************************************************************************
** Function name:           drawFastVLine
** Description:             draw a vertical line
***************************************************************************************/
void TSD_GFX::drawFastVLine(clip_t& clip, int32_t x, int32_t y, int32_t h, rgb_t color)
{
  CLIP_Y
  if (IF_CLIP_Y && h > 0) {
    drawClippedPixelRec(x, y, 1, h, color);
  }
}


/***************************************************************************************
** Function name:           drawFastHLine
** Description:             draw a horizontal line
***************************************************************************************/
void TSD_GFX::drawFastHLine(clip_t& clip, int32_t x, int32_t y, int32_t w, rgb_t color)
{
  CLIP_X
  if (IF_CLIP_Y && w > 0) {
    drawClippedPixelRec(x, y, w, 1, color);
  }
}


void TSD_GFX::fillRectHelper(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color)
{
  CLIP_X
  CLIP_Y
  if (w > 0 && h > 0) {
    drawClippedPixelRec(x, y, w, h, color);
  }
}


/***************************************************************************************
** Function name:           fillRect
** Description:             draw a filled rectangle
***************************************************************************************/
void TSD_GFX::fillRect(clip_t& clip, int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color)
{
  startWrite();
  fillRectHelper(clip, x, y, w, h, color);
  endWrite();
}






// color565toRGB14		- converts 16 bit 565 format color to 14 bit RGB (2 bits clear for math and sign)
// returns 00rrrrr000000000,00gggggg00000000,00bbbbb000000000
// thus not overloading sign, and allowing up to double for additions for fixed point delta

struct grec_t_ {
  int16_t r, g, b;
  void RGB14fromColor(const rgb_t color);
  rgb_t RGB14toColor();
};

typedef grec_t_  grec_t;

void grec_t_::RGB14fromColor(const rgb_t color)
{
//#ifdef COLOR_565
//  r = (color >> 2) & 0x3E00;
//  g = (color << 3) & 0x3F00;
//  b = (color << 9) & 0x3E00;
//#else
  r = (color >> 10) & 0x3F00;
  g = (color >>  2) & 0x3F00;
  b = (color <<  6) & 0x3F00;
//#endif
}

// RGB14tocolor565		- converts 14 bit RGB back to 16 bit 565 format color
rgb_t grec_t_::RGB14toColor()
{
//#ifdef COLOR_565
//  return (((r & 0x3E00) << 2) | ((g & 0x3F00) >> 3) | ((b & 0x3E00) >> 9));
//#else
  return (((r & 0x3F00) << 10) | ((g & 0x3F00) << 2) | ((b & 0x3F00) >> 6));
//#endif
}

static void nextGradHue(int8_t prc, grec_t& cc, const grec_t c1, const grec_t c2, const int32_t j, const int32_t h)
{
    int adj = prc - 50;
    if (adj > 45)
      adj = 45;
    else
    if (adj < -45)
      adj = -45;

    int16_t p = prc + (50 - prc + adj) * j * 2 / h;
    int16_t dr = (int)(c2.r - c1.r) * 50 / (p * h);
    int16_t dg = (int)(c2.g - c1.g) * 50 / (p * h);
    int16_t db = (int)(c2.b - c1.b) * 50 / (p * h);
    int q;
    q = cc.r + dr;
    if ((c2.r >= c1.r && q <= c2.r && q >= c1.r) || (c1.r >= c2.r && q <= c1.r && q >= c2.r)) cc.r = q; else
      q = 0;
    q = cc.g + dg;
    if ((c2.g >= c1.g && q <= c2.g && q >= c1.g) || (c1.g >= c2.g && q <= c1.g && q >= c2.g)) cc.g = q; else
      q = 0;
    q = cc.b + db;
    if ((c2.b >= c1.b && q <= c2.b && q >= c1.b) || (c1.b >= c2.b && q <= c1.b && q >= c2.b)) cc.b = q; else
      q = 0;
}

int ixi = 0;

void TSD_GFX::fillRectGradient(clip_t& clip, const int32_t x, const int32_t y, const int32_t w, const int32_t h, gradient_t& z)
{
  int x1 = x;
  int dx1 = 0;
  if (x1 < clip.x1) { dx1 = clip.x1 - x1; x1 = clip.x1; }
  int dx2 = x + w - clip.x2; dx2 = dx2 >= 0 ? dx2 : 0;
  int y1 = y;
  int dy1 = 0;
  if (y1 < clip.y1) { dy1 = clip.y1 - y1; y1 = clip.y1; }
  int dy2 = y + h - clip.y2; dy2 = dy2 >= 0 ? dy2 : 0;

  int w12 = w - dx1 - dx2;
  int h12 = h - dy1 - dy2;
  if (w12 <= 0 || h12 <= 0) return;

  startWrite();

  grec_t c1, c2;
  c1.RGB14fromColor(z.color1);
  c2.RGB14fromColor(z.color2);
  grec_t cc = c1;

  int8_t prc = z.percent;
  if (prc < 0) {
    prc = 0;
  }
  if (prc > 100) {
    prc = 100;
  }
  if (z.deg & 1) {
    for (int i = 0; i < w; ++i) {
//      if (i >= clip.x1 && i < clip.x2) {
        writeAddrWindow(z.deg == 3 ? w - 1 - i : i, y1, 1, h12);
        sendMDTColor(mdt_color(cc.RGB14toColor()), h12);
//      }
      nextGradHue(prc, cc, c1, c2, i, w);
    }
  }
  else {
    for (int j = 0; j < h; ++j) {
//      if (j >= clip.y1 && j < clip.y2) {
        writeAddrWindow(x1, z.deg == 2 ? h - j - 1 : j, w12, 1);
        sendMDTColor(mdt_color(cc.RGB14toColor()), w12);
//      }
      nextGradHue(prc, cc, c1, c2, j, h);
    }
  }

  endWrite();
}

void TSD_GFX::sendMDTColor(const mdt_t c, int32_t len)
{
  while (--len >= 0) {
    sendMDTColor1(c);
  }
}

void TSD_GFX::drawClippedPixel(const int32_t x, const int32_t y, const rgb_t color)
{
  writeAddrWindow(x, y, 1, 1);
  sendMDTColor1(mdt_color(color));
}

void TSD_GFX::drawClippedPixelRec(const int32_t x, const int32_t y, const int32_t w, const int32_t h, const rgb_t color)
{
  writeAddrWindow(x, y, w, h);
  sendMDTColor(mdt_color(color), w * h);
}









/***************************************************************************************
** Function name:           alphaBlend
** Description:             Blend 16bit foreground and background
*************************************************************************************x*/
uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc)
{
  // Split out and blend 5-bit red and blue channels
  uint32_t rxb = bgc & 0xF81F;
  rxb += ((fgc & 0xF81F) - rxb) * (alpha >> 2) >> 6;
  // Split out and blend 6-bit green channel
  uint32_t xgx = bgc & 0x07E0;
  xgx += ((fgc & 0x07E0) - xgx) * alpha >> 8;
  // Recombine channels
  return (rxb & 0xF81F) | (xgx & 0x07E0);
}

/***************************************************************************************
** Function name:           alphaBlend
** Description:             Blend 16bit foreground and background with dither
*************************************************************************************x*/
uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither)
{
  if (dither) {
    int16_t alphaDither = (int16_t)alpha - dither + random() % (2*dither+1); // +/-4 randomised
    alpha = (uint8_t)alphaDither;
    if (alphaDither <  0) alpha = 0;
    if (alphaDither >255) alpha = 255;
  }

  return alphaBlend(alpha, fgc, bgc);
}

/***************************************************************************************
** Function name:           alphaBlend
** Description:             Blend 24bit foreground and background with optional dither
*************************************************************************************x*/
uint32_t alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither)
{

  if (dither) {
    int16_t alphaDither = (int16_t)alpha - dither + random() % (2*dither+1); // +/-dither randomised
    alpha = (uint8_t)alphaDither;
    if (alphaDither <  0) alpha = 0;
    if (alphaDither >255) alpha = 255;
  }

  uint32_t rxx = bgc & 0xFF0000;
  rxx += ((fgc & 0xFF0000) - rxx) * alpha >> 8;
  uint32_t xgx = bgc & 0x00FF00;
  xgx += ((fgc & 0x00FF00) - xgx) * alpha >> 8;
  uint32_t xxb = bgc & 0x0000FF;
  xxb += ((fgc & 0x0000FF) - xxb) * alpha >> 8;
  return (rxx & 0xFF0000) | (xgx & 0x00FF00) | (xxb & 0x0000FF);
}
