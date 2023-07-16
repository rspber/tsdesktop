/*
  DS3231 for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <Wire.h>

typedef struct
{
  uint8_t sec;
  uint8_t min;
  uint8_t hour; //24 hour time
  uint8_t dow; //0-6 -> sunday - Saturday
  uint8_t day;
  uint8_t month;
  uint8_t year;

  char aging;

  void bad();
} dttm_t;

//#define DS1307_ADDRESS 0x68
#define DS3231_ADDRESS 0x68

class TSD_DS3231 : public TwoWire {
public:
  TSD_DS3231(const uint8_t i2c_nr = 0, const uint8_t deviceAddress = DS3231_ADDRESS)
    : TwoWire(deviceAddress, i2c_nr)
  {
  }

  void setDateTime(const dttm_t* dttm);
  void getDateTime(dttm_t* dttm);
  const char setAging(dttm_t* dttm, const char aging);
  const char getAging(dttm_t* dttm);
  const int up(dttm_t* dttm, const int8_t what, const int8_t up);

private:
  const uint8_t getDS3231Reg(const uint8_t reg);
  const uint8_t getDS3231RegBcdToDec(const uint8_t reg);
  const bool setDS3231Reg(const uint8_t reg, const uint8_t value);
  const uint8_t setDS3231RegDecToBcd(const uint8_t reg, const uint8_t value);
};

const bool dttm_eq(const dttm_t& t1, const dttm_t& t2);

const char* dow(const int8_t aDay);
