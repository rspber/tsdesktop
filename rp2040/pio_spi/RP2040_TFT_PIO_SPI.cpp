/*
  RP2040 TFT PIO SPI

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

  Based on

  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_RP2040.c

  Bodmer's license.txt in file /licenses/Bodmer_license.txt
*/

#include <Setup.h>

#if defined(TFT_PIO_SPI_WRITE)

  #include "RP2040_TFT_PIO_SPI.h"
  #include "pio_spi.pio.h"
  #include "pio_spi_mdt_block_16.pio.h"
  #include "pio_spi_mdt_block_24.pio.h"
  #include "hardware/clocks.h"
  #include <api/Common.h>

  #define TFT_CASET       0x2A    // Column address set
  #define TFT_PASET       0x2B    // Page address set
  #define TFT_RAMWR       0x2C    // Memory write
  #define TFT_RAMRD       0x2E    // Memory read
  #define TFT_IDXRD       0xD9    // undocumented

// global variables

  p_pio_t pio_spi_0;

  uint8_t pio_spi_curr_clk_div = 0;

void p_pio_t::initPins()
{
  pio_gpio_init(pio, TFT_SPI_DC);
  pio_gpio_init(pio, TFT_SPI_CLK);
  pio_gpio_init(pio, TFT_SPI_MOSI);
#if defined(TFT_PIO_SPI_READ)
  pio_gpio_init(pio, TFT_SPI_MISO);
#endif
  pio_sm_set_pindir(*pio_sm, TFT_SPI_DC, true);
  pio_sm_set_pindir(*pio_sm, TFT_SPI_CLK, true);
  pio_sm_set_pindir(*pio_sm, TFT_SPI_MOSI, true);
#if defined(TFT_PIO_SPI_READ)
  pio_sm_set_pindir(*pio_sm, TFT_SPI_MISO, false);
#endif
}

void p_pio_t::deinitPins()
{
  gpio_deinit(TFT_SPI_DC);
  gpio_deinit(TFT_SPI_CLK);
  gpio_deinit(TFT_SPI_MOSI);
#if defined(TFT_PIO_SPI_READ)
  gpio_deinit(TFT_SPI_MISO);
#endif
}

void p_pio_t::enable()
{
  initPins();
  PIO_SM_ENABLE(pio, sm);
}

void p_pio_t::disable()
{
  PIO_SM_DISABLE(pio, sm);
//  deinitPins();
}

void p_pio_t::load_program()
{
  // Using the fact that it is our processor, we give our program the highest priority,
  // we don't need to check if another program is running on our PIO now.
  pio = pio1; // available are 0 .. 1
  sm = 3;     // available are 0 .. 3
  pio_sm = &pio->sm[sm];
  PIO_SM_DISABLE(pio, sm);  // The state machine was successfully allocated.
  pio_load_program(pio, pio_spi_program.instructions, 0, pio_spi_program.length);
  // The following lines must correspond to those in the commented config.
  pio_sm_set_wrap(*pio_sm, pio_spi_wrap_target, pio_spi_wrap);
  pio_sm_set_sideset(*pio_sm, 2, true, false);
}

void p_pio_t::setup_pio()
{
  pio_sm_set_set_pins(*pio_sm, TFT_SPI_DC, 1);
  pio_sm_set_sideset_pins(*pio_sm, TFT_SPI_CLK);
  pio_sm_set_out_pins(*pio_sm, TFT_SPI_MOSI, 1);
#if defined(TFT_PIO_SPI_READ)
  pio_sm_set_in_pins(*pio_sm, TFT_SPI_MISO);
#endif
  use_fifo_for_writing(*pio_sm);
  pio_sm_set_out_shift(*pio_sm, false, false, 0);
#if defined(TFT_PIO_SPI_READ)
  pio_sm_set_in_shift(*pio_sm, false, true, 8);
#endif
  SM_STALL_MASK = 1u << (PIO_FDEBUG_TXSTALL_LSB + sm);
  instr_clr_dc = pio_encode_set((pio_src_dest)0, 0);
  instr_set_dc = pio_encode_set((pio_src_dest)0, 1);
}

void p_pio_t::init()
{
  load_program();
  setup_pio();
  instr_jmp_send8  = pio_encode_jmp(pio_spi_offset_send8);
  instr_jmp_send16 = pio_encode_jmp(pio_spi_offset_send16);
  instr_jmp_send24 = pio_encode_jmp(pio_spi_offset_send24);
#if defined(TFT_PIO_SPI_READ)
  instr_jmp_read8  = pio_encode_jmp(pio_spi_offset_read8);
#endif
  instr_jmp_chunk  = pio_encode_jmp(pio_spi_offset_chunk);
}

