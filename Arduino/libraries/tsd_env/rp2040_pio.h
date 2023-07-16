/*
  hardware/pio.h simplified version: no config, claims, interrupts handling

  Copyright (c) 2023, rspber (https://github.com/rspber)

  originally notes below:
*/

/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "pico.h"
#include "hardware/address_mapped.h"
#include "hardware/structs/pio.h"
#include "hardware/gpio.h"
#include "hardware/regs/dreq.h"
#include "hardware/pio_instructions.h"

/** \file rp2040_pio.h
 *  \defgroup hardware_pio hardware_pio
 *
 * Programmable I/O (PIO) API
 *
 * A programmable input/output block (PIO) is a versatile hardware interface which
 * can support a number of different IO standards. There are two PIO blocks in the RP2040.
 *
 * Each PIO is programmable in the same sense as a processor: the four state machines independently
 * execute short, sequential programs, to manipulate GPIOs and transfer data. Unlike a general
 * purpose processor, PIO state machines are highly specialised for IO, with a focus on determinism,
 * precise timing, and close integration with fixed-function hardware. Each state machine is equipped
 * with:
 *  * Two 32-bit shift registers – either direction, any shift count
 *  * Two 32-bit scratch registers
 *  * 4×32 bit bus FIFO in each direction (TX/RX), reconfigurable as 8×32 in a single direction
 *  * Fractional clock divider (16 integer, 8 fractional bits)
 *  * Flexible GPIO mapping
 *  * DMA interface, sustained throughput up to 1 word per clock from system DMA
 *  * IRQ flag set/clear/status
 *
 * Full details of the PIO can be found in the RP2040 datasheet.
 */

