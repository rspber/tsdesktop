/*
  GFXfont for TSD_GFX

  Copyright (c) 2023, rspber (https://github.com/rspber)

  It is a modified Adafruit's gfxfont.h to handle characters
  in utf-8 encoding.

  Font bitmap structure was extended for 2,4,8 bits per pixel.
*/

// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass FontName to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#pragma once

/// Font data stored PER GLYPH

typedef struct {
  uint8_t code;          // char code (last utf-8 char)
  uint8_t bpp;           // bits per pixel: 0:1, 1:2, 2:4, 3:8
  uint8_t ver;           // 0 - compressed Adafruit, 1 - freefont bitmap
  uint8_t width;         // Bitmap dimensions in pixel units
  uint8_t height;        // Bitmap dimensions in pixel units
  uint8_t xAdvance;      // Distance to advance cursor (x axis)
  int8_t xOffset;        // X dist from cursor pos to UL corner, positive
  int8_t yOffset;        // Y dist from cursor pos to UL corner, positive
  uint8_t len0, len1;    // bitmap length
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
  const uint8_t *glyphs;  // Glyphs with bitmaps, concatenated
  uint8_t ch1;      // 0 - for 1 byte ASCII, else utf-8 like encoding - first char code
  uint8_t ch2;      // second char code
  uint8_t ch3;      // third char code
  uint8_t first;   // last char code: from
  uint8_t last;    // last char code: to
  uint8_t yAdvance; // Newline distance (y axis)
} GFXfont;