void p_pio_t::setClockDiv(const uint8_t clock_div)
{
  PIO_SM_DISABLE(pio, sm);
  pio_sm_set_clkdiv(*pio_sm, clock_div);
  PIO_SM_RESTART_CLKDIV(pio, sm);
  pio_sm_clear_fifos(*pio_sm);
  pio_sm_clr_fifo_debug_flags(pio, sm);
  PIO_SM_RESTART(pio, sm);
  PIO_START_SEND_8;
}

void rp2040_pio_spi_setClockDiv(const uint8_t clock_div)
{
  if (pio_spi_curr_clk_div != clock_div) {
    pio_spi_0.setClockDiv(clock_div);
    pio_spi_0.enable();
    pio_spi_curr_clk_div = clock_div;
  }
}

extern void initPin(const int16_t pin, PinMode mode);
extern void tft_hardReset(const int16_t RST);

void rp2040_pio_spi_initBus()
{
#ifdef TOUCH_SPI_CS
  initPin(TOUCH_SPI_CS, OUTPUT);
#endif
  initPin(TFT_SPI_CS, OUTPUT);
  initPin(TFT_SPI_DC, OUTPUT);
  initPin(TFT_SPI_RST, OUTPUT);
  tft_hardReset(TFT_SPI_RST);
}




// write

  const char* tft_identification() { return "RP2040 PIO SPI"; }

void rp2040_pio_spi_enable(bool state)
{
  if (state) {
    pio_spi_0.enable();
  }
  else {
    pio_spi_0.disable();
  }
}

void tft_write_begin()
{
  rp2040_pio_spi_initBus();
  pio_spi_0.init();
  pio_spi_curr_clk_div = 0;
}

void tft_startWriteCmd()
{
  rp2040_pio_spi_setClockDiv(TFT_PIO_SPI_WRITE_DIV);
  PIO_CS_L;
}

void tft_sendCmd(const uint8_t cmd)
{
  PIO_DC_C;
  PIO_CS_H;
  PIO_CS_L;
  PIO_START_SEND_8;
  PIO_SEND(cmd);
  PIO_DC_D;
}

void tft_sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t len)
{
  PIO_DC_C;
  PIO_CS_H;
  PIO_CS_L;
  PIO_START_SEND_8;
  PIO_SEND(cmd);
  PIO_DC_D;

  for (int i = 0; i < len; ++i) {
    PIO_SEND(data[i]);
  }
}

void tft_writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  PIO_ADDR_W;
  PIO_TX_FIFO(TFT_CASET);
  PIO_TX_FIFO( (x << 16) | (x + w  - 1));
  PIO_TX_FIFO(TFT_PASET);
  PIO_TX_FIFO( (y << 16) | (y + h - 1));
  PIO_TX_FIFO(TFT_RAMWR);
  PIO_WAIT_FOR_STALL;
/*
  PIO_DC_C;
  PIO_SEND_8(TFT_CASET);
  PIO_DC_D;
  PIO_SEND_16(x);
  PIO_SEND_16(x + w - 1);
  PIO_DC_C;
  PIO_SEND_8(TFT_PASET);
  PIO_DC_D;
  PIO_SEND_16(y);
  PIO_SEND_16(y + h - 1);
  PIO_DC_C;
  PIO_SEND_8(TFT_RAMWR);
  PIO_DC_D;
*/
}

void tft_sendMDTColor(const mdt_t c)
{
  #if defined(COLOR_565)
    PIO_START_SEND_16;
  #else
    PIO_START_SEND_24;
  #endif
  PIO_SEND(c);
}

void tft_sendMDTColor(const mdt_t c, int32_t len)
{
#if defined(COLOR_565)
  if (len >= 3) {
    p_pio_t& p = pio_spi_0;
    PIO_WAIT_FOR_STALL;
    PIO_SM_DISABLE(p.pio, p.sm);
    pio_load_program(p.pio, pio_spi_mdt_block_16_program.instructions, pio_spi_offset_chunk, pio_spi_mdt_block_16_program.length);
    PIO_START_CHUNK;
    PIO_SM_ENABLE(p.pio, p.sm);
    PIO_TX_FIFO(c);
    PIO_TX_FIFO(len-1); // Decrement first as PIO sends n+1
    PIO_WAIT_FOR_STALL;
    PIO_SM_DISABLE(p.pio, p.sm);
    pio_load_program(p.pio, pio_spi_program.instructions, pio_spi_offset_chunk, pio_spi_program.length);
    PIO_START_SEND_8;
    PIO_SM_ENABLE(p.pio, p.sm);
  }
  else {
    PIO_START_SEND_16;
    while (--len >= 0) {
      PIO_SEND(c);
    }
  }
#else
  if (len >= 3 ) {
    p_pio_t& p = pio_spi_0;
    PIO_WAIT_FOR_STALL;
    PIO_SM_DISABLE(p.pio, p.sm);
    pio_load_program(p.pio, pio_spi_mdt_block_24_program.instructions, pio_spi_offset_chunk, pio_spi_mdt_block_24_program.length);
    PIO_START_CHUNK;
    PIO_SM_ENABLE(p.pio, p.sm);
    PIO_TX_FIFO(c);
    PIO_TX_FIFO(len-1); // Decrement first as PIO sends n+1
    PIO_WAIT_FOR_STALL;
    PIO_SM_DISABLE(p.pio, p.sm);
    pio_load_program(p.pio, pio_spi_program.instructions, pio_spi_offset_chunk, pio_spi_program.length);
    PIO_START_SEND_8;
    PIO_SM_ENABLE(p.pio, p.sm);
  }
  else {
    PIO_START_SEND_24;
    while (--len >= 0) {
      PIO_SEND(c);
    }
  }
#endif
}

