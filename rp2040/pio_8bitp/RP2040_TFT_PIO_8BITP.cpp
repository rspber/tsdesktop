/*
  RP2040 TFT PIO 8BITP WRITE ONLY

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

  Based on

  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_RP2040.c

  Bodmer's license.txt in file /licenses/Bodmer_license.txt
*/

#include <Setup.h>

#if defined(TFT_PIO_8BITP_WRITE)

  #include "RP2040_TFT_PIO_8BITP.h"
  #include "pio_8bitp.pio.h"
  #include "pio_8bitp_mdt_block_16.pio.h"
  #include "pio_8bitp_mdt_block_24.pio.h"
  #include "hardware/clocks.h"
  #include <api/Common.h>

  #define TFT_CASET       0x2A    // Column address set
  #define TFT_PASET       0x2B    // Page address set
  #define TFT_RAMWR       0x2C    // Memory write
  #define TFT_RAMRD       0x2E    // Memory read
  #define TFT_IDXRD       0xD9    // undocumented

// global variables

  p_pio_t pio_8bitp_0;

  bool pio_8bitp_freq_set = false;

void p_pio_t::initPins()
{
  pio_gpio_init(pio, TFT_8BITP_DC);
  pio_gpio_init(pio, TFT_8BITP_WR);
  for (int i = 0; i < 8; i++) {
    pio_gpio_init(pio, TFT_8BITP_D0 + i);
  }

  pio_sm_set_pindir(*pio_sm, TFT_8BITP_DC, true);
  pio_sm_set_pindir(*pio_sm, TFT_8BITP_WR, true);
  pio_sm_set_consecutive_pindirs(*pio_sm, TFT_8BITP_D0, 8, true);
}

void p_pio_t::deinitPins()
{
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
  pio = pio0; // available are 0 .. 1
  sm = 2;     // available are 0 .. 3
  pio_sm = &pio->sm[sm];
  PIO_SM_DISABLE(pio, sm);  // The state machine was successfully allocated.
  pio_load_program(pio, pio_8bitp_program.instructions, 0, pio_8bitp_program.length);
  // The following lines must correspond to those in the commented config.
  pio_sm_set_wrap(*pio_sm, pio_8bitp_wrap_target, pio_8bitp_wrap);
  pio_sm_set_sideset(*pio_sm, 2, true, false);
}

void p_pio_t::setup_pio()
{
  pio_sm_set_set_pins(*pio_sm, TFT_8BITP_DC, 1);
  pio_sm_set_sideset_pins(*pio_sm, TFT_8BITP_WR);
  pio_sm_set_out_pins(*pio_sm, TFT_8BITP_D0, 8);
  use_fifo_for_writing(*pio_sm);
  pio_sm_set_out_shift(*pio_sm, false, false, 0);
  SM_STALL_MASK = 1u << (PIO_FDEBUG_TXSTALL_LSB + sm);
  instr_clr_dc = pio_encode_set((pio_src_dest)0, 0);
  instr_set_dc = pio_encode_set((pio_src_dest)0, 1);
}

void p_pio_t::init()
{
  load_program();
  setup_pio();
  instr_jmp_send8  = pio_encode_jmp(pio_8bitp_offset_start_tx_8);
  instr_jmp_send16 = pio_encode_jmp(pio_8bitp_offset_start_tx_16);
  instr_jmp_send24 = pio_encode_jmp(pio_8bitp_offset_start_tx_24);
  instr_jmp_chunk  = pio_encode_jmp(pio_8bitp_offset_chunk);
}

void p_pio_t::setClockDiv(const uint16_t DIV_UNITS, const uint16_t DIV_FRACT)
{
  PIO_SM_DISABLE(pio, sm);
  pio_sm_set_clkdiv_int_frac(*pio_sm, DIV_UNITS, DIV_FRACT);
  pio_sm_clear_fifos(*pio_sm);
  pio_sm_clr_fifo_debug_flags(pio, sm);
  PIO_SM_RESTART(pio, sm);
  PIO_SM_RESTART_CLKDIV(pio, sm);
  PIO_START_SEND_8;
}

