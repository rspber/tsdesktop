/*
  Canvas 1 on TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include <Media.h>
#include <TSDesktop.h>
#include <GFXButton.h>

Display display;
bool screenEnabled = false;

Touch touch;

#define ROTATION ROTATION_HLR

GFXButton canvas;

Container* CANVT[]{
  &canvas
};

FieldSet tcentb(VERTICAL, 0, 10, 10, CANVT, 1);

Container* LCRT[]{
  &tcentb
};

FieldSet desktop(0, 0, ALIGN_CLIENT, ALIGN_CLIENT, LCRT, 1);

// ----------------------------------------------------------------

uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;
int pin13state = LOW;

GFXLine* line1;
GFXLine* line2;

void setup()   {

  media_begin(ROTATION);

  pinMode(LED_PIN, OUTPUT);

  canvas.setBackground(YELLOW);
  gradient_t g1{GNT_LR, RED, 100, BLUE};
  canvas.fillRectGradient(0, 0, 100, 220, &g1);
  gradient_t g2{GNT_BT, GREEN, 20, CYAN};
  canvas.fillRectGradient(110, 0, 300, 100, &g2);

  canvas.fillCircle(60, 10, 30, BLUE);
  canvas.circleFragment(140, 180, 20, 0x06, BROWN, 5);
  canvas.line(200, 30, 250, 160, AQUA, 10)->mode=1; // cut
  canvas.line(50, 160, 150, 190, AQUA, 10);
  canvas.line(160, 100, 250, 200, AQUA, 10);
  canvas.line(280, 20, 280, 200, AQUA, 3);
  canvas.rect(180, 50, 80, 60, DARK_GREEN, 10);
  canvas.fillRect(140, 10, 40, 20, GREEN);
  canvas.roundRect(180, 150, 80, 60, 5, DARK_GREEN, 8);
  canvas.circle(100, 100, 50, BLACK, 5);
  canvas.fillCircle(100, 100, 20, BLUE);
  canvas.triangle(200, 80, 350, 120, 210, 200, BLACK, 5);
  canvas.fillTriangle(160, 40, 180, 60, 170, 80, PINK);
// canvas.textLine(30, 150, "5V", RED)->setFontSize(3);
  GFXTextLine* tx;
  tx = canvas.textLine(30, 150, "5V", RED, RED);
  tx->setFontSize(3);

  line1 = canvas.line(0, 0, 100, 100, AQUA, 1);
  line1->setOverlaid(true);

  line2 = canvas.line(100, 100, 320, 100, RED, 1);
  line2->setOverlaid(true);

  tcentb.setAlignClient();
  tcentb.setAlignCenter(true);
  tcentb.setTransparent(true);

  desktop.setBackground(AQUA);
  desktop.setOrientation(HORIZONTAL, 2);

  screenEnabled = true;

  desktop.draw();
}

void loop() {

  point_t p;
  if (touch.getTouch(&p)) {
    /*Container* b =*/ desktop.pressed(p.x, p.y);
  }
  line1->hide();
  line2->hide();

  line1->setY1(line1->getY1() + 1);
  if (line1->getY1() > 200) {
    line1->setY1(0);
  }

  line2->setY2(line2->getY2() + 1);
  if (line2->getY2() > 200) {
    line2->setY2(0);
  }

//  line1->draw();
//  line2->draw();
  desktop.draw();

//  delay(50);

  pin13state = 1 - pin13state;
  digitalWrite(LED_PIN, pin13state);
}

int main() {

  setup();

  while (true) {
    loop();
  }
}

// ----------------------------------------------------------------
