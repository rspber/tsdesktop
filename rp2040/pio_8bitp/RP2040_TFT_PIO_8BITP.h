/*
  RP2040 TFT PIO 8BITP WRITE ONLY

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

  Based on

  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_RP2040.h

  Bodmer's license.txt in file /licenses/Bodmer_license.txt
*/

#pragma once

#if defined(TFT_PIO_8BITP_WRITE)

  #include "rp2040_pio.h"
  #include <rgb.h>

  typedef struct
  {
    PIO pio;
    int8_t sm;
    pio_sm_hw_t* pio_sm;

    void initPins();
    void deinitPins();
    void enable();
    void disable();
    void setup_pio();
    void load_program();
    void init();
    void setClockDiv(const uint16_t DIV_UNITS, const uint16_t DIV_FRACT);

    uint32_t SM_STALL_MASK;
    inline void WAIT_FOR_STALL() { pio->fdebug = SM_STALL_MASK; while (!(pio->fdebug & SM_STALL_MASK)); }

    // SM "set" instructions to control DC control signal
    uint32_t instr_clr_dc;
    inline void DC_C() { WAIT_FOR_STALL(); pio_sm->instr = instr_clr_dc; }

    uint32_t instr_set_dc;
    inline void DC_D() { WAIT_FOR_STALL(); pio_sm->instr = instr_set_dc; }

    uint32_t instr_jmp_send8;
    inline void START_SEND_8() { pio_sm->instr = instr_jmp_send8; }

    uint32_t instr_jmp_send16;
    inline void START_SEND_16() { WAIT_FOR_STALL(); pio_sm->instr = instr_jmp_send16; }

    uint32_t instr_jmp_send24;
    inline void START_SEND_24() { WAIT_FOR_STALL(); pio_sm->instr = instr_jmp_send24; }

    uint32_t instr_jmp_chunk;
    inline void START_CHUNK() { pio_sm->instr = instr_jmp_chunk; }

  } p_pio_t;

  extern p_pio_t pio_8bitp_0;

  #define PIO_WAIT_FOR_STALL pio_8bitp_0.WAIT_FOR_STALL()

  #if defined(TFT_8BITP_CS) && (TFT_8BITP_CS >= 0)
    #define PIO_CS_L sio_hw->gpio_clr = (1ul << TFT_8BITP_CS)
    #define PIO_CS_H PIO_WAIT_FOR_STALL; sio_hw->gpio_set = (1ul << TFT_8BITP_CS)
  #else
    #define PIO_CS_L
    #define PIO_CS_H
  #endif

  #define PIO_DC_C pio_8bitp_0.DC_C()
  #define PIO_DC_D pio_8bitp_0.DC_D()

  #define PIO_TX_FIFO(d)   TX_FIFO(pio_8bitp_0.pio, pio_8bitp_0.sm, d)

  #define PIO_ADDR_W   PIO_WAIT_FOR_STALL; pio_8bitp_0.START_CHUNK()

  #define PIO_START_SEND_8  pio_8bitp_0.START_SEND_8()
  #define PIO_START_SEND_16 pio_8bitp_0.START_SEND_16()
  #define PIO_START_SEND_24 pio_8bitp_0.START_SEND_24()
  #define PIO_START_CHUNK   pio_8bitp_0.START_CHUNK()

  #define PIO_WAIT_FOR_FIFO_FREE(S) WAIT_FOR_FIFO_FREE(pio_8bitp_0.pio, pio_8bitp_0.sm, S)
//  #define PIO_WAIT_FOR_FIFO_5_FREE WAIT_FOR_FIFO_5_FREE(pio_8bitp_0.pio, pio_8bitp_0.sm)
//  #define PIO_WAIT_FOR_FIFO_1_FREE WAIT_FOR_FIFO_1_FREE(pio_8bitp_0.pio, pio_8bitp_0.sm)
//  #define PIO_WAIT_FOR_FIFO_EMPTY  WAIT_FOR_FIFO_EMPTY(pio_8bitp_0.pio, pio_8bitp_0.sm)

  #define PIO_SEND(d) PIO_WAIT_FOR_FIFO_FREE(1); PIO_TX_FIFO(d)


// write

  void tft_write_begin();

  const char* tft_identification();

  void tft_startWriteCmd();
  void tft_sendCmd(const uint8_t cmd);
  void tft_sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t len);

  void rp2040_pio_8bitp_setFreq();

  inline void tft_startWrite()
  {
    rp2040_pio_8bitp_setFreq();
    PIO_CS_L;
  }

  inline void tft_endWrite()
  {
    PIO_CS_H;
  }

  void tft_writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h);

  void tft_sendMDTColor(const mdt_t c);
  void tft_sendMDTColor(const mdt_t c, int32_t len);
  void tft_sendMDTBuffer16(const uint8_t* buffer, int32_t len);
  void tft_sendMDTBuffer24(const uint8_t* buffer, int32_t len);

#ifndef TFT_GPIO_8BITP_READ

  #define tft_setBUSWriteMode()
  #define tft_setBUSReadMode()

#endif

#endif
