/*
  Copyright (c) 2022, rspber (https://github.com/rspber)
  All rights reserved

TrueType to TSD_GFX font converter. Supports UTF-8.

The program has two modes of operation,
- either we specify a range of characters
- or a file with characters
for which we want to obtain fonts.

Based on Adafruit_GFX library fontconvert.
*/

/*
Derived from Peter Jakobs' Adafruit_ftGFX fork & makefont tool,
and Paul Kourany's Adafruit_mfGFX.

NOT AN ARDUINO SKETCH.  This is a command-line tool for preprocessing
fonts to be used with the TSD_GFX.

For UNIX-like systems.  Outputs to stdout; redirect to header file, e.g.:
  ./tsdutf8fontc ~/Library/Fonts/FreeSans.ttf 18 > TSD_FreeSans_18pt.h

REQUIRES FREETYPE LIBRARY.  www.freetype.org

See notes at end for glyph nomenclature & other tidbits.
*/

// Original Adafruit's license attached below.

/*
Software License Agreement (BSD License)

Copyright (c) 2012 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ARDUINO

#include <ctype.h>
#include <ft2build.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_TRUETYPE_DRIVER_H

#define DPI 141 // Approximate res. of Adafruit 2.8" TFT

// ----------------------------------------------------------------------------
// Accumulate bits for output, with periodic hexadecimal byte write
uint8_t sum = 0, bit = 0x80;
uint8_t gphbuf[0x200];
int16_t ngph;

void enbit(uint8_t value)
{
  if (value)
    sum |= bit;          // Set bit if needed
  if (!(bit >>= 1)) {    // Advance to next bit, end of byte reached?
    gphbuf[ngph++] = sum; // Write byte value
    sum = 0;               // Clear for next byte
    bit = 0x80;            // Reset bit counter
  }
}

FT_Face face;

// ----------------------------------------------------------------------------
int16_t proc_char(const char z, const char w, const char c)
{
//uint uni32 = ((z & 0x07) << 18) | ((u & 0x3f) << 12) | ((w & 0x3f) << 6) | (i & 0x3f);
  uint uni;

  if (z) {  // utf-8 3 byte
    uni = ((z & 0x0f) << 12) | ((w & 0x3f) << 6) | (c & 0x3f);
  }
  else {
    if (w) {  // utf-8 2 byte
      uni = ((w & 0x1f) << 6) | (c & 0x3f);
    }
    else {
      uni = c;
    }
  }

  int err;

  // MONO renderer provides clean image with perfect crop
  // (no wasted pixels) via bitmap struct.
  if ((err = FT_Load_Char(face, uni, FT_LOAD_TARGET_MONO))) {
    fprintf(stderr, "Error %d loading char 0x%06X\n", err, uni);
    return 0;
  }

    if ((err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO))) {
      fprintf(stderr, "Error %d rendering char 0x%06X\n", err, uni);
      return 0;
  }

  FT_Glyph glyph;
  if ((err = FT_Get_Glyph(face->glyph, &glyph))) {
    fprintf(stderr, "Error %d getting glyph 0x%06X\n", err, uni);
    return 0;
  }

  FT_Bitmap *bitmap = &face->glyph->bitmap;
  FT_BitmapGlyphRec *g = (FT_BitmapGlyphRec *)glyph;

  // Minimal font and per-glyph information is stored to
  // reduce flash space requirements.  Glyph bitmaps are
  // fully bit-packed; no per-scanline pad, though end of
  // each character may be padded to next byte boundary
  // when needed.  16-bit offset means 64K max for bitmaps,
  // code currently doesn't check for overflow.  (Doesn't
  // check that size & offsets are within bounds either for
  // that matter...please convert fonts responsibly.)

  sum = 0;
  bit = 0x80;
  ngph = 0;
  for (int y = 0; y < bitmap->rows; y++) {
    for (int x = 0; x < bitmap->width; x++) {
      int byte = x / 8;
      uint8_t bit = 0x80 >> (x & 7);
      enbit(bitmap->buffer[y * bitmap->pitch + byte] & bit);
    }
  }

  // Pad end of char bitmap to next byte boundary if needed
  int n = (bitmap->width * bitmap->rows) & 7;
  if (n) {     // Pixel count not an even multiple of 8?
    n = 8 - n; // # bits to next multiple
    while (n--)
      enbit(0);
  }
  uint8_t code = (uint8_t)c;
  uint8_t width = bitmap->width;
  uint8_t height = bitmap->rows;
  uint8_t xAdvance = face->glyph->advance.x >> 6;
  int8_t xOffset = g->left;
  int8_t yOffset = g->top;
  uint8_t len0 = ngph % 0x100;
  uint8_t len1 = ngph / 0x100;
  printf("  0x%02X, 0x00, 0x00, %d, %d, %d, 0x%02x, 0x%02x, %d, %d,", code, width, height, xAdvance, (uint8_t)xOffset, (uint8_t)yOffset, len0, len1);

  int j = 8;
  for (int i = 0; i < ngph; ++i) {
    printf(" 0x%02X,", gphbuf[i]);
    if (++j > 15) {
      printf("\n   ");
      j = 0;
    }
  }

  if (j > 15 - (z != 0 ? 1 : 0) - (w != 0 ? 2 : 0) ) {
   	printf("\n   ");
  }

  if (z == 0)
    if (w == 0)
      printf(" // '%c'", c != 0 && c != 9 && c != 10 && c != 13 ? c : 'o');
    else
      printf(" // '%c%c' 0x%02X, 0x%02X,", w, c, (uint8_t)w, (uint8_t)c);
  else
    printf(" // '%c%c%c' 0x%02X, 0x%02X, 0x%02X,", z, w, c, (uint8_t)z, (uint8_t)w, (uint8_t)c);
  printf("\n");
  FT_Done_Glyph(glyph);

//  int16_t len = (bitmap->width * bitmap->rows + 7) / 8;
  return 10 + ngph;
}

// ----------------------------------------------------------------------------
  char TH[0x800][8];
  int THN;

  char fontName[0x80];
  char fract[8];

  int hgh;

void page_start()
{
    printf("static const uint8_t %s_Glyphs_%s[] {\n", fontName, fract);
}

int page_end(const int z, const int w, const int first, const int last)
{
    printf("0};\n\n");

    printf("static const GFXfont %s_%s {\n", fontName, fract);
    printf("  %s_Glyphs_%s,\n", fontName, fract);
    int16_t h = face->size->metrics.height;
    // No face height info, assume fixed width and get from a glyph.
    int c1 = z > 0 ? z : w;
    int c2 = z > 0 ? w : 0;
    printf("  0x%02X, 0x%02X, 0, 0x%02X, 0x%02X, %d\n};\n\n", c1, c2, first, last, h != 0 ? h >> 6 : hgh);

    return 4 + 6;
}

void print_footer(int totsize)
{
  printf("static const GFXfont* %s[] {\n", fontName);
  for (int i = 0; i < THN; ++i) {
    printf("  &%s_%s,\n", fontName, TH[i]);
  }
  printf("  0\n};\n\n");

  totsize += (THN + 1) * 4;

  printf("// Approx. %d bytes\n", totsize);
  // Size estimate is based on AVR struct and pointer sizes;
  // actual size may vary.
}

// ----------------------------------------------------------------------------

void proc_range(int first, int last)
{
  hgh = 0;
  THN = 0;
  int totsize = 0;
  int fr0 = 0;
  int z = 0;
  int w = 0;
  int fr = 0;
  int to = -1;
  for( ; ; ++fr) {

    if (fr > to) {

      if (fr > 0) {
        totsize += page_end(z, w, fr0, to);
      }

      if (z == 0 && w == 0) {
        if (first >= 0xC0) {
          if (fr == 0) {
            fr = 0x20;
            to = fr + 0x1f;
            snprintf(fract, sizeof(fract), "%02X", fr);
          }
          else {
            if (fr < 0x80) {
              to = fr + 0x1f;
              snprintf(fract, sizeof(fract), "%02X", fr);
            }
            else {
              if (first >= 0xE0) {
                z = first;
                w = 0x80;
                snprintf(fract, sizeof(fract), "%02X%02X", z, w);
              }
              else {
                w = first;
                snprintf(fract, sizeof(fract), "%02X", w);
              }
              fr = 0x80;
              to = 0xbf;
            }
          }
        }
        else {
          if (fr == 0) {
            fr = first;
            to = fr + 0x1f;
          }
          else {
            fr = to + 1;
            to = fr + 0x1f;
            if (fr > last) {
              break;
            }
            if (to > last) {
              to = last;
            }
          }
          snprintf(fract, sizeof(fract), "%02X", fr);
        }
      }
      else {
        if (first >= 0xe0) {  // utf-8 3 byte
          if (w < 0xbf) {
            ++w;
          }
          else {
            if (z < last) {
              ++z;
              w = 0x80;
            }
            else {
              break;
            }
          }
          snprintf(fract, sizeof(fract), "%02X%02X", z, w);
        }
        else {
          if (first >= 0xc0) {  // utf-8 2 byte
            if (w < last) {
              ++w;
            }
            else {
              break;
            }
            snprintf(fract, sizeof(fract), "%02X", w);
          }
          else {
            break;
          }
        }
        fr = 0x80;
        to = 0xbf;
      }
      page_start();
      fr0 = fr;
      strncpy(TH[THN++], fract, sizeof(fract));
    }

    totsize += proc_char(z, w, fr);
  }
  print_footer(totsize);
}

// ----------------------------------------------------------------------------

typedef struct {
  int size;
  int len;
  uint32_t* t;

  bool init(const char* t2xname);
  void done();
  void add(uint32_t b4);
  void sort();
  void run();
  void read_file();
  void proc_file(const char* t2xname);
  void t2x_err(const char* msg);

  const char* t2xname;
  int line, pos;
  bool err;
  FILE* file;
  bool getc_(int* c);
  bool getc(int* c);
} b4buf_t;

bool b4buf_t::init(const char* t2xname)
{
  this->t2xname = t2xname;
  size = 500;
  len = 0;
  t = (uint32_t*)malloc(size * sizeof(uint32_t));
  err = 0;
  line = 1;
  pos = 0;

  file = fopen(t2xname, "r");

  if (!file) {
    fprintf(stderr, "File %s can't be opened\n", t2xname);
    err = true;
    return false;
  }
  return true;
}

void b4buf_t::done()
{
  free(t);
}

void b4buf_t::add(uint32_t b4)
{
  if (b4 == 0 || b4 == '\n' || b4 == '\r' || b4 == '\t') {
    return;
  }
  for (int i = 0; i < len; ++i) {
    if (t[i] == b4) {
      return;
    }
  }
  if (len >= size) {
    size += 500;
    t = (uint32_t*)realloc(t, size * sizeof(uint32_t));
  }
  t[len++] = b4;
}

void b4buf_t::sort()
{
  for( int i = 0; i < len - 1; ++i) {
    for( int j = i + 1; j < len; ++j) {
      if (t[j] < t[i]) {
        uint32_t tmp = t[i];
        t[i] = t[j];
        t[j] = tmp;
      }
    }
  }
}

void b4buf_t::run()
{
  hgh = 0;
  THN = 0;
  int totsize = 0;
  int z0 = 0, w0 = 0, first = 0, last = 0, to = -1;
  for( int i = 0; i < len; ++i) {

    uint32_t b4 = t[i];
    int z = b4 >> 16;
    int w = (b4 >> 8) & 0xff;
    int c = b4 & 0xff;

    if (z != z0 || w != w0 || c > to) {

      if (first > 0) {
        totsize += page_end(z0, w0, first, last);
      }

      if (z) {  // utf-8 3 byte
        snprintf(fract, sizeof(fract), "%02X%02X", z, w);
        to = 0xbf;
      }
      else {
        if (w) { // utf-8 2 byte
          snprintf(fract, sizeof(fract), "%02X", w);
          to = 0xbf;
        }
        else {
          snprintf(fract, sizeof(fract), "%02X", c);
          to = c + 0x1f;
        }
      }
      page_start();
      strncpy(TH[THN++], fract, sizeof(fract));
      z0 = z;
      w0 = w;
      first = c;
    }
    last = c;
    totsize += proc_char(z0, w0, c);
  }
  totsize += page_end(z0, w0, first, last);
  print_footer(totsize);
}

const uint32_t toUtf8b4(const uint16_t ucode)
{
  if (ucode & 0xF800) {
    return ((0xE0 | ((ucode >> 12) & 0x0F)) << 16) | ((0x80 | ((ucode >> 6) & 0x3F)) << 8) | (0x80 | (ucode & 0x3F));
  }
  else
  if (ucode & 0x0700) {
    return ((0xC0 | ((ucode >> 6) & 0x1F)) << 8) | (0x80 | (ucode & 0x3F));
  }
  else {
    return (ucode & 0xff);
  }
}

bool isHex(int c)
{
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

void b4buf_t::t2x_err(const char* msg)
{
  fprintf(stderr, "Error in file %s in line %d:%d: %s", t2xname, line, pos, msg);
  err = true;
}

bool b4buf_t::getc_(int* c)
{
  int i = fgetc(file);
  if (i != -1) {
    *c = i;
    return true;
  }
  else {
    *c = 0;
    return false;
  }
}

bool b4buf_t::getc(int* c)
{
  if (getc_(c) && *c != 0) {
    if (*c != '\n') {
      ++pos;
    }
    else {
      ++line;
      pos = 1;
    }
    return true;
  }
  return false;
}

void b4buf_t::read_file()
{
  bool start = true;
  bool unicode = false;
  int c1, c2, c3, c4;
  while (!err && getc_(&c1)) {
    if (start) {
      start = false;
      if (c1 == 0xff) {
        if (!getc_(&c2)) {
          break;
        }
        if (c2 == 0xfe) {
          unicode = true;
        }
        continue;
      }
    }
    if (unicode) {
      if (!getc_(&c2)) {
        break;
      }
      uint16_t u = c1 | (c2 << 8);
      if (!u) {
        break;
      }
      if (u == '\n') {
        ++line;
        pos = 0;
        continue;
      }
      ++pos;
      if (u == ' ' || u == '\t' || u == '\r') {
        continue;
      }
      add(toUtf8b4(u));
      continue;
    }
    if (c1 == '\n') {
      ++line;
      pos = 0;
      continue;
    }
    ++pos;
    if (c1 == ' ' || c1 == '\t' || c1 == '\r') {
      continue;
    }
    if (c1 >= 0xc0) {
      if (getc(&c2)) {
        if (c2 >= 0x80 && c2 <= 0xbf) {
          if (c1 >= 0xe0) {
            if (getc(&c3)) {
              if (c3 >= 0x80 & c3 <= 0xbf) {
                add((c1 << 16) | (c2 << 8) | c3);
              }
              else {
                t2x_err("80..bf expected");
              }
            }
            else {
              t2x_err("unexpected eof");
            }
          }
          else {
            if (c2 >= 0x80 & c2 <= 0xbf) {
              add((c1 << 8) | c2);
            }
            else {
              t2x_err("80..bf expected");
            }
          }
        }
        else {
          t2x_err("80..bf expected");
        }
      }
      else {
        t2x_err("unexpected eof");
      }
    }
    else {
      if (isHex(c1)) {
        if (getc(&c2)) {
          bool h4 = false;
          if (c1 == '0' && c2 == 'x') {
            h4 = true;
            if (!getc(&c1)) {
              t2x_err("unexpected eof");
              break;
            }
            if (!isHex(c1)) {
              t2x_err("0..9 or a..f expected");
              break;
            }
            if (!getc(&c2)) {
              t2x_err("unexpected eof");
              break;
            }
            if (!isHex(c2)) {
              t2x_err("0..9 or a..f expected");
              break;
            }
          }
          if (isHex(c2)) {
            char buf[3] {(char)c1, (char)c2, 0};
            c1 = strtoll(buf, NULL, 16);
            if (getc(&c3)) {
              if (isHex(c3)) {  // unicode
                if (getc(&c4)) {
                  if (isHex(c4)) {
                    buf[0] = c3;
                    buf[1] = c4;
                    c2 = strtoll(buf, NULL, 16);
                    uint16_t u = (c1 << 8) | c2;
                    add(toUtf8b4(u));
                  }
                  else {
                    t2x_err("0..9 or a..f expected");
                  }
                }
                else {
                  t2x_err("unexpected eof");
                }
              }
              else {  // first utf-8 byte in c1
                if (c1 >= 0xc0) {
                  if (c3 == ' ') {
                    if (getc(&c2)) {
                      if (isHex(c2)) {
                        if (getc(&c3)) {
                          if (isHex(c3)) {
                            buf[0] = c2;
                            buf[1] = c3;
                            c2 = strtoll(buf, NULL, 16); // second utf-8 byte in c2
                            if (c2 >= 0x80 && c2 <= 0xbf) {
                              if (c1 >= 0xe0) {
                                if (getc(&c3)) {
                                  if (c3 == ' ') {
                                    if (getc(&c3)) {
                                      if (isHex(c3)) {
                                        if (getc(&c4)) {
                                          if (isHex(c4)) {
                                            buf[0] = c3;
                                            buf[1] = c4;
                                            c3 = strtoll(buf, NULL, 16); // third utf-8 byte in c3
                                            if (c3 >= 0x80 & c3 <= 0xbf) {
                                              add((c1 << 16) | (c2 << 8) | c3);
                                            }
                                            else {
                                              t2x_err("80..bf expected");
                                            }
                                          }
                                          else {
                                          t2x_err("unexpected eof");
                                          }
                                        }
                                        else {
                                          t2x_err("0..9 or a..f expected");
                                        }
                                      }
                                      else {
                                        t2x_err("unexpected eof");
                                      }
                                    }
                                    else {
                                      t2x_err("0..9 or a..f expected");
                                    }
                                  }
                                  else {
                                    t2x_err("space expected");
                                  }
                                }
                                else {
                                  t2x_err("unexpected eof");
                                }
                              }
                              else { // 2 byte utf-8
                                add((c1 << 8) | c2);
                              }
                            }
                            else {
                              t2x_err("80..bf expected");
                            }
                          }
                          else {
                            t2x_err("unexpected eof");
                          }
                        }
                        else {
                          t2x_err("unexpected eof");
                        }
                      }
                      else {
                        t2x_err("unexpected eof");
                      }
                    }
                    else {
                      add(c1);
                    }
                  }
                  else {
                    t2x_err("space expected");
                  }
                }
                else {
                  add(c1);
                }
              }
            }
            else {
              if (h4) {
                t2x_err("0..9 or a..f expected");
              }
              else {
                if (c1 >= 0xc0) {
                  t2x_err("0..9 or a..f expected");
                }
                else {
                  add(c1);
                }
              }
            }
          }
        }
        else {
          add(c1);
        }
      }
      else {
        add(c1);
      }
    }
  }
  fclose(file);
}

void b4buf_t::proc_file(const char* t2xname)
{
  if (init(t2xname)) {
/*
  for (char c = 0x20; c <= 0x7E; ++c) {
    bb.add(c);
  }
*/
    read_file();
    sort();
    run();
    done();
  }
}

