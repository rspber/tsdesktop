/*
  Interface to pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
  Arduino contains shortcuts to spi and i2c interfaces and
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

// ----------------------------- Init hardware --------------------------------

void init_hardware();

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
// section 3:  spi1 gpio's: RX(MISO) 12, CS(available) 13, SCK(CLK) 14, TX(MOSI) 15
// section 4:  spi0 gpio's: RX(MISO) 16, CS(available) 17, SCK(CLK) 18, TX(MOSI) 19    // spi0 pico-sdk default

spi_inst_t* init_spi(const uint8_t RX, const uint8_t SCK, const uint8_t TX, const uint8_t spi_nr, const uint Hz);

void set_spi_speed(spi_inst_t* spi, const uint Hz);

// ----------------------------------- I2C ------------------------------------

#define DEF_I2C_HZ 100 * 1000

// section 0:  gpio's:  i2c0:  0,  1   i2c1:  2,  3
// section 1:  gpio's:  i2c0:  4,  5   i2c1:  6,  7
// section 2:  gpio's:  i2c0:  8,  9   i2c1: 10, 11
// section 3:  gpio's:  i2c0: 12, 13   i2c1: 14, 15
// section 4:  gpio's:  i2c0: 16, 17   i2c1: 18, 19

//#define PICO_DEFAULT_I2C_SDA_PIN 4    // i2c0 SDA pico-sdk default
//#define PICO_DEFAULT_I2C_SCL_PIN 5    // i2c0 SCL pico-sdk default

i2c_inst_t* init_i2c(const uint8_t SDA, const uint8_t SCL, const uint8_t i2c_nr, const uint Hz = DEF_I2C_HZ);