#ifdef __cplusplus
extern "C" {
#endif

  #define PIO_SM_ENABLE(pio, sm)  pio->ctrl |= (1u << sm)
  #define PIO_SM_DISABLE(pio, sm) pio->ctrl &= ~(1u << sm)

  #define PIO_SM_RESTART(pio, sm)        pio->ctrl |= 1u << (PIO_CTRL_SM_RESTART_LSB + sm)
  #define PIO_SM_RESTART_CLKDIV(pio, sm) pio->ctrl |= 1u << (PIO_CTRL_CLKDIV_RESTART_LSB + sm)

  #define TX_FIFO(pio, sm, d) pio->txf[sm] = d

  // Wait until at least "S" locations free
  #define WAIT_FOR_FIFO_FREE(pio, sm, S) { while (((pio->flevel >> (sm * 8)) & 0x000F) > (uint8_t)(8-S)); }

  // Wait until at least 5 locations free
  #define WAIT_FOR_FIFO_5_FREE(pio, sm) { while ((pio->flevel) & (0x000c << (sm * 8))); }

  // Wait until at least 1 location free
  #define WAIT_FOR_FIFO_1_FREE(pio, sm) { while ((pio->flevel) & (0x0008 << (sm * 8))); }

  #define WAIT_FOR_FIFO_EMPTY(pio, sm) { while(!(pio->fstat & (1u << (PIO_FSTAT_TXEMPTY_LSB + sm)))); }

// read
  #define RX_FIFO(pio, sm) pio->rxf[sm]
  #define RX_FIFO_IS_EMPTY(pio, sm) ((pio->fstat & (1u << (PIO_FSTAT_RXEMPTY_LSB + sm))) != 0)
  #define EMPTY_RX_FIFO(pio, sm) { while (!RX_FIFO_IS_EMPTY(pio, sm)) (void) RX_FIFO(pio, sm); }


/** \brief FIFO join states
 *  \ingroup hardware_pio
 */
enum pio_fifo_join {
    PIO_FIFO_JOIN_NONE = 0,
    PIO_FIFO_JOIN_TX = 1,
    PIO_FIFO_JOIN_RX = 2,
};

typedef pio_hw_t *PIO;

/** Identifier for the first (PIO 0) hardware PIO instance (for use in PIO functions).
 *
 * e.g. pio_gpio_init(pio0, 5)
 *
 *  \ingroup hardware_pio
 */
#define pio0 pio0_hw

/** Identifier for the second (PIO 1) hardware PIO instance (for use in PIO functions).
 *
 * e.g. pio_gpio_init(pio1, 5)
 *
 *  \ingroup hardware_pio
 */
#define pio1 pio1_hw

inline void pio_sm_clr_fifo_debug_flags(PIO pio, uint sm)
{
  const uint32_t fdebug_sm_mask =
    (1u << PIO_FDEBUG_TXOVER_LSB) |
    (1u << PIO_FDEBUG_RXUNDER_LSB) |
    (1u << PIO_FDEBUG_TXSTALL_LSB) |
    (1u << PIO_FDEBUG_RXSTALL_LSB);
  pio->fdebug = fdebug_sm_mask << sm;
}

/*! \brief Set the current 'out' pins for a state machine
 *  \ingroup hardware_pio
 *
 * Can overlap with the 'in', 'set' and 'sideset' pins
 *
 * \param sm pio_sm_hw_t
 * \param out_base 0-31 First pin to set as output
 * \param out_count 0-32 Number of pins to set.
 */
inline void pio_sm_set_out_pins(pio_sm_hw_t& sm, uint out_base, uint out_count)
{
  sm.pinctrl = (sm.pinctrl & ~(PIO_SM0_PINCTRL_OUT_BASE_BITS | PIO_SM0_PINCTRL_OUT_COUNT_BITS)) |
    (out_base << PIO_SM0_PINCTRL_OUT_BASE_LSB) |
    (out_count << PIO_SM0_PINCTRL_OUT_COUNT_LSB);
}

/*! \brief Set the current 'set' pins for a state machine
 *  \ingroup hardware_pio
 *
 * Can overlap with the 'in', 'out' and 'sideset' pins
 *
 * \param sm pio_sm_hw_t
 * \param set_base 0-31 First pin to set as
 * \param set_count 0-5 Number of pins to set.
 */
inline void pio_sm_set_set_pins(pio_sm_hw_t& sm, uint set_base, uint set_count)
{
  sm.pinctrl = (sm.pinctrl & ~(PIO_SM0_PINCTRL_SET_BASE_BITS | PIO_SM0_PINCTRL_SET_COUNT_BITS)) |
    (set_base << PIO_SM0_PINCTRL_SET_BASE_LSB) |
    (set_count << PIO_SM0_PINCTRL_SET_COUNT_LSB);
}

/*! \brief Set the current 'in' pins for a state machine
 *  \ingroup hardware_pio
 *
 * Can overlap with the 'out', 'set' and 'sideset' pins
 *
 * \param sm pio_sm_hw_t
 * \param in_base 0-31 First pin to use as input
 */
inline void pio_sm_set_in_pins(pio_sm_hw_t& sm, uint in_base)
{
  sm.pinctrl = (sm.pinctrl & ~PIO_SM0_PINCTRL_IN_BASE_BITS) |
    (in_base << PIO_SM0_PINCTRL_IN_BASE_LSB);
}

/*! \brief Set the current 'sideset' pins for a state machine
 *  \ingroup hardware_pio
 *
 * Can overlap with the 'in', 'out' and 'set' pins
 *
 * \param sm pio_sm_hw_t
 * \param sideset_base 0-31 base pin for 'side set'
 */
inline void pio_sm_set_sideset_pins(pio_sm_hw_t& sm, uint sideset_base)
{
  sm.pinctrl = (sm.pinctrl & ~PIO_SM0_PINCTRL_SIDESET_BASE_BITS) |
    (sideset_base << PIO_SM0_PINCTRL_SIDESET_BASE_LSB);
}

/* \brief Set the 'sideset' options
 *  \ingroup hardware_pio
 *
 * \param sm pio_sm_hw_t
 * \param bit_count Number of bits to steal from delay field in the instruction for use of side set (max 5)
 * \param optional True if the topmost side set bit is used as a flag for whether to apply side set on that instruction
 * \param pindirs True if the side set affects pin directions rather than values
 */
inline void pio_sm_set_sideset(pio_sm_hw_t& sm, uint bit_count, bool optional, bool pindirs)
{
  sm.pinctrl = (sm.pinctrl & ~(PIO_SM0_PINCTRL_SIDESET_COUNT_BITS)) |
    (bit_count << PIO_SM0_PINCTRL_SIDESET_COUNT_LSB);

  sm.execctrl = (sm.execctrl & ~(PIO_SM0_EXECCTRL_SIDE_EN_BITS | PIO_SM0_EXECCTRL_SIDE_PINDIR_BITS)) |
    (bool_to_bit(optional) << PIO_SM0_EXECCTRL_SIDE_EN_LSB) |
    (bool_to_bit(pindirs) << PIO_SM0_EXECCTRL_SIDE_PINDIR_LSB);
}

/*! \brief Set the state machine clock divider (from integer and fractional parts - 16:8)
 *  \ingroup hardware_pio
 *
 * The clock divider can slow the state machine's execution to some rate below
 * the system clock frequency, by enabling the state machine on some cycles
 * but not on others, in a regular pattern. This can be used to generate e.g.
 * a given UART baud rate. See the datasheet for further detail.
 *
 * \param sm pio_sm_hw_t
 * \param div_int Integer part of the divisor
 * \param div_frac Fractional part in 1/256ths
 * \sa sm_config_set_clkdiv()
 */
inline void pio_sm_set_clkdiv_int_frac(pio_sm_hw_t& sm, uint16_t div_int, uint8_t div_frac)
{
  sm.clkdiv = (div_int << PIO_SM0_CLKDIV_INT_LSB) | (div_frac << PIO_SM0_CLKDIV_FRAC_LSB);
}

static inline void pio_calculate_clkdiv_from_float(float div, uint16_t *div_int, uint8_t *div_frac) {
    *div_int = (uint16_t)div;
    if (*div_int == 0) {
        *div_frac = 0;
    } else {
        *div_frac = (uint8_t)((div - (float)*div_int) * (1u << 8u));
    }
}

/*! \brief Set the state machine clock divider (from a floating point value)
 * \param sm pio_sm_hw_t
 *
 * The clock divider slows the state machine's execution by masking the
 * system clock on some cycles, in a repeating pattern, so that the state
 * machine does not advance. Effectively this produces a slower clock for the
 * state machine to run from, which can be used to generate e.g. a particular
 * UART baud rate. See the datasheet for further detail.
 *
 * \param sm pio_sm_hw_t
 * \param div The fractional divisor to be set. 1 for full speed. An integer clock divisor of n
 *  will cause the state machine to run 1 cycle in every n.
 *  Note that for small n, the jitter introduced by a fractional divider (e.g. 2.5) may be unacceptable
 *  although it will depend on the use case.
 */
inline void pio_sm_set_clkdiv(pio_sm_hw_t& sm, const uint16_t clock_div)
{
    uint16_t div_int;
    uint8_t div_frac;
    pio_calculate_clkdiv_from_float(clock_div, &div_int, &div_frac);
    pio_sm_set_clkdiv_int_frac(sm, div_int, div_frac);
}

/*! \brief Set the wrap addresses in a state machine
 *  \ingroup hardware_pio
 *
 * \param sm pio_sm_hw_t
 * \param wrap_target the instruction memory address to wrap to
 * \param wrap        the instruction memory address after which to set the program counter to wrap_target
 *                    if the instruction does not itself update the program_counter
 */
inline void pio_sm_set_wrap(pio_sm_hw_t& sm, uint wrap_target, uint wrap)
{
  sm.execctrl = (sm.execctrl & ~(PIO_SM0_EXECCTRL_WRAP_TOP_BITS | PIO_SM0_EXECCTRL_WRAP_BOTTOM_BITS)) |
    (wrap_target << PIO_SM0_EXECCTRL_WRAP_BOTTOM_LSB) |
    (wrap << PIO_SM0_EXECCTRL_WRAP_TOP_LSB);
}

/*! \brief Setup 'in' shifting parameters in a state machine
 *  \ingroup hardware_pio
 *
 * \param sm pio_sm_hw_t
 * \param shift_right true to shift ISR to right, false to shift ISR to left
 * \param autopush whether autopush is enabled
 * \param push_threshold threshold in bits to shift in before auto/conditional re-pushing of the ISR
 */
inline void pio_sm_set_in_shift(pio_sm_hw_t& sm, bool shift_right, bool autopush, uint push_threshold)
{
  sm.shiftctrl = (sm.shiftctrl & ~(PIO_SM0_SHIFTCTRL_IN_SHIFTDIR_BITS | PIO_SM0_SHIFTCTRL_AUTOPUSH_BITS | PIO_SM0_SHIFTCTRL_PUSH_THRESH_BITS)) |
    (bool_to_bit(shift_right) << PIO_SM0_SHIFTCTRL_IN_SHIFTDIR_LSB) |
    (bool_to_bit(autopush) << PIO_SM0_SHIFTCTRL_AUTOPUSH_LSB) |
    ((push_threshold & 0x1fu) << PIO_SM0_SHIFTCTRL_PUSH_THRESH_LSB);
}

/*! \brief Setup 'out' shifting parameters in a state machine
 *  \ingroup hardware_pio
 *
 * \param sm pio_sm_hw_t
 * \param shift_right true to shift OSR to right, false to shift OSR to left
 * \param autopull whether autopull is enabled
 * \param pull_threshold threshold in bits to shift out before auto/conditional re-pulling of the OSR
 */
inline void pio_sm_set_out_shift(pio_sm_hw_t& sm, bool shift_right, bool autopull, uint pull_threshold)
{
  sm.shiftctrl = (sm.shiftctrl & ~(PIO_SM0_SHIFTCTRL_OUT_SHIFTDIR_BITS | PIO_SM0_SHIFTCTRL_AUTOPULL_BITS | PIO_SM0_SHIFTCTRL_PULL_THRESH_BITS)) |
    (bool_to_bit(shift_right) << PIO_SM0_SHIFTCTRL_OUT_SHIFTDIR_LSB) |
    (bool_to_bit(autopull) << PIO_SM0_SHIFTCTRL_AUTOPULL_LSB) |
    ((pull_threshold & 0x1fu) << PIO_SM0_SHIFTCTRL_PULL_THRESH_LSB);
}

/*! \brief Setup the FIFO joining in a state machine configuration
 *  \ingroup hardware_pio
 *
 * \param sm pio_sm_hw_t
 * \param join Specifies the join type. \see enum pio_fifo_join
 */
inline void pio_sm_set_fifo_join(pio_sm_hw_t& sm, enum pio_fifo_join join)
{
  sm.shiftctrl = (sm.shiftctrl & ~(PIO_SM0_SHIFTCTRL_FJOIN_TX_BITS | PIO_SM0_SHIFTCTRL_FJOIN_RX_BITS)) |
    (join << PIO_SM0_SHIFTCTRL_FJOIN_TX_LSB);
}

inline void use_fifo_for_writing(pio_sm_hw_t& sm)
{
  pio_sm_set_fifo_join(sm, PIO_FIFO_JOIN_TX);
}

inline void use_fifo_for_reading(pio_sm_hw_t& sm)
{
  pio_sm_set_fifo_join(sm, PIO_FIFO_JOIN_RX);
}

/*! \brief Setup the function select for a GPIO to use output from the given PIO instance
 *  \ingroup hardware_pio
 *
 * PIO appears as an alternate function in the GPIO muxing, just like an SPI
 * or UART. This function configures that multiplexing to connect a given PIO
 * instance to a GPIO. Note that this is not necessary for a state machine to
 * be able to read the *input* value from a GPIO, but only for it to set the
 * output value or output enable.
 *
 * \param pio The PIO instance; either \ref pio0 or \ref pio1
 * \param pin the GPIO pin whose function select to set
 */
inline void pio_gpio_init(PIO pio, uint pin) {
  gpio_set_function(pin, pio == pio0 ? GPIO_FUNC_PIO0 : GPIO_FUNC_PIO1);
}

typedef struct pio_program {
    const uint16_t *instructions;
    uint8_t length;
    int8_t origin; // required instruction memory origin or -1
} __packed pio_program_t;

inline void pio_load_program(PIO pio, const uint16_t instructions[], uint from, uint to)
{
  for (uint i = from; i < to; ++i) {
    pio->instr_mem[i] = instructions[i];
  }
}

//static inline uint8_t pio_sm_get_pc(PIO pio, uint sm);
//inline static void pio_sm_exec(PIO pio, uint sm, uint instr)
//static inline bool pio_sm_is_exec_stalled(PIO pio, uint sm);
//static inline void pio_sm_exec_wait_blocking(PIO pio, uint sm, uint instr);

//static inline void pio_sm_put(PIO pio, uint sm, uint32_t data);
//static inline uint32_t pio_sm_get(PIO pio, uint sm);

//static inline bool pio_sm_is_rx_fifo_full(PIO pio, uint sm);
//static inline bool pio_sm_is_rx_fifo_empty(PIO pio, uint sm)
//static inline uint pio_sm_get_rx_fifo_level(PIO pio, uint sm);

//static inline bool pio_sm_is_tx_fifo_full(PIO pio, uint sm);
//static inline bool pio_sm_is_tx_fifo_empty(PIO pio, uint sm);
//static inline uint pio_sm_get_tx_fifo_level(PIO pio, uint sm);
//static inline void pio_sm_put_blocking(PIO pio, uint sm, uint32_t data);
//static inline uint32_t pio_sm_get_blocking(PIO pio, uint sm);
//void pio_sm_drain_tx_fifo(PIO pio, uint sm);

/*! \brief Clear a state machine's TX and RX FIFOs
 *  \ingroup hardware_pio
 *
 * \param sm pio_sm_hw_t
 */
inline void pio_sm_clear_fifos(pio_sm_hw_t& sm) {
    // changing the FIFO join state clears the fifo
    hw_xor_bits(&sm.shiftctrl, PIO_SM0_SHIFTCTRL_FJOIN_RX_BITS);
    hw_xor_bits(&sm.shiftctrl, PIO_SM0_SHIFTCTRL_FJOIN_RX_BITS);
}

//void pio_sm_set_pins(pio_sm_hw_t& sm, uint32_t pin_values);
//void pio_sm_set_pins_with_mask(pio_sm_hw_t& sm, uint32_t pin_values, uint32_t pin_mask);
//void pio_sm_set_pindirs_with_mask(pio_sm_hw_t& sm, uint32_t pin_dirs, uint32_t pin_mask);

/*! \brief Use a state machine to set the same pin direction for multiple consecutive pins for the PIO instance
 *  \ingroup hardware_pio
 *
 * This method repeatedly reconfigures the target state machine's pin configuration and executes 'set' instructions to set the pin direction on consecutive pins,
 * before restoring the state machine's pin configuration to what it was.
 *
 * This method is provided as a convenience to set initial pin directions, and should not be used against a state machine that is enabled.
 *
 * \param sm pio_sm_hw_t
 * \param pin_base the first pin to set a direction for
 * \param pin_count the count of consecutive pins to set the direction for
 * \param is_out the direction to set; true = out, false = in
 */
inline void pio_sm_set_consecutive_pindirs(pio_sm_hw_t& sm, uint pin, uint count, bool is_out)
{
    uint32_t pinctrl_saved = sm.pinctrl;
    uint pindir_val = is_out ? 0x1f : 0;
    while (count > 5) {
        sm.pinctrl = (5u << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (pin << PIO_SM0_PINCTRL_SET_BASE_LSB);
        sm.instr = pio_encode_set(pio_pindirs, pindir_val);
        count -= 5;
        pin = (pin + 5) & 0x1f;
    }
    sm.pinctrl = (count << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (pin << PIO_SM0_PINCTRL_SET_BASE_LSB);
    sm.instr = pio_encode_set(pio_pindirs, pindir_val);
    sm.pinctrl = pinctrl_saved;
}

inline void pio_sm_set_pindir(pio_sm_hw_t& sm, uint pin, bool is_out)
{
    uint32_t pinctrl_saved = sm.pinctrl;
    uint pindir_val = is_out ? 0x1f : 0;
    sm.pinctrl = (1 << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (pin << PIO_SM0_PINCTRL_SET_BASE_LSB);
    sm.instr = pio_encode_set(pio_pindirs, pindir_val);
    sm.pinctrl = pinctrl_saved;
}

#ifdef __cplusplus
}
#endif
