/*
  DS3231 for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "TSD_DS3231.h"

TwoWire* init_i2c(const uint8_t i2c_nr);

void TSD_DS3231::begin()
{
  _wire = init_i2c(_i2c_nr);
}

uint8_t zero = 0x00; //workaround for issue #527

/**
  year     0-99
  month    1-12
  day      1-31
  dow      1-7  or 0
  hour     0-23
  min      0-59
  sec      0-59
 */

void dttm_t::bad()
{
  year = -1;
  month = -1;
  day = -1;
  hour = -1;
  min = -1;
  sec = -1;
  dow = -1;
  aging = -128;
}

const bool dttm_eq(const dttm_t& t1, const dttm_t& t2)
{
  return
    t1.year == t2.year &&
    t1.month == t2.month &&
    t1.day == t2.day &&
    t1.dow == t2.dow &&
    t1.hour == t2.hour &&
    t1.min == t2.min &&
    t1.sec == t2.sec/* &&
    t1.aging == t2.aging*/;
}

const char* dow(const int8_t aDay)
{
  switch (aDay) {
  case 1: return "Sun";
  case 2: return "Mon";
  case 3: return "Tue";
  case 4: return "Wed";
  case 5: return "Thu";
  case 6: return "Fri";
  case 7: return "Sat";
  default: return "***";
  }
}

/**
 * Convert normal decimal numbers to binary coded decimal
 */
uint8_t decToBcd(uint8_t val)
{
  return val / 10 * 16 + val % 10;
}

/**
 * Convert binary coded decimal to normal decimal numbers
 */
uint8_t bcdToDec(uint8_t val)
{
  return val / 16 * 10 + val % 16;
}

/*
void setDateTime( uint8_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t min, uint8_t sec )
{
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write(zero); //stop Oscillator

  Wire.write(decToBcd(sec));
  Wire.write(decToBcd(min));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dow));
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));

  Wire.write(zero); //start

  Wire.endTransmission();
}
*/
const uint8_t TSD_DS3231::getDS3231Reg(const uint8_t reg)
{
  _wire->beginTransmission(_addr);
  _wire->write(reg);
  _wire->endTransmission();
  _wire->requestFrom(_addr, (uint8_t)1);
  return _wire->read();
}

const uint8_t TSD_DS3231::getDS3231RegBcdToDec(const uint8_t reg)
{
  return bcdToDec(getDS3231Reg(reg));
}

const bool TSD_DS3231::setDS3231Reg(const uint8_t reg, const uint8_t value)
{
  _wire->beginTransmission(_addr);
  _wire->write(reg);
  _wire->write(value);
  return _wire->endTransmission() == 0;
}

const uint8_t TSD_DS3231::setDS3231RegDecToBcd(const uint8_t reg, const uint8_t value)
{
  setDS3231Reg(reg, decToBcd(value));
  return value;
}

void TSD_DS3231::getDateTime(dttm_t* dttm)
{
  _wire->beginTransmission(_addr);
  _wire->write(zero);
  _wire->endTransmission();

  if (_wire->requestFrom(_addr, (uint8_t)7) == 7) {
    dttm->sec = bcdToDec(_wire->read());
    dttm->min = bcdToDec(_wire->read());
    dttm->hour = bcdToDec(_wire->read() & 0b111111); //24 hour time
    dttm->dow = bcdToDec(_wire->read()); //1-7 -> Sunday - Saturday
    dttm->day = bcdToDec(_wire->read());
    dttm->month = bcdToDec(_wire->read() & 0x7f);  // cut century
    dttm->year = bcdToDec(_wire->read());
  }
}

const char TSD_DS3231::getAging(dttm_t* dttm)
{
  uint8_t b = getDS3231Reg(0x10);
  return (dttm->aging = b < 128 ? b : b - 256);
}

const char TSD_DS3231::setAging(dttm_t* dttm, const char aging)
{
  dttm->aging = aging;
  setDS3231Reg(0x10, aging >= 0 ? aging : aging + 256);

  // Since normally the TCXO is adjusted every 64 seconds, in order for the changes to take effect immediately,
  // we need to force the CONV bit (bit 5) to high in the control register (0x0E):

  uint8_t b = getDS3231Reg(0x0E);
  b |= 0x20;
  setDS3231Reg(0x0E, b);
  return aging;
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

const int TSD_DS3231::up(dttm_t* dttm, const int8_t what, const int8_t up)
{
  switch (what) {
  case 1:
    return (dttm->dow = setDS3231RegDecToBcd(0x03, to_min_max(dttm->dow + up, 1, 7)));
  case 2:
    return (dttm->year = setDS3231RegDecToBcd(0x06, to_min_max(dttm->year + up, 0, 99)));
  case 3:
    return (dttm->month = setDS3231RegDecToBcd(0x05, to_min_max(dttm->month + up, 1, 12)));
  case 4:
    return (dttm->day = setDS3231RegDecToBcd(0x04, to_min_max(dttm->day + up, 1, 31)));
  case 5:
    return (dttm->hour = setDS3231RegDecToBcd(0x02, to_min_max(dttm->hour + up, 0, 23)));
  case 6:
    return (dttm->min = setDS3231RegDecToBcd(0x01, to_min_max(dttm->min + up, 0, 59)));
  case 7:
    return (dttm->sec = setDS3231RegDecToBcd(0x00, to_min_max(dttm->sec + up, 0, 59)));
  case 8:
    return setAging(dttm, to_min_max(dttm->aging + up, -127, +128));
  default:
    return 0;
  }
}
