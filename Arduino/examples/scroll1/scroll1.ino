/*
  Scroll 1 on TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include <Media.h>
#include <TSDesktop.h>
#include <Fonts/TSD_FreeSerif_24pt.h>

#define FONT FreeSerif_24pt

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

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

void setup() {

  media_begin(ROTATION);

  pinMode(LED_PIN, OUTPUT);

  btn01.setSize(ALIGN_COMPACT, 400);
  btn01.setBackground(BROWN);
  btn01.setFont(FONT);
  btn01.setStaticText("Abcdefghijklmnopqrstuvwxyz");
  btn01.setFontSize(5);
  btn01.setTextColor(WHITE);

  div01.enableScroll(SCROLL_PROGRESS);

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

  delay(50);

  pin13state = 1 - pin13state;
  digitalWrite(LED_PIN, pin13state);
}

// ----------------------------------------------------------------
