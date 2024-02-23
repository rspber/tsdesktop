/*
  SSD1963_800BD init
*/

  sendCmdData(set_pll_mn, (uint8_t*)"\x23\x02\x54", 3);     //PLL multiplier, set PLL clock to 120M
    // 0x23 N=0x36 for 6.5M, 0x23 for 10M crystal

  // PLL enable
  sendCmdByte(set_pll, 0x01);
  delay(10);
  sendCmdByte(set_pll, 0x03);
  delay(10);

  sendCmd(TFT_SWRESET);

  delay(100);

  //PLL setting for PCLK, depends on resolution
  sendCmdData(set_lshift_freq, (uint8_t*)"\x03\x33\x33", 3);

  {
    uint8_t dd[] {0x20, 0x00, 799 >> 8, 799 & 0xFF, 479 >> 8, 479 & 0xFF, 0x00};
    sendCmdData(set_lcd_mode, dd, 7);
    // 0x20
    // 0x00
    // (799 >> 8) Set HDP 799
    // (799 & 0xFF)
    // (479 >> 8) Set VDP 479
    // (479 & 0xFF)
    // 0x00
  }

  sendCmdData(set_hori_period, (uint8_t*)"\x04\x1F\x00\xD2\x00\x00\x00\x00", 8); // HSYNC
    // 0x04 Set HT
    // 0x1F
    // 0x00 Set HPS
    // 0xD2
    // 0x00 Set HPW
    // 0x00 Set LPS
    // 0x00

  sendCmdData(set_vert_period, (uint8_t*)"\x02\x0C\x00\x22\x00\x00\x00", 7); // VSYNC
    // 0x02 Set VT
    // 0x0C
    // 0x00 Set VPS
    // 0x22
    // 0x00 Set VPW
    // 0x00 Set FPS
    // 0x00

  sendCmdData(set_gpio_conf, (uint8_t*)"\x0F\x01", 2);
    // 0x0F GPIO3=input, GPIO[2:0]=output
    // 0x01 GPIO0 normal

  sendCmdByte(set_gpio_value, 0x01);      // 0x01 GPIO[3:0] out 1 --- LCD display on/off control PIN

  sendCmdByte(TFT_MADCTL, 0x21 | MAD_BGR);

  sendCmdByte(set_pix_dt_intf, 0x00);
    // 0x00     //000 = 8 bit bus, 011 = 16 bit, 110 = 9 bit

  sendCmdData(set_post_proc, (uint8_t*)"\x40\x80\x40\x01", 4);
    // 0x40 contrast value
    // 0x80 brightness value
    // 0x40 saturation value
    // 0x01 Post Processor Enable

  delay(10);

  sendCmd(TFT_DISPON);

  //set PWM for B/L
  sendCmdData(set_pwm_conf, (uint8_t*)"\x06\x80\x01\xF0\x00\x00", 6);
  sendCmdByte(set_dbc_conf, 0x0D);