inline uint16_t fetch_16(const uint8_t *p)
{
  uint8_t b1 = *p++;
  uint8_t b0 = *p++;
  return ((uint16_t)b1 << 8) | b0;
}

void tft_sendMDTBuffer16(const uint8_t* p, int32_t len)
{
  PIO_START_SEND_16;
  while (len >= 5) {
    // 5 seems to be the optimum for maximum transfer rate
    PIO_WAIT_FOR_FIFO_FREE(5);
    PIO_TX_FIFO(fetch_16(p)); p += 2;
    PIO_TX_FIFO(fetch_16(p)); p += 2;
    PIO_TX_FIFO(fetch_16(p)); p += 2;
    PIO_TX_FIFO(fetch_16(p)); p += 2;
    PIO_TX_FIFO(fetch_16(p)); p += 2;
    len -= 5;
  }

  while (--len >= 0) {
    PIO_WAIT_FOR_FIFO_FREE(1);
    PIO_TX_FIFO(fetch_16(p)); p += 2;
  }
}

inline uint32_t fetch_24(const uint8_t* p)
{
  uint8_t r = *p++;
  uint8_t g = *p++;
  uint8_t b = *p++;
  return ((uint32_t)r << 16) | ((uint16_t)g << 8) | b;
}

void tft_sendMDTBuffer24(const uint8_t* p, int32_t len)
{
  PIO_START_SEND_24;
  while (len >= 5) {
    // 5 seems to be the optimum for maximum transfer rate
    PIO_WAIT_FOR_FIFO_FREE(5);
    PIO_TX_FIFO(fetch_24(p)); p += 3;
    PIO_TX_FIFO(fetch_24(p)); p += 3;
    PIO_TX_FIFO(fetch_24(p)); p += 3;
    PIO_TX_FIFO(fetch_24(p)); p += 3;
    PIO_TX_FIFO(fetch_24(p)); p += 3;
    len -= 5;
  }

  while (--len >= 0) {
    PIO_WAIT_FOR_FIFO_FREE(1);
    PIO_TX_FIFO(fetch_24(p)); p += 3;
  }
}




// read

#if defined(TFT_PIO_SPI_READ)

void tft_setBUSReadMode()
{
  use_fifo_for_reading(*pio_spi_0.pio_sm);
  pio_spi_0.START_READ_8();
}

void tft_setBUSWriteMode()
{
  use_fifo_for_writing(*pio_spi_0.pio_sm);
  pio_spi_0.START_SEND_8();
}

void tft_read_begin()
{
}

void tft_startReading()
{
  rp2040_pio_spi_setClockDiv(TFT_PIO_SPI_READ_DIV);
  PIO_CS_L;
}

void tft_endReading()
{
  tft_setBUSWriteMode();
  PIO_CS_H;
}

void tft_readAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  PIO_ADDR_W;
  PIO_TX_FIFO(TFT_CASET);
  PIO_TX_FIFO( (x << 16) | (x + w  - 1));
  PIO_TX_FIFO(TFT_PASET);
  PIO_TX_FIFO( (y << 16) | (y + h - 1));
  PIO_TX_FIFO(TFT_RAMRD);
  PIO_WAIT_FOR_STALL;
/*
  PIO_DC_C;
  PIO_SEND_8(TFT_CASET);
  PIO_DC_D;
  PIO_SEND_16(x);
  PIO_SEND_16(x + w - 1);
  PIO_DC_C;
  PIO_SEND_8(TFT_PASET);
  PIO_DC_D;
  PIO_SEND_16(y);
  PIO_SEND_16(y + h - 1);
  PIO_DC_C;
  PIO_SEND_8(TFT_RAMRD);
  PIO_DC_D;
*/
}

const uint8_t tft_transfer(const uint8_t cmd)
{
  while (PIO_RX_FIFO_IS_EMPTY);
  return PIO_RX_FIFO;
}

const uint16_t tft_transfer16(const uint8_t cmd)
{
  uint8_t b0 = tft_transfer(cmd);
  uint8_t b1 = tft_transfer(cmd);
  return (b0 << 8) | b1;
}

#endif

#endif
