/*
  SSD1963_800 init
*/

  sendCmdData(set_pll_mn, (uint8_t*)"\x1E\x02\x54", 3);     //PLL multiplier, set PLL clock to 120M
    // 0x1E N=0x36 for 6.5M, 0x23 for 10M crystal

  // PLL enable
  sendCmdByte(set_pll, 0x01);
  delay(10);
  sendCmdByte(set_pll, 0x03);
  delay(10);

  sendCmd(TFT_SWRESET);

  delay(100);

  //PLL setting for PCLK, depends on resolution
  sendCmdData(set_lshift_freq, (uint8_t*)"\x03\xFF\xFF", 3);
  sendCmdData(set_lcd_mode, (uint8_t*)"\x20\x00\x03\x1F\x01\xDF\x00", 7);
    // 0x20
    // 0x00
    // 0x03 Set HDP 799
    // 0x1F
    // 0x01 Set VDP 479
    // 0xDF
    // 0x00

  sendCmdData(set_hori_period, (uint8_t*)"\x03\xA0\x00\x2E\x30\x00\x0F\x00", 8); // HSYNC
    // 0x03 Set HT  928
    // 0xA0
    // 0x00 Set HPS 46
    // 0x2E
    // 0x30 Set HPW 48
    // 0x00 Set LPS 15
    // 0x0F
    // 0x00

  sendCmdData(set_vert_period, (uint8_t*)"\x02\x0D\x00\x10\x10\x00\x08", 7); // VSYNC
    // 0x02 Set VT  525
    // 0x0D
    // 0x00 Set VPS 16
    // 0x10
    // 0x10 Set VPW 16
    // 0x00 Set FPS 8
    // 0x08

  sendCmdByte(set_gpio_value, 0x0F);      // out 1

  sendCmdData(set_gpio_conf, (uint8_t*)"\x07\x01", 2);
    // 0x07 GPIO3=input, GPIO[2:0]=output
    // 0x01 GPIO0 normal

  sendCmdByte(TFT_MADCTL, 0x21 | MAD_BGR);

  sendCmdByte(set_pix_dt_intf, 0x00);
    // 0x00 8 bit bus

  delay(1);

  sendCmdData(set_gpio_conf, (uint8_t*)"\x0f\x01", 2);
    // 0x0f GPIO is controlled by host GPIO[3:0]=output   GPIO[0]=1  LCD ON  GPIO[0]=1  LCD OFF
    // 0x01 GPIO0 normal

  sendCmdByte(set_gpio_value, 0x01);      //GPIO[0] out 1 --- LCD display on/off control PIN

  sendCmd(TFT_DISPON);

  //set PWM for B/L
  sendCmdData(set_pwm_conf, (uint8_t*)"\x06\xF0\x01\xF0\x00\x00", 6);
  sendCmdByte(set_dbc_conf, 0x0D);
