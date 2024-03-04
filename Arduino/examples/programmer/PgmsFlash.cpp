/*
  Programmer project on TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include "Pgms.h"

#if defined(ARDUINO_ARCH_RP2040)

  #include <PicoFlash.h>

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

#elif defined(ESP32)

  #include <Preferences.h>
  Preferences preferences;

  void readPgmsFromEEProm(from_to_t PGMS[])
  {
    preferences.getBytes("PGMS", PGMS, sizeof(from_to_t) * N_OF_PGMS);
  }

  void storePgmsInEEProm(const from_to_t PGMS[])
  {
    preferences.putBytes("PGMS", PGMS, sizeof(from_to_t) * N_OF_PGMS);
  }

#elif defined(ARDUINO_ARCH_ESP8266)

  void readPgmsFromEEProm(from_to_t PGMS[])
  {
  }

  void storePgmsInEEProm(const from_to_t PGMS[])
  {
  }

#else

  void readPgmsFromEEProm(from_to_t PGMS[])
  {
  }

  void storePgmsInEEProm(const from_to_t PGMS[])
  {
  }

#endif
