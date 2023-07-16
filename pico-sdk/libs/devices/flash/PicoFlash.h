/*
  Flash for pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <stdint.h>
#include "hardware/flash.h"

// Define the flash sizes
// This is setup to read a block of the flash from the end

//#define FLASH_PAGE_SIZE (1u << 8)                       256B
//#define FLASH_SECTOR_SIZE (1u << 12)                     4kB      // 16 pages per sector
//#define FLASH_BLOCK_SIZE (1u << 16)                     64kB      // 16 sectors per block
//#define PICO_FLASH_SIZE_BYTES (2 * 1024 * 1024)          2MB
#define PICO_NOF_SECTORS (PICO_FLASH_SIZE_BYTES >> 12)              // default is 512 sectors in EEPROM
//#define MY_FLASH_STORAGE_BYTES (16 * 4096)         //     64kB      my storage
#define MY_FLASH_FIRST_SECTOR (PICO_NOF_SECTORS - 1 - 16)           // my first sector is 512 - 1 - 16


const int pico_flash_offset(const int sector, const int page);
const uint8_t* pico_flash_ptr(const int sector, const int page, const int pos);
void pico_flash_read(const int sector, const int page, uint8_t* buffer, const int nofpages);
void pico_flash_read(const int sector, const int page, uint8_t* buffer);
void pico_flash_prog(const int sector, const int page, const uint8_t* buffer, const int nofpages);
void pico_flash_erase(const int sector, const int nofsect);
void pico_flash_write(const int sector, const int page, const uint8_t* buffer, const int nofpages);
void pico_flash_write(const int sector, const int page, const uint8_t* buffer);

void pico_read_into_flash_buffer(const int sector, const int page);
void pico_write_from_flash_buffer(const int sector, const int page);

void clear_flash_buffer();
void copy_to_flash_buffer(const void* data, const int length);
void copy_from_flash_buffer(void* data, const int length);

extern uint8_t flash_buffer[FLASH_PAGE_SIZE];
