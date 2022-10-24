/*
  Interface to pico-sdk

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "Picoino.h"

#include "pico/binary_info.h"
#include "hardware/sync.h"

/*
#include "<Display.h>

void debug(const char *buf)
{
  if( screenEnabled ) {
//    display.fillRect(0, 0, 240, 8, BLACK);
    display.drawText(0, 0, buf, 1, WHITE);
  }
}
*/

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
  if (mode == INPUT) {
    gpio_set_dir(pin, GPIO_IN);
    gpio_put(pin, 1);
  }
  else {
    if (mode == INPUT_PULLUP) {
      gpio_set_dir(pin, GPIO_IN);
      gpio_put(pin, 1);
      gpio_pull_up(pin);
    }
    else {
      gpio_set_dir(pin, GPIO_OUT);
      gpio_put(pin, 1);
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

#define RX_OFFSET 0
#define CS_OFFSET 1
#define SCK_OFFSET 2
#define TX_OFFSET 3

void init_spi(const uint8_t RX, const uint8_t SCK, const uint8_t TX, spi_inst_t* spi, const uint Hz)
{
  spi_init(spi, Hz);
  //    spi_set_format(spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);  - default
  //    spi_set_format(spi, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);  - problematic
  gpio_set_function(RX, GPIO_FUNC_SPI);    // MISO
  gpio_set_function(SCK, GPIO_FUNC_SPI);    // CLK
  gpio_set_function(TX, GPIO_FUNC_SPI);    // MOSI
  // Make the SPI pins available to picotool
  bi_decl(bi_3pins_with_func(RX, TX, SCK, GPIO_FUNC_SPI));
}

void init_spi(spi_section_t section, const uint Hz)
{
  int8_t n = section.section;
  if (n >= 0) {
    init_spi(n * 4 + RX_OFFSET, n * 4 + SCK_OFFSET, n * 4 + TX_OFFSET, n & 0x2 ? spi1 : spi0, Hz);
  }
}

void init_spi1(const uint8_t RX, const uint8_t SCK, const uint8_t TX, const uint Hz)
{
  init_spi(RX, SCK, TX, spi1, Hz);
}

// ---------------------------------- PicoSPI ---------------------------------

void PicoSPI::beginTransaction(const uint Hz)
{
  cs(1);  // Just in case it has been left low
  saved_baudrate = spi_get_baudrate(_spi);
  spi_set_baudrate(_spi, Hz);
  cs(0);
}

void PicoSPI::endTransaction()
{
  cs(1);
  spi_set_baudrate(_spi, saved_baudrate);  // restore speed
}


// ----------------------------------- I2C ------------------------------------

#define SDA_OFFSET 0
#define SCL_OFFSET 1

void init_i2c(const uint8_t SDA, const uint8_t SCL, i2c_inst_t* i2c, const uint Hz)
{
  // I2C is "open drain", pull ups to keep signal high when no data is being sent
  i2c_init(i2c, Hz);
  gpio_set_function(SDA, GPIO_FUNC_I2C);
  gpio_set_function(SCL, GPIO_FUNC_I2C);
  gpio_pull_up(SDA);
  gpio_pull_up(SCL);
}

void init_i2c0(i2c_section_t section, const uint Hz)
{
  int8_t n = section.section;
  if (n >= 0) {
    init_i2c(n * 4 + SDA_OFFSET, n * 4 + SCL_OFFSET, i2c0, Hz);
  }
}

void init_i2c0(const uint8_t SDA, const uint8_t SCL, const uint Hz)
{
  init_i2c(SDA, SCL, i2c0, Hz);
}

void init_i2c1(i2c_section_t section, const uint Hz)
{
  int8_t n = section.section;
  if (n >= 0) {
    init_i2c(n * 4 + 2 + SDA_OFFSET, n * 4 + 2 + SCL_OFFSET, i2c1, Hz);
  }
}

void init_i2c1(const uint8_t SDA, const uint8_t SCL, const uint Hz)
{
  init_i2c(SDA, SCL, i2c1, Hz);
}

// --------------------------------- PicoI2C ----------------------------------

void PicoI2C::read(uint8_t* dst, const int16_t len, const bool nostop)
{
  i2c_read_blocking(_i2c, _addr, dst, len, nostop);
}

const uint8_t PicoI2C::read8(const bool nostop)
{
  uint8_t b;
  i2c_read_blocking(_i2c, _addr, &b, 1, nostop);
  return b;    // endian
}

const uint16_t PicoI2C::read16(const bool nostop)
{
  uint8_t b[2];
  i2c_read_blocking(_i2c, _addr, b, 2, nostop);
  return (b[1] << 8) | b[0];    // endian
}

void PicoI2C::write(const uint8_t* src, const int16_t len, const bool nostop)
{
  i2c_write_blocking(_i2c, _addr, src, len, nostop);
}

void PicoI2C::write8(const uint8_t endian, const bool nostop)
{
  i2c_write_blocking(_i2c, _addr, &endian, 1, nostop);
}

void PicoI2C::write8write8(const uint8_t reg, const uint8_t value, const bool nostop)
{
  write16(value << 8 | reg, nostop);
}

void PicoI2C::write16(const uint16_t endian, const bool nostop)
{
  i2c_write_blocking(_i2c, _addr, (uint8_t*)&endian, 2, nostop);
}

void PicoI2C::write8read(const uint8_t reg, uint8_t* dst, const int16_t len)
{
  write8(reg, true);
  read(dst, len);
}

const uint8_t PicoI2C::write8read8(const uint8_t reg)
{
  write8(reg, true);
  return read8();
}

