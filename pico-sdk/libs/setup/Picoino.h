/*
  Interface to pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
  Picoino contains shortcuts to spi and i2c interfaces and
  very very common functions from Arduino, implemented on pico-sdk
*/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <math.h>

#include <pico/printf.h>
#include <pico/time.h>

#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PROGMEM

//void debug(const char *buf);

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

#define bit(b) (1UL << (b))

typedef bool boolean;
typedef uint8_t byte;
typedef uint16_t word;

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))

// ------------------------------- Interrupts ---------------------------------

uint32_t noInterrupts(void);
void interrupts(uint32_t);

// ---------------------------------- GPIO ------------------------------------

void pinMode(const int16_t pin, const uint8_t mode);
void digitalWrite(const int16_t pin, uint8_t data);
bool digitalRead(uint8_t pin);

// ---------------------------------- time ------------------------------------

unsigned long millis(void);
unsigned long micros(void);

void delay(const int ms);

// ----------------------------------- SPI ------------------------------------

#define IDLE_SPI_SPEED 40 * 1000 * 1000   // 40 MHz

void init_spi(const uint8_t RX, const uint8_t SCK, const uint8_t TX, spi_inst_t* spi = spi0, const uint Hz = IDLE_SPI_SPEED);
void init_spi1(const uint8_t RX, const uint8_t SCK, const uint8_t TX, const uint Hz = IDLE_SPI_SPEED);

void inline cs_select(const uint8_t CS, const uint8_t mode)
{
  asm volatile("nop \n nop \n nop");
  gpio_put(CS, mode);
  asm volatile("nop \n nop \n nop");
}

// short init_spi function
// section 0:  spi0 gpio's: RX(MISO)  0, CS(available)  1, SCK(CLK)  2, TX(MOSI)  3
// section 1:  spi0 gpio's: RX(MISO)  4, CS(available)  5, SCK(CLK)  6, TX(MOSI)  7
// section 2:  spi1 gpio's: RX(MISO)  8, CS(available)  9, SCK(CLK) 10, TX(MOSI) 11
// section 3:  spi1 gpio's: RX(MISO) 12, CS(available) 13, SCK(CLK) 14, TX(MOSI) 15    // spi1 picoino default
// section 4:  spi0 gpio's: RX(MISO) 16, CS(available) 17, SCK(CLK) 18, TX(MOSI) 19    // spi0 pico-sdk & picoino default

typedef struct {
  const int8_t section;
} spi_section_t;

#define NO_SPI_SECT spi_section_t{-1}

#define SPI_SECT_0 spi_section_t{0}
#define SPI_SECT_1 spi_section_t{1}
#define SPI_SECT_2 spi_section_t{2}
#define SPI_SECT_3 spi_section_t{3}      // spi1 picoino default
#define SPI_SECT_4 spi_section_t{4}      // spi0 pico-sdk & picoino default

#define SPI0_SECT SPI_SECT_4        // spi0 pico-sdk & picoino default
#define SPI1_SECT SPI_SECT_3        // spi1 picoino default

void init_spi(spi_section_t section = SPI0_SECT, const uint Hz = IDLE_SPI_SPEED);

void set_spi_speed(spi_inst_t* spi, const uint Hz);

// --------------------------------- PicoSPI ----------------------------------

#define SPI_SPEED 40 * 1000 * 1000

class PicoSPI {
public:
  PicoSPI(const int16_t CS, const int16_t DC, const uint Hz = SPI_SPEED, spi_inst_t* spi = spi0)
  {
    _CS = CS;
    pinMode(CS, OUTPUT);
    _DC = DC;
    if (DC >= 0) {
      pinMode(DC, OUTPUT);
    }
    _spi = spi;
    spi_speed = Hz;
  }

  void inline cs(const uint8_t mode)
  {
    cs_select(_CS, mode);
  }

  void inline dc(const uint8_t mode)
  {
    if (_DC >= 0) {
      gpio_put(_DC, mode);
    }
  }

  void beginTransaction(const uint Hz)
  {
    cs(1);  // Just in case it has been left low
    set_spi_speed(_spi, Hz);
  }

  // default speed transaction
  void beginTransaction()
  {
    beginTransaction(spi_speed);
  }

  void endTransaction()
  {
    cs(1);
  }
/*
  void inline write(const uint8_t *data, const int16_t size)
  {
    spi_write_blocking(_spi, data, size);
  }
*/
  void inline writeCmd(const uint8_t cmd)
  {
    dc(0);
    cs(0);
    spi_write_blocking(_spi, &cmd, 1);
    cs(1);
    dc(1);
  }

  void inline writeData(const uint8_t* data, const int16_t size)
  {
    dc(1);
    cs(0);
    spi_write_blocking(_spi, data, size);
    cs(1);
    dc(1);
   }