void rp2040_pio_8bitp_setFreq()
{
  // Different controllers have different minimum write cycle periods, so the PIO clock is changed accordingly
  // The PIO clock is a division of the CPU clock so scales when the processor is overclocked
  // PIO write frequency = (CPU clock/(4 * RP2040_PIO_CLK_DIV))
  // The write cycle periods below assume a 125MHz CPU clock speed

  #if defined (RP2040_PIO_CLK_DIV)
    #if (RP2040_PIO_CLK_DIV > 0)
      #define DIV_UNITS RP2040_PIO_CLK_DIV
      #define DIV_FRACT 0
    #else
      #define DIV_UNITS 3
      #define DIV_FRACT 0
    #endif
  #else // 8 bit parallel mode default 64ns write cycle time
    #define DIV_UNITS 2
    #define DIV_FRACT 0 // Note: Fractional values done with clock period dithering
  #endif

  if (!pio_8bitp_freq_set) {
    pio_8bitp_0.setClockDiv(DIV_UNITS, DIV_FRACT);
    pio_8bitp_0.enable();
    pio_8bitp_freq_set = true;
  }
}

extern void initPin(const int16_t pin, PinMode mode);
extern void tft_hardReset(const int16_t RST);

void rp2040_pio_8bitp_initBus()
{
#ifdef TFT_8BITP_CS
  initPin(TFT_8BITP_CS, OUTPUT);
#endif
  initPin(TFT_8BITP_DC, OUTPUT);
  initPin(TFT_8BITP_WR, OUTPUT);
#ifdef TFT_8BITP_RD
  initPin(TFT_8BITP_RD, OUTPUT);
#endif
#ifdef TFT_8BITP_RST
  initPin(TFT_8BITP_RST, OUTPUT);
  tft_hardReset(TFT_8BITP_RST);
#endif
}




// write

  const char* tft_identification() { return "RP2040 PIO 8BITP"; }

void rp2040_pio_8bitp_enable(bool state)
{
  if (state) {
    pio_8bitp_0.enable();
  }
  else {
    pio_8bitp_0.disable();
  }
}

void tft_write_begin()
{
  rp2040_pio_8bitp_initBus();
  pio_8bitp_0.init();
  pio_8bitp_freq_set = false;
}

void tft_startWriteCmd()
{
  rp2040_pio_8bitp_setFreq();
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
  if (len >= 21) {
    p_pio_t& p = pio_8bitp_0;
    PIO_WAIT_FOR_STALL;
    PIO_SM_DISABLE(p.pio, p.sm);
    pio_load_program(p.pio, pio_8bitp_mdt_block_16_program.instructions, pio_8bitp_offset_chunk, pio_8bitp_mdt_block_16_program.length);
    PIO_START_CHUNK;
    PIO_SM_ENABLE(p.pio, p.sm);
    PIO_TX_FIFO(c);
    PIO_TX_FIFO(len-1); // Decrement first as PIO sends n+1
    PIO_WAIT_FOR_STALL;
    PIO_SM_DISABLE(p.pio, p.sm);
    pio_load_program(p.pio, pio_8bitp_program.instructions, pio_8bitp_offset_chunk, pio_8bitp_program.length);
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
  if (len >= 21 ) {
    p_pio_t& p = pio_8bitp_0;
    PIO_WAIT_FOR_STALL;
    PIO_SM_DISABLE(p.pio, p.sm);
    pio_load_program(p.pio, pio_8bitp_mdt_block_24_program.instructions, pio_8bitp_offset_chunk, pio_8bitp_mdt_block_24_program.length);
    PIO_START_CHUNK;
    PIO_SM_ENABLE(p.pio, p.sm);
    PIO_TX_FIFO(c);
    PIO_TX_FIFO(len-1); // Decrement first as PIO sends n+1
    PIO_WAIT_FOR_STALL;
    PIO_SM_DISABLE(p.pio, p.sm);
    pio_load_program(p.pio, pio_8bitp_program.instructions, pio_8bitp_offset_chunk, pio_8bitp_program.length);
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


#endif
