/*
  Scroll1 on TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#include <Display.h>
#include <Touch.h>
#include <TSDesktop.h>
#include <Fonts/FreeSerif24pt7b.h>

#define ROTATION ROTATION_HLR

Display display;
bool screenEnabled = false;

Touch touch;

TextButton btn01;

Container* TBTT[]{
  &btn01
};

FieldSet div01(HORIZONTAL, 12, 13, 13, TBTT, 1);

Container* TMT[]{
  &div01
};

FieldSet desktop(0, 0, ALIGN_CLIENT, ALIGN_CLIENT, TMT, 1, AQUA);

uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;
int pin13state = LOW;

void setup()   {

  init_hardware();
  init_spi();

  media_begin(ROTATION);

  pinMode(LED_PIN, OUTPUT);

  btn01.setSize(1400, 150);
  btn01.setBackground(BROWN);
  btn01.setText("Abcdefghijklmnopqrstuvwxyz");
  btn01.setFont(&FreeSerif24pt7b);
  btn01.setFontSize(2);
  btn01.setTextColor(WHITE);

  desktop.setMargin(10);
  desktop.setOrientation(HORIZONTAL, 10);

  screenEnabled = true;
  desktop.draw();
}

// ----------------------------------------------------------------

void loop()
{
  point_t p;
  if (touch.getTouch(&p)) {
    /*Container* b =*/ desktop.pressed(p.x, p.y);
  }

  sleep_ms(50);

  pin13state = 1 - pin13state;
  digitalWrite(LED_PIN, pin13state);
}

// ----------------------------------------------------------------
