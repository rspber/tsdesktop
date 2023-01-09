/*
  Interface to pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "Arduino.h"

#include <pico/stdio.h>
#include "pico/binary_info.h"
#include "hardware/sync.h"

/*
#include <Display.h>

void debug(const char *buf)
{
  if (screenEnabled) {
    clip_t clip{0, 0, display.width(), display.height()};
    display.fillRect(&clip, 0, 0, display.width(), 8, BLACK);
    display.drawText(0, 0, buf, 1, WHITE);
  }
}
*/

// ----------------------------- Init hardware --------------------------------

void init_hardware()
{
  stdio_init_all();
}

// ------------------------------- Interrupts ---------------------------------

uint32_t noInterrupts(void)
{
  return save_and_disable_interrupts();
}

void interrupts(uint32_t ints)
{
  restore_interrupts(ints);
}

// ---------------------------------- GPIO ------------------------------------

void pinMode(const int16_t pin, const uint8_t mode)
{
  gpio_init(pin);
  gpio_put(pin, HIGH);
  if (mode == INPUT) {
    gpio_set_dir(pin, GPIO_IN);
  }
  else {
    if (mode == INPUT_PULLUP) {
      gpio_set_dir(pin, GPIO_IN);
      gpio_pull_up(pin);
    }
    else {
      gpio_set_dir(pin, GPIO_OUT);
    }
  }
}

void digitalWrite(const int16_t pin, uint8_t data)
{
  gpio_put(pin, data);
}

bool digitalRead(uint8_t pin)
{
  return gpio_get(pin);
}

// ---------------------------------- time ------------------------------------

unsigned long millis(void)
{
  return time_us_64() / 1000;
}

unsigned long micros()
{
  return time_us_64();
}

void delay(const int ms)
{
  sleep_ms(ms);
}

// ----------------------------------- SPI ------------------------------------

uint current_spi0_speed = 0;
uint current_spi1_speed = 0;

spi_inst_t* init_spi(const uint8_t RX, const uint8_t SCK, const uint8_t TX, const uint8_t spi_nr, const uint Hz)
{
  spi_inst_t* spi = spi_nr == 0 ? spi0 : spi1;

  if (current_spi0_speed || current_spi1_speed) {
    return spi;
  }

  spi_init(spi, Hz);
  //    spi_set_format(spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);  - default
  //    spi_set_format(spi, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);  - problematic
  gpio_set_function(RX, GPIO_FUNC_SPI);    // MISO
  gpio_set_function(SCK, GPIO_FUNC_SPI);    // CLK
  gpio_set_function(TX, GPIO_FUNC_SPI);    // MOSI
  // Make the SPI pins available to picotool
  bi_decl(bi_3pins_with_func(RX, TX, SCK, GPIO_FUNC_SPI));

  (spi_nr == 0 ? current_spi0_speed : current_spi1_speed) = Hz;
  return spi;
}

void set_spi_speed(spi_inst_t* spi, const uint Hz)
{
  if (spi == spi0) {
    if (current_spi0_speed != Hz) {
      spi_set_baudrate(spi, Hz);
      current_spi0_speed = Hz;
    }
  }
  else {
    if (current_spi1_speed != Hz) {
      spi_set_baudrate(spi, Hz);
      current_spi1_speed = Hz;
    }
  }
}

// ----------------------------------- I2C ------------------------------------

i2c_inst_t* init_i2c(const uint8_t SDA, const uint8_t SCL, const uint8_t i2c_nr, const uint Hz)
{
  // I2C is "open drain", pull ups to keep signal high when no data is being sent
  i2c_inst_t* i2c = i2c_nr == 0 ? i2c0 : i2c1;
  i2c_init(i2c, Hz);
  gpio_set_function(SDA, GPIO_FUNC_I2C);
  gpio_set_function(SCL, GPIO_FUNC_I2C);
  gpio_pull_up(SDA);
  gpio_pull_up(SCL);
  return i2c;
}

