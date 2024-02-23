/*
  ST7796 init
*/

  delay(120);
  sendCmd(TFT_SWRESET);
  delay(120);
  sendCmd(TFT_SLPOUT);
  delay(120);
  sendCmdByte(ST7796_CMDSETCTL, 0xC3); // Enable extension command 2 partI
  sendCmdByte(ST7796_CMDSETCTL, 0x96); // Enable extension command 2 partII
  sendCmdByte(TFT_MADCTL, 0x48);
  sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
  sendCmdByte(ST7796_INVCTR, 0x01); // 1-dot inversion
  sendCmdData(ST7796_DFUNCTR, (uint8_t*)"\x80\x02\x3B", 3);
  // 0x80 Bypass
  // 0x02 Source Output Scan from S1 to S960, Gate Output scan from G1 to G480, scan cycle=2
  // 0x3B LCD Drive Line=8*(59+1)
  sendCmdData(ST7796_DIGOUTCA, (uint8_t*)"\x40\x8A\x00\x00\x29\x19\xA5\x33", 8);
  // 0x40
  // 0x8A
  // 0x00
  // 0x00
  // 0x29 Source eqaulizing period time= 22.5 us
  // 0x19 Timing for "Gate start"=25 (Tclk)
  // 0xA5 Timing for "Gate End"=37 (Tclk), Gate driver EQ function ON
  // 0x33
  sendCmdByte(ST7796_PWCTR2, 0x06); // VAP(GVDD)=3.85+( vcom+vcom offset), VAN(GVCL)=-3.85+( vcom+vcom offset)
  sendCmdByte(ST7796_PWCTR3, 0xA7); // Source driving current level=low, Gamma driving current level=High
  sendCmdByte(ST7796_VCOMCTR1, 0x18); // VCOM=0.9
  delay(120);
  sendCmdData(ST7796_PVGAMMA, (uint8_t*)"\xF0\x09\x0b\x06\x04\x15\x2F\x54\x42\x3C\x17\x14\x18\x1B", 14);
  sendCmdData(ST7796_NVGAMMA, (uint8_t*)"\xE0\x09\x0B\x06\x04\x03\x2B\x43\x42\x3B\x16\x14\x17\x1B", 14);
  delay(120);
  sendCmdByte(ST7796_CMDSETCTL, 0x3C); // Disable extension command 2 partI
  sendCmdByte(ST7796_CMDSETCTL, 0x69); // Disable extension command 2 partII
  delay(120);
  sendCmd(TFT_DISPON);
