/*
  ILI9481_YA2 init
*/

  sendCmd(TFT_SWRESET);
  delay(220);
  sendCmd(TFT_SLPOUT);
  delay(280);
  sendCmdData(ILI9481_POWERSET, (uint8_t*)"\x07\x44\x1E", 3);
    // 07  VC[2:0] Sets the ratio factor of Vci to generate the reference voltages Vci1
    // 41  BT[2:0] Sets the Step up factor and output voltage level from the reference voltages Vci1
    // 1F  17   1C  VRH[3:0]: Sets the factor to generate VREG1OUT from VCILVL
  delay(220);
  sendCmdData(ILI9481_VCOMCTL, (uint8_t*)"\x00\x0C\x1A", 3);
    // 00
    // 1A   VCM [6:0] is used to set factor to generate VCOMH voltage from the reference voltage VREG1OUT  15    09
    // 1F   VDV[4:0] is used to set the VCOM alternating amplitude in the range of VREG1OUT x 0.70 to VREG1OUT   1F   18
  sendCmdByte(ILI9481_FRAINCTL, 0x03);
  sendCmdData(ILI9481_POWNORMD, (uint8_t*)"\x01\x11", 2);
  sendCmdByte(0xE4, 0xa0);  // undocumented
  sendCmdData(0xF3, (uint8_t*)"\x00\x2a", 2); // undocumented
  sendCmdData(ILI9481_GAMMASET, (uint8_t*)"\x00\x26\x21\x00\x00\x1f\x65\x23\x77\x00\x0f\x00", 12);
  sendCmdData(ILI9481_PANELDRV, (uint8_t*)"\x00\x3B\x00\x02\x11", 5);
    // 1 00  REV  SM  GS
    // 2 NL[5:0]: Sets the number of lines to drive the LCD at an interval of 8 lines.
    // 3 SCN[6:0]
    // 4 PTV: Sets the Vcom output in non-display area drive period
    // 5 NDL: Sets the source output level in non-display area.  PTG: Sets the scan mode in non-display area.
  sendCmdByte(ILI9481_INTRFCTL, 0x83);
  sendCmdByte(0xF0, 0x01);  // undocumented
  sendCmdByte(0xE4, 0xa0);  // undocumented
  // 倒装设置   NG
  sendCmdByte(TFT_MADCTL, 0x0A); //  8C:出来两行   CA：出来一个点
  sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
  sendCmdData(ILI9481_DMFMWSET, (uint8_t*)"\x02\x00\x00\x01", 4);
  delay(280);
  //sendCmd(TFT_INVON);
  sendCmd(TFT_DISPON);

