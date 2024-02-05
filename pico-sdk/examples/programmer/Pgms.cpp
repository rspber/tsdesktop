/*
  Programmer project on TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include "Pgms.h"
#include <PicoFlash.h>
#include <TSD_PCF8575.h>

#define PGMS_SECTOR  0
#define PGMS_PAGE    0

uint8_t flash_buffer[FLASH_PAGE_SIZE];

void readPgmsFromEEProm(from_to_t PGMS[])
{
  pico_read_into_flash_buffer(PGMS_SECTOR, PGMS_PAGE);
  copy_from_flash_buffer(PGMS, sizeof(from_to_t) * N_OF_PGMS);
}

void storePgmsInEEProm(const from_to_t PGMS[])
{
  clear_flash_buffer();
  copy_to_flash_buffer(PGMS, sizeof(from_to_t) * N_OF_PGMS);
  pico_write_from_flash_buffer(PGMS_SECTOR, PGMS_PAGE);
}

int inline to_min_max(int v, int min, int max)
{
  if (v < min) {
    return max;
  }
  else {
    if (v > max) {
      return min;
    }
  }
  return v;
}

const int pgm_value_up(from_to_t PGMS[], const int8_t block, const int8_t pos, const int8_t up)
{
  if (block > 0 && block <= N_OF_PGMS) {
    from_to_t* d = &PGMS[block - 1];
    switch (pos - 1) {
    case 0: return (d->start.hour = to_min_max(d->start.hour + up, 0, 23));
    case 1: return (d->start.min = to_min_max(d->start.min + up, 0, 59));
    case 2: return (d->stop.hour = to_min_max(d->stop.hour + up, 0, 23));
    case 3: return (d->stop.min = to_min_max(d->stop.min + up, 0, 59));
    }
  }
  return 0;
}

void onOffPgms(TSD_PCF8575* pcf8575, const from_to_t PGMS[], const dttm_t tm)
{
  int curr = tm.hour * 24 + tm.min;
  uint16_t bit16 = 0;
  for (int i = N_OF_PGMS; --i >= 0; ) {
    const from_to_t* p = &PGMS[i];
    int start = p->start.hour * 24 + p->start.min;
    int stop = p->stop.hour * 24 + p->stop.min;
    bit16 = (bit16 << 1) | (start <= curr && curr < stop);
  }
  pcf8575->write16(~bit16);
}
