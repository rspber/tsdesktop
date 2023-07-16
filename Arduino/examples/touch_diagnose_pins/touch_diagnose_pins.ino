/*
  Diagnose analog/digital touch pins

  Copyright (c) 2023, rspber (https://github.com/rspber)

  Based on: MCUFRIEND_kbv TouchScreen_Calibr_native.ino

  Originally notes below:
*/

// TouchScreen_Calibr_native for MCUFRIEND UNO Display Shields
// adapted by David Prentice
// for Adafruit's <TouchScreen.h> Resistive Touch Screen Library
// from Henning Karlsen's original UTouch_Calibration program.
// Many Thanks.

/*
  You are on the TFT touch screen diagnostics page.
  
  Your task is to find two analog pins and two digital pins
  under which the manufacturer of the display has hidden the touch.
  
  (1) Your display should have pins: LCD_CS, LCD_RS, LCD_WR, LCD_RST, LCD_RD.
  These are the manufacturers favorite pins for hiding analog connections.

  (2) And LCD D0 and LCD D1 or LCD_D6 and LCD_D7 are potential places to hide
  digital connections. The following method assumes that all LCD_D0 to LCD_D7
  pins are connected in ascending order.

  The touch connection detection method is to randomly connect analog (1) pins
  to analog pins, not necessarily all at once, and others (1) and digital (2)
  to digital pins.

  The diagnose_pins method will first set all connected pins to the input state
  and then, by activating one by one, it will check if there is voltage on some
  pairs of pins. If so, it is suspected that connections have been detected.

  It may happen that the diagnosis is ambiguous,
  be vigilant and do not be fooled by similar-looking results.

  ATTENTION:
  There is a trap hidden in the program.
  After loading the program, the program will stop and wait for the console:
  Arduino/Tools/Serial Monitor to start.
  
  Some technical informations copied from original code:
  Typical 30k Analog pullup with corresponding pin
  would read low when digital is written LOW
  e.g. reads ~25 for 300R X direction
  e.g. reads ~30 for 500R Y direction

 */

#include "Arduino.h"

// suggested start pinout: chance of success 33%

// 26   - LCD_CS   // analog A0
// 27   - LCD_RS   // analog A1

//  0   - LCD_RD
//  1   - LCD_WR
//  2   - LCD_RST

#define FIRST_DATA_PIN 8

//  8   - LCD_D0
//  9   - LCD_D1
// 10   - LCD_D2
// 11   - LCD_D3
// 12   - LCD_D4
// 13   - LCD_D5
// 14   - LCD_D6
// 15   - LCD_D7

void diagnose_pins()
{
  uint8_t Apins[20], Dpins[20];
  int16_t Values[20];

  int found = 0;

  // set all connected other pins as input
  for (int i = 0; i <= 2; i++) pinMode(i, INPUT_PULLUP);

  // set all analog pins as input
  for (int i = A0; i <= A2; i++) pinMode(i, INPUT_PULLUP);

  // set all connected data pins as input
  for (int i = FIRST_DATA_PIN; i < FIRST_DATA_PIN + 8; i++) pinMode(i, INPUT_PULLUP);

  // for all analog pins
  for (int i = A0; i <= A2; i++) {
    pinMode(i, INPUT_PULLUP);
    // for all data pins  
    for (int j = FIRST_DATA_PIN; j < FIRST_DATA_PIN + 8; j++) {
      pinMode(j, OUTPUT);
      digitalWrite(j, LOW);
      int16_t value;
      value = analogRead(i);               // ignore first reading
      value = analogRead(i);
      if (value < 100 && value > 0) {
        Serial.printf("Testing : (A%d, D%d) = %d\n", i - 26, j, value);
        if (found < 20) {
          Apins[found] = i;
          Dpins[found] = j;
          Values[found] = value;
        }
        ++found;
      }
      pinMode(j, INPUT_PULLUP);
    }
    pinMode(i, INPUT_PULLUP);
  }

  if (found >= 2) {
    int idx = Values[0] < Values[1];
    Serial.println(F("Diagnosing as:-"));
    for (int i = 0; i < 2; i++) {
      Serial.printf("%s: (A%d, D%d) = %d\n", Values[i] < Values[1-i] ? "X-,X+" : "Y+,Y-", Apins[i] - 26, Dpins[i], Values[i]);
    }
  }
  if (found == 0) Serial.println(F("MISSING TOUCHSCREEN"));
    //else Serial.println(F("BROKEN TOUCHSCREEN"));
}

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  diagnose_pins();
}

void loop()
{
}
