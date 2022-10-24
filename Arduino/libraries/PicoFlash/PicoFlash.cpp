/*
  Flash for pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "PicoFlash.h"
#include <cstring>

#include <Arduino.h>

void clear_flash_buffer()
{
  memset(flash_buffer, 0, sizeof(flash_buffer));
}

void copy_to_flash_buffer(const void* data, const int size)
{
  memcpy(flash_buffer, data, size < (int)sizeof(flash_buffer) ? size : sizeof(flash_buffer));
}

void copy_from_flash_buffer(void* data, const int size)
{
  memcpy(data, flash_buffer, size < (int)sizeof(flash_buffer) ? size : sizeof(flash_buffer));
}

const int pico_offset(const int sector, const int page)
{
  return (MY_FLASH_FIRST_SECTOR + sector) * FLASH_SECTOR_SIZE + page * FLASH_PAGE_SIZE;
}

const uint8_t* pico_ptr(const int sector, const int page, const int pos)
{
  return (uint8_t*)(XIP_BASE + pico_offset(sector, page) + pos);
}

void pico_read(const int sector, const int page, uint8_t* buffer, const int nofpages)
{
  memcpy(buffer, pico_ptr(sector, page, 0), nofpages * FLASH_PAGE_SIZE);
}

void pico_read(const int sector, const int page, uint8_t* buffer)
{
  pico_read(sector, page, buffer, 1);
}

void pico_read_into_flash_buffer(const int sector, const int page)
{
  pico_read(sector, page, flash_buffer, 1);
}

void pico_prog(const int sector, const int page, const uint8_t* buffer, const int nofpages)
{
  noInterrupts();
  rp2040.idleOtherCore();
  flash_range_program(pico_offset(sector, page), buffer, nofpages * FLASH_PAGE_SIZE);
  rp2040.resumeOtherCore();
  interrupts();
}

void pico_erase(const int sector, const int nofsect)
{
  noInterrupts();
  rp2040.idleOtherCore();
  flash_range_erase(pico_offset(sector, 0), nofsect * FLASH_SECTOR_SIZE);
  rp2040.resumeOtherCore();
  interrupts();
}

void pico_write(const int sector, const int page, const uint8_t* buffer, const int nofpages)
{
  noInterrupts();
  rp2040.idleOtherCore();
  flash_range_erase(pico_offset(sector, 0), FLASH_SECTOR_SIZE);
  flash_range_program(pico_offset(sector, page), buffer, nofpages * FLASH_PAGE_SIZE);
  rp2040.resumeOtherCore();
  interrupts();
}

void pico_write(const int sector, const int page, const uint8_t* buffer)
{
  pico_write(sector, page, buffer, 1);
}

void pico_write_from_flash_buffer(const int sector, const int page)
{
  pico_write(sector, page, flash_buffer, 1);
}
