/*
  Programmer project on TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <TSD_DS3231.h>
#include <TSD_PCF8575.h>

typedef struct {
  uint8_t hour;
  uint8_t min;
} hour_min_t;

typedef struct {
  hour_min_t start, stop;
} from_to_t;

#define N_OF_PGMS 8

void readPgmsFromEEProm(from_to_t PGMS[]);
void storePgmsInEEProm(const from_to_t PGMS[]);

const int pgm_value_up(from_to_t PGMS[], const int8_t block, const int8_t pos, const int8_t up);
void onOffPgms(TSD_PCF8575* pcf8575, const from_to_t PGMS[], const dttm_t tm);
