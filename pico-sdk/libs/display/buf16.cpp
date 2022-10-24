/*
  Pixel's buffer for TSD_ILI9341

  FreeBSD License.

*/

#include "buf16.h"

#define BUFLEN 2048

uint16_t buffer[BUFLEN];

uint16_t* uint16_color(uint16_t* buf, const uint16_t color, int size)
{
  if (size > BUFLEN) {
    size = BUFLEN;
  }
  uint16_t* p = buf;
  for (int i = 0; i < size; ++i) {
    *p++ = SWAP16(color);
  }
  return buf;
}

uint16_t* uint16_array(uint16_t* buf, int size)
{
  if (size > BUFLEN) {
    size = BUFLEN;
  }
  uint16_t* p = buf;
  for (int i = 0; i < size; ++i) {
    *p++ = 0;
  }
  return buf;
}

uint16_t* buffer_uint16_color(const uint16_t color, const int size)
{
  return uint16_color(buffer, color, size);
}

uint16_t* buffer_uint16_array(const int size)
{
  return uint16_array(buffer, size);
}