  void writeCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size)
  {
    dc(0);
    cs(0);
    spi_write_blocking(_spi, &cmd, 1);
    if (size > 0) {
      dc(1);
      spi_write_blocking(_spi, data, size);
    }
    cs(1);
    dc(1);
  }

  // SPI read

  const uint8_t transfer(const uint8_t cmd)
  {
    uint8_t data;
    spi_read_blocking(_spi, cmd, &data, 1);
    return data;
  }

  const uint16_t transfer16(const uint8_t cmd)
  {
    uint8_t data[2];
    spi_read_blocking(_spi, cmd, data, 2);
    return (data[0] << 8) | data[1];
  }

private:
  int16_t _CS, _DC;
  spi_inst_t* _spi;
  uint spi_speed;
};

// ----------------------------------- I2C ------------------------------------

#define DEF_I2C_HZ 100 * 1000

void init_i2c(const uint8_t SDA, const uint8_t SCL, i2c_inst_t* i2c, const uint Hz = DEF_I2C_HZ);
void init_i2c0(const uint8_t SDA, const uint8_t SCL, const uint Hz = DEF_I2C_HZ);
void init_i2c1(const uint8_t SDA, const uint8_t SCL, const uint Hz = DEF_I2C_HZ);

// section 0:  gpio's:  i2c0:  0,  1   i2c1:  2,  3
// section 1:  gpio's:  i2c0:  4,  5   i2c1:  6,  7
// section 2:  gpio's:  i2c0:  8,  9   i2c1: 10, 11
// section 3:  gpio's:  i2c0: 12, 13   i2c1: 14, 15
// section 4:  gpio's:  i2c0: 16, 17   i2c1: 18, 19

typedef struct {
  const int8_t section;
} i2c_section_t;

#define NO_I2C_SECT i2c_section_t{-1}

#define I2C_SECT_0 i2c_section_t{0}
#define I2C_SECT_1 i2c_section_t{1}
#define I2C_SECT_2 i2c_section_t{2}
#define I2C_SECT_3 i2c_section_t{3}
#define I2C_SECT_4 i2c_section_t{4}
#define I2C_SECT_5 i2c_section_t{5}      // not for i2c1
#define I2C_SECT_6 i2c_section_t{6}      // not for i2c0

// ---------------------------------- I2C0 ------------------------------------

//#define PICO_DEFAULT_I2C_SDA_PIN 4    // i2c0 SDA pico-sdk default
//#define PICO_DEFAULT_I2C_SCL_PIN 5    // i2c0 SCL pico-sdk default

// short init_i2c0 function
// I2C0 section: n (0 .. 5)
// n * 4 + 0   i.e. 0, 4, 8, 12, 16, 20
// n * 4 + 1   i.e, 1, 5, 9, 13, 17, 21
// section 0: i2c0: SDA =  0, SCL =  1
// section 1: i2c0: SDA =  4, SCL =  5    // i2c0 pico-sdk & picoino default
// section 2: i2c0: SDA =  8, SCL =  9
// section 3: i2c0: SDA = 12, SCL = 13
// section 4: i2c0: SDA = 16, SCL = 17
// section 5: i2c0: SDA = 20, SCL = 21

#define I2C0_SECT I2C_SECT_1

void init_i2c0(i2c_section_t section = I2C0_SECT, const uint Hz = DEF_I2C_HZ);

// ---------------------------------- I2C1 ------------------------------------

#define I2C1_SDA_PIN 6            // i2c1 SDA picoino default
#define I2C1_SCL_PIN 7            // i2c1 SCL picoino default

// short init_i2c1 function
// I2C1 section: n (0 .. 4, 6)
// n * 4 + 2   i.e. 2, 6, 10, 14, 18
// n * 4 + 3   i.e, 3, 7, 11, 15, 19
// section 0: i2c1: SDA =  2, SCL =  3
// section 1: i2c1: SDA =  6, SCL =  7    // i2c1 picoino default
// section 2: i2c1: SDA = 10, SCL = 11
// section 3: i2c1: SDA = 14, SCL = 15
// section 4: i2c1: SDA = 18, SCL = 19
//
// section 6: i2c1: SDA = 26, SCL = 27    // ?

#define I2C1_SECT I2C_SECT_1

void init_i2c1(i2c_section_t section = I2C1_SECT, const uint Hz = DEF_I2C_HZ);

// --------------------------------- PicoI2C ----------------------------------

class PicoI2C {
public:
  PicoI2C(const uint8_t addr, i2c_inst_t* i2c = i2c0)
  {
    _i2c = i2c;
    _addr = addr;
  }

  // I2C read
  void read(uint8_t* dst, const int16_t len, const bool nostop = false);
  const uint8_t read8(const bool nostop = false);
  const uint16_t read16(const bool nostop = false);

  // I2C write
  void write(const uint8_t* src, const int16_t len, const bool nostop = false);
  void write8(const uint8_t endian, const bool nostop = false);
  void write8write8(const uint8_t reg, const uint8_t value, const bool nostop = false);
  void write16(const uint16_t endian, const bool nostop = false);

  // I2C write and read
  void write8read(const uint8_t reg, uint8_t* dst, const int16_t len);
  const uint8_t write8read8(const uint8_t reg);

private:
  i2c_inst_t* _i2c;
  uint8_t _addr;
};