// ----------------------------------------------------------------------------
int getint(const char* s)
{
  if (s) {
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
      return strtol(s+2, NULL, 16);
    }
    else {
      return atoi(s);
    }
  }
  return 0;
}

// ----------------------------------------------------------------------------
int main(int argc, char *argv[]) {

  // Parse command line.  Valid syntaxes are:
  //   utf8fontc [filename] [size]
  //   utf8fontc [filename] [size] [-c charset-file]
  //   utf8fontc [filename] [size] [first char]
  //   utf8fontc [filename] [size] [first char] [last char]
  // Unless overridden, default first and last chars are
  // ' ' (space) and '~', respectively

  if (argc < 3) {
    fprintf(stderr, "Usage: %s fontfile size\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile size [first] [last]\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile 12                for ASCII 12pt 0x20 to 0x7E \n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile 18 0x20 0x7E      for ASCII 18pt\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile 24 0x20 0xFE      for ASCII 24pt with latin 1\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile 9  0xC4           for ASCII 9pt  with utf-8 2 bytes starting with 0xC4 - latin Extended A\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile 12 0xC6           for ASCII 12pt with utf-8 2 bytes starting with 0xC6 - latin Extended B\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile 18 0xC0 0xDF      for ASCII 18pt with all utf-8 2 bytes\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile 24 0xE4           for ASCII 24pt with utf-8 3 bytes starting with 0xE4\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile 18 0xEB           for ASCII 18pt with utf-8 3 bytes starting with 0xEB\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile 12 0xE0 0xEF      for ASCII 12pt with all utf-8 3 bytes\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile size [-t sample-text-file] - can be in utf-8 or unicode\n", argv[0]);
    fprintf(stderr, "Usage: %s fontfile 12 -t sample.utf-8 for ASCII 12pt with characters defined in the sample.utf-8\n", argv[0]);
    fprintf(stderr, "use for ttf/ttc font files\n");
    return 1;
  }

  int err;
  char *ptr;
  FT_Library library;

  ptr = strrchr(argv[1], '/'); // Find last slash in filename
  if (ptr) {
    ptr++; // First character of filename (path stripped)
  }
  else {
    ptr = argv[1]; // No path; font in local dir.
  }

  int size = getint(argv[2]);

  // Derive font table names from filename.  Period (filename
  // extension) is truncated and replaced with the font size & bits.
  strcpy(fontName, ptr);
  ptr = strrchr(fontName, '.'); // Find last period (file ext)
  if (!ptr)
    ptr = &fontName[strlen(fontName)]; // If none, append
  // Insert font size and 7/8 bit.  fontName was alloc'd w/extra
  // space to allow this, we're not sprintfing into Forbidden Zone.
  sprintf(ptr, "_%dpt", size);
  // Space and punctuation chars in name replaced w/ underscores.
  int c;
  for (int i = 0; (c = fontName[i]); ++i) {
    if (isspace(c) || ispunct(c))
      fontName[i] = '_';
  }

  // Init FreeType lib, load font
  if ((err = FT_Init_FreeType(&library))) {
    fprintf(stderr, "FreeType init error: %d", err);
    return err;
  }

  // Use TrueType engine version 35, without subpixel rendering.
  // This improves clarity of fonts since this library does not
  // support rendering multiple levels of gray in a glyph.
  // See https://github.com/adafruit/Adafruit-GFX-Library/issues/103
  FT_UInt interpreter_version = TT_INTERPRETER_VERSION_35;
  FT_Property_Set(library, "truetype", "interpreter-version",
                  &interpreter_version);

  if ((err = FT_New_Face(library, argv[1], 0, &face))) {
    fprintf(stderr, "Font load error: %d", err);
    FT_Done_FreeType(library);
    return err;
  }

  // << 6 because '26dot6' fixed-point format
  FT_Set_Char_Size(face, size << 6, 0, DPI, 0);


  if (argc >= 5 && !strcmp(argv[3], "-t")) {
    b4buf_t bb;
    bb.proc_file(argv[4]);
  }
  else {
    int first = 0x20, last = 0;

    if (argc >= 4) {
      first = getint(argv[3]);
    };
    if (argc >= 5) {
      last = getint(argv[4]);
    }

    if (first >= 0x100) {
      fprintf(stderr, "Value of first: 0x%x is to big\n", first);
      return 1;
    }
    if (first >= 0xf0) {  // 4 byte utf-8
      fprintf(stderr, "4 byte utf-8 not supported yet.\n");
      return 1;
    }
    if (first >= 0xe0) {  // 3 byte utf-8
      if (last != 0) {
        if (last < first || last >= 0xf0) {
          fprintf(stderr, "Value of last: 0x%x for first (0xe0 to 0xef) should be >= first, but not too much.\n", last);
          return 1;
        }
      }
      else {
        last = first;
      }
    }
    else
    if (first >= 0xc0) {  // 2 byte utf-8
      if (last != 0) {
        if (last < first || last >= 0xe0) {
          fprintf(stderr, "Value of last: 0x%x for first (0xc0 to 0xdf) should be >= first, but not too much.\n", last);
          return 1;
        }
      }
      else {
        last = first;
      }
    }
    else {  // ASCII
      if (last != 0) {
        if (last < first || last >= 0x100) {
          fprintf(stderr, "Value of last: 0x%x for first (0x00 to 0xbf) should be >= first, but <= 0xff.\n", last);
          return 1;
        }
      }
      else {
        last = first >= 0x80 ? 0xff : 0x7e;
      }
    }

    // Currently all symbols from 'first' to 'last' are processed.
    // Fonts may contain WAY more glyphs than that, but this code
    // will need to handle encoding stuff to deal with extracting
    // the right symbols, and that's not done yet.
    // fprintf(stderr, "%ld glyphs\n", face->num_glyphs);

    proc_range(first, last);
  }
  FT_Done_FreeType(library);

  return 0;
}

