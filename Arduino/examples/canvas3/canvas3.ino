/*
  Canvas 3 on TSDesktop

  buffered

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include <Media.h>
#include <TSDesktop.h>
#include <GfxStd.h>
#include <GfxButton.h>

Display display;
bool screenEnabled = false;

Touch touch;

#define ROTATION ROTATION_HLR

GfxButton gfxbtn;
TextButton b1("half of the available memory", -1, -1, ALIGN_CLIENT, ALIGN_CLIENT, CYAN);
TextButton b2("not enough memeory for buffered display", -1, -1, ALIGN_CLIENT, 0, RED);

Container* CANVT[]{
  &gfxbtn,
  &b1,
  &b2
};

FieldSet tcentb(VERTICAL, 0, 10, 10, CANVT, 3);

Container* LCRT[]{
  &tcentb
};

FieldSet desktop(0, 0, ALIGN_CLIENT, ALIGN_CLIENT, LCRT, 1);

// ----------------------------------------------------------------

gradient_t g1{GNT_LR, RED, 100, BLUE};
GfxFillRectGradient frg1(0, 0, 100, 220, g1);

gradient_t g2{GNT_BT, GREEN, 20, CYAN};
GfxFillRectGradient frg2(110, 0, 300, 100, g2);

GfxFillCircle fc1(60, 10, 30, BLUE);
GfxCircleFragment cf1(140, 180, 20, 0x06, BROWN, 5);
GfxLine l1(200, 30, 250, 160, AQUA, 6);
GfxLine l2(50, 160, 150, 190, AQUA, 6);
GfxLine l3(160, 100, 250, 200, AQUA, 6);
GfxLine l4(280, 20, 280, 200, AQUA, 3);
GfxRect r1(180, 50, 80, 60, GOLDEN, 5);
GfxFillRect re(140, 10, 40, 20, GOLDEN);
GfxRoundRect rr1(180, 150, 80, 60, 5, GREEN, 8);
GfxFillCircle fc2(100, 100, 20, BLUE);
GfxTriangle t1(200, 80, 350, 120, 210, 200, SILVER, 5);
GfxFillTriangle ft1(160, 40, 180, 60, 170, 80, BLUE);
GfxCircle cr(100, 100, 50, GOLDEN, 1);
GfxEllipse eli(260, 100, 30, 10, RED);
GfxFillEllipse ufo(200, 160, 30, 10, PINK);
GfxStaticTextLine tx(30, 150, "5V", RED, RED);

GfxStaticTextLine buffered(80, 5, "", OLIVE, OLIVE);

// ----------------------------------------------------------------

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

void setup()   {

  media_begin(ROTATION);

  pinMode(LED_PIN, OUTPUT);

  Canvas* canvas = gfxbtn.getCanvas();
  
  gfxbtn.setBackground(LIME);

  int width = display.width() - 20;
  int height = display.height() - 20;

  #if defined(COLOR_565)
    int maxsize = 122 * 1000;  // * 2 = 244kB
  #else
    int maxsize = 81 * 1000;  // * 3 = 243kB
  #endif

  int size = width * height;
  if (size > maxsize) {
    b2.setHeight( (size - maxsize) / width );
  }
  else {
    b2.setHidden(true);
  }

  b1.setHidden(true);

  canvas->setBuffered(true);

  canvas->add(frg1);
  canvas->add(frg2);

  canvas->animate(fc1, 2, 1);
  canvas->animate(cf1, -2, 1);
  canvas->animate(l1, 2, -1);    l1.mode=1; // cut
  canvas->animate(l2, -2, -1);
  canvas->animate(l3, 2, 1);
  canvas->animate(l4, -2, 1);
  canvas->animate(r1, 2, -1);
  canvas->animate(re, -2, -1);
  canvas->animate(rr1, 2, 1);
  canvas->animate(fc2, -2, 1);
  canvas->animate(t1, 2, -1);
  canvas->animate(ft1, -2, -1);
  canvas->animate(cr, 2, 1);
  canvas->animate(eli, -2, 1);
  canvas->animate(ufo, 2, -1);
  canvas->animate(tx, -2, -1);    tx.setFontSize(3);

  canvas->add(buffered);

  tcentb.setAlignClient();
  tcentb.setAlignCenter(true);
  tcentb.setTransparent(true);

  desktop.setBackground(AQUA);
  desktop.setOrientation(HORIZONTAL, 2);

  screenEnabled = true;

  desktop.draw();
}

// ----------------------------------------------------------------

int i = 0;
int d = 2;

void loop() {

  point_t p;
  if (touch.getTouch(&p)) {
    /*Container* b =*/ desktop.pressed(p.x, p.y);
  }

  if ((i+=d) > 50) {
    b1.setHidden(!b1.getHidden());
    i = 0;
    d = 3 - d;
  }

  char buf[80];
  clip_t clip;
  gfxbtn.getOuterClip(clip);
  #if defined(COLOR_565)
    const char* K = "64K";
  #else
    const char* K = "256K";
  #endif
  snprintf(buf, 80, "%s %dx%d %s BUFFERED", display.protocol(), clip.width(), clip.height(), K);
  buffered.setText(buf);

  desktop.draw();

//  delay(50);

  pin13state = 1 - pin13state;
  digitalWrite(LED_PIN, pin13state);
}

// ----------------------------------------------------------------
