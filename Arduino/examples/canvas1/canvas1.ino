/*
  Canvas 1 on TSDesktop

  overlaid

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include <Media.h>
#include <TSDesktop.h>
#include <GFXStd.h>
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

gradient_t g1{GNT_LR, RED, 100, BLUE};
GFXFillRectGradient frg1(0, 0, 100, 220, g1);

gradient_t g2{GNT_BT, GREEN, 20, CYAN};
GFXFillRectGradient frg2(110, 0, 300, 100, g2);

GFXFillCircle fc1(60, 10, 30, BLUE);
GFXCircleFragment cf1(140, 180, 20, 0x06, BROWN, 5);
GFXLine l1(200, 30, 250, 160, AQUA, 6);
GFXLine l2(50, 160, 150, 190, AQUA, 6);
GFXLine l3(160, 100, 250, 200, AQUA, 6);
GFXLine l4(280, 20, 280, 200, AQUA, 3);
GFXRect r1(180, 50, 80, 60, GOLDEN, 5);
GFXFillRect fr1(140, 10, 40, 20, GOLDEN);
GFXRoundRect rr1(180, 150, 80, 60, 5, GREEN, 8);
GFXFillCircle fc2(100, 100, 20, BLUE);
GFXTriangle t1(200, 80, 350, 120, 210, 200, SILVER, 5);
GFXFillTriangle ft1(160, 40, 180, 60, 170, 80, BLUE);
GFXCircle c1(100, 100, 50, GOLDEN, 5);
GFXStaticTextLine tx(30, 150, "5V", RED, RED);
GFXLine line1(0, 0, 100, 100, AQUA, 1);
GFXLine line2(100, 100, 320, 100, RED, 1);

GFXStaticTextLine overlaid(80, 5, "", OLIVE, OLIVE);

// ----------------------------------------------------------------

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

void setup()   {

  media_begin(ROTATION);

  pinMode(LED_PIN, OUTPUT);

  canvas.setBackground(LIME);
  canvas.add(frg1);
  canvas.add(frg2);

  canvas.add(fc1);
  canvas.add(cf1);
  canvas.add(l1); 
  canvas.add(l2);
  canvas.add(l3);
  canvas.add(l4);
  canvas.add(r1);
  canvas.add(fr1);
  canvas.add(rr1);
  canvas.add(c1);
  canvas.add(fc2);
  canvas.add(t1);
  canvas.add(ft1);
  canvas.add(tx);    tx.setFontSize(3);
  canvas.add(line1);   line1.setOverlaid(true);
  canvas.add(line2);   line2.setOverlaid(true);
  canvas.add(overlaid);

  tcentb.setAlignClient();
  tcentb.setAlignCenter(true);
  tcentb.setTransparent(true);

  desktop.setBackground(AQUA);
  desktop.setOrientation(HORIZONTAL, 2);

  screenEnabled = true;

  char buf[80];
  #if defined(COLOR_565)
    const char* K = "64K";
  #else
    const char* K = "256K";
  #endif
  snprintf(buf, 80, "%s %dx%d %s OVERLAID", display.protocol(), display.width(), display.height(), K);
  overlaid.setText(buf);

  desktop.draw();
}

// ----------------------------------------------------------------

void loop() {

  point_t p;
  if (touch.getTouch(&p)) {
    /*Container* b =*/ desktop.pressed(p.x, p.y);
  }
  line1.hide();
  line1.setY1(line1.getY1() + 1);
  if (line1.getY1() > 200) {
    line1.setY1(0);
  }

  line2.hide();
  line2.setY2(line2.getY2() + 1);
  if (line2.getY2() > 200) {
    line2.setY2(0);
  }

//  line1.draw();
//  line2.draw();
  
  desktop.draw();

//  delay(50);

  pin13state = 1 - pin13state;
  digitalWrite(LED_PIN, pin13state);
}

// ----------------------------------------------------------------
