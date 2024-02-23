/*
  SSD1963_480 init
*/

  sendCmdData(set_pll_mn, (uint8_t*)"\x23\x02\x54", 3);
  sendCmdByte(set_pll, 0x01);   //PLL multiplier, set PLL clock to 120M
    // 0x23 N=0x36 for 6.5M, 0x23 for 10M crystal

  delay(10);
  sendCmdByte(set_pll, 0x03);
  delay(10);

  sendCmd(TFT_SWRESET);

  delay(100);

  //PLL setting for PCLK, depends on resolution
  sendCmdData(set_lshift_freq, (uint8_t*)"\x01\x1F\xFF", 3);
  sendCmdData(set_lcd_mode, (uint8_t*)"\x20\x00\x01\xDF\x01\x0F\x00", 7);
    // 0x20
    // 0x00
    // 0x01 Set HDP 479
    // 0xDF
    // 0x01 Set VDP 271
    // 0x0F
    // 0x00

  sendCmdData(set_hori_period, (uint8_t*)"\x02\x13\x00\x08\x2B\x00\x02\x00",  8); // HSYNC
    // 0x02 Set HT  531
    // 0x13
    // 0x00 Set HPS 8
    // 0x08
    // 0x2B Set HPW 43
    // 0x00 Set LPS 2
    // 0x02
    // 0x00

  sendCmdData(set_vert_period, (uint8_t*)"\x01\x20\x00\x04\x0c\x00\x02", 7); // VSYNC
    // 0x01 Set VT  288
    // 0x20
    // 0x00 Set VPS 4
    // 0x04
    // 0x0c Set VPW 12
    // 0x00 Set FPS 2
    // 0x02

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