/* -------------------------------------------------------------------------

Character metrics are slightly different from classic GFX & ftGFX.
In classic GFX: cursor position is the upper-left pixel of each 5x7
character; lower extent of most glyphs (except those w/descenders)
is +6 pixels in Y direction.
W/new GFX fonts: cursor position is on baseline, where baseline is
'inclusive' (containing the bottom-most row of pixels in most symbols,
except those with descenders; ftGFX is one pixel lower).

Cursor Y will be moved automatically when switching between classic
and new fonts.  If you switch fonts, any print() calls will continue
along the same baseline.

                    ...........#####.. -- yOffset
                    ..........######..
                    ..........######..
                    .........#######..
                    ........#########.
   * = Cursor pos.  ........#########.
                    .......##########.
                    ......#####..####.
                    ......#####..####.
       *.#..        .....#####...####.
       .#.#.        ....##############
       #...#        ...###############
       #...#        ...###############
       #####        ..#####......#####
       #...#        .#####.......#####
====== #...# ====== #*###.........#### ======= Baseline
                    || xOffset

glyph->xOffset and yOffset are pixel offsets, in GFX coordinate space
(+Y is down), from the cursor position to the top-left pixel of the
glyph bitmap.  i.e. yOffset is typically negative, xOffset is typically
zero but a few glyphs will have other values (even negative xOffsets
sometimes, totally normal).  glyph->xAdvance is the distance to move
the cursor on the X axis after drawing the corresponding symbol.

There's also some changes with regard to 'background' color and new GFX
fonts (classic fonts unchanged).  See Adafruit_GFX.cpp for explanation.
*/

#endif /* !ARDUINO */
