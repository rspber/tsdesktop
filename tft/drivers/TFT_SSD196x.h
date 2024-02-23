/*
  SSD196x defines
*/

#pragma once

//                        0x3A  // Reserved

#define read_ddb          0xA1   // Read the DDB from the provided location
//                        0xA8   // Reserved
#define set_lcd_mode      0xB0   // Set the LCD panel mode and resolution
#define get_lcd_mode      0xB1   // Get the current LCD panel mode, pad strength and resolution
#define set_hori_period   0xB4   // Set front porch
#define get_hori_period   0xB5   // Get current front porch settings
#define set_vert_period   0xB6   // Set the vertical blanking interval between last scan line and next LFRAME pulse
#define get_vert_period   0xB7   // Set the vertical blanking interval between last scan line and next LFRAME pulse
#define set_gpio_conf     0xB8   // Set the GPIO configuration. If the GPIO is not used for LCD, set the direction. Otherwise, they are toggled with LCD signals.
#define get_gpio_conf     0xB9   // Get the current GPIO configuration
#define set_gpio_value    0xBA   // Set GPIO value for GPIO configured as output
#define get_gpio_status   0xBB   // Read current GPIO status. If the individual GPIO was configured as input, the value is the status of the corresponding pin. Otherwise, it is the programmed value.
#define set_post_proc     0xBC   // Set the image post processor
#define get_post_proc     0xBD   // Set the image post processor
#define set_pwm_conf      0xBE   // Set the image post processor
#define get_pwm_conf      0xBF   // Set the image post processor
#define set_lcd_gen0      0xC0   // Set the rise, fall, period and toggling properties of LCD signal generator 0
#define get_lcd_gen0      0xC1   // Get the current settings of LCD signal generator 0
#define set_lcd_gen1      0xC2   // Set the rise, fall, period and toggling properties of LCD signal generator 1
#define get_lcd_gen1      0xC3   // Get the current settings of LCD signal generator 1
#define set_lcd_gen2      0xC4   // Set the rise, fall, period and toggling properties of LCD signal generator 2
#define get_lcd_gen2      0xC5   // Get the current settings of LCD signal generator 2
#define set_lcd_gen3      0xC6   // Set the rise, fall, period and toggling properties of LCD signal generator 3
#define get_lcd_gen3      0xC7   // Get the current settings of LCD signal generator 3
#define set_gpio0_rop     0xC8   // Set the GPIO0 with respect to the LCD signal generators using ROP operation. No effect if the GPIO0 is configured as general GPIO.
#define get_gpio0_rop     0xC9   // Get the GPIO0 properties with respect to the LCD signal generators.
#define set_gpio1_rop     0xCA   // Set the GPIO1 with respect to the LCD signal generators using ROP operation. No effect if the GPIO1 is configured as general GPIO.
#define get_gpio1_rop     0xCB   // Get the GPIO1 properties with respect to the LCD signal generators.
#define set_gpio2_rop     0xCC   // Set the GPIO2 with respect to the LCD signal generators using ROP operation. No effect if the GPIO2 is configured as general GPIO.
#define get_gpio2_rop     0xCD   // Get the GPIO2 properties with respect to the LCD signal generators.
#define set_gpio3_rop     0xCE   // Set the GPIO3 with respect to the LCD signal generators using ROP operation. No effect if the GPIO3 is configured as general GPIO.
#define get_gpio3_rop     0xCF   // Get the GPIO3 properties with respect to the LCD signal generators.
#define set_dbc_conf      0xD0   // Set the dynamic back light configuration
#define get_dbc_conf      0xD1   // Get the current dynamic back light configuration
#define set_dbc_th        0xD4   // Set the threshold for each level of power saving
#define get_dbc_th        0xD5   // Get the threshold for each level of power saving
#define set_pll           0xE0   // Start the PLL. Before the start, the system was operated with the crystal oscillator or clock input
#define set_pll_mn        0xE2   // Set the PLL
#define get_pll_mn        0xE3   // Get the PLL settings
#define get_pll_status    0xE4   // Get the current PLL status
#define set_deep_sleep    0xE5   // Set deep sleep mode
#define set_lshift_freq   0xE6   // Set the LSHIFT (pixel clock) frequency
#define get_lshift_freq   0xE7   // Get current LSHIFT (pixel clock) frequency setting
//                        0xE8   // Reserved
//                        0xE9   // Reserved
#define set_pix_dt_intf   0xF0   // Set the pixel data format of the parallel host processor interface
#define get_pix_dt_intf   0xF1   // Get the current pixel data format settings
//                        0xFF   // Reserved
