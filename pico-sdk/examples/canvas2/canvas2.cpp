/*
  Canvas 2 on TSDesktop

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

GFXFillRect* re;
GFXTextLine* tx;
GFXCircle* cr;

void setup()   {

  init_hardware();
  init_spi();

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
  re = canvas.fillRect(140, 10, 40, 20, GREEN);
  re->setOverlaid(true);

  canvas.roundRect(180, 150, 80, 60, 5, DARK_GREEN, 8);
  canvas.fillCircle(100, 100, 20, BLUE);
  canvas.triangle(200, 80, 350, 120, 210, 200, BLACK, 5);
  canvas.fillTriangle(160, 40, 180, 60, 170, 80, PINK);

  cr = canvas.circle(100, 100, 50, BLACK, 1);
//  cr->setOverlaid(true); // circles have a redundant method drawing pixels

// canvas.textLine(30, 150, "5V", RED)->setFontSize(3);
  tx = canvas.textLine(30, 150, "5V", RED, RED);
  tx->setFontSize(3);
  tx->setOverlaid(true);

  tcentb.setAlignClient();
  tcentb.setAlignCenter(true);
  tcentb.setTransparent(true);

  desktop.setBackground(AQUA);
  desktop.setOrientation(HORIZONTAL, 2);

  screenEnabled = true;

  desktop.draw();
}

int adrx = 2;
int adry = 1;

int adcx = 2;
int adcy = 1;

int adtx = 2;
int adty = 1;

void loop() {

  point_t p;
  if (touch.getTouch(&p)) {
    /*Container* b =*/ desktop.pressed(p.x, p.y);
  }
  re->hide();

  re->setX1(re->getX1() + adrx);
  if (re->getX1() < 0 || re->getX1() > 260) {
    adrx = -adrx;
  }

  re->setY1(re->getY1() + adry);
  if (re->getY1() < 0 || re->getY1() > 180) {
    adry = -adry;
  }
/*
  cr->hide();

  cr->setX0(cr->getX0() + adcx);
  if (cr->getX0() < 50 || cr->getX0() > 210) {
    adcx = -adcx;
  }

  cr->setY0(cr->getY0() + adcy);
  if (cr->getY0() < 50 || cr->getY0() > 140) {
    adcy = -adcy;
  }
*/
  tx->hide();

  tx->setX1(tx->getX1() + adtx);
  if (tx->getX1() < 0 || tx->getX1() > 260) {
    adtx = -adtx;
  }

  tx->setY1(tx->getY1() + adty);
  if (tx->getY1() < 0 || tx->getY1() > 190) {
    adty = -adty;
  }


//  tx->draw();
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
