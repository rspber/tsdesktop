/*
  Calculator on TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#define TX_COLOR  LIGHT_GRAY
#define BG_COLOR  DODGER_BLUE
#define CBG_COLOR  RGB(0x40, 0x40, 0x40)

#include <Display.h>
#include <Touch.h>
#include <TSDesktop.h>
#include <Editable.h>

Display display;
bool screenEnabled = false;

Touch touch;

#define ROTATION 2

Editable operBtn(80);
TextButton sumBtn;
TextButton kDelBtn;
TextButton kMvRBtn;
TextButton kModBtn;
TextButton kMvLBtn;
TextButton kInsBtn;
TextButton k09Btn;
TextButton k08Btn;
TextButton k07Btn;
TextButton k06Btn;
TextButton k05Btn;
TextButton k04Btn;
TextButton k03Btn;
TextButton k02Btn;
TextButton k01Btn;
TextButton k00Btn;
TextButton kLParBtn;
TextButton kRParBtn;
TextButton kDotBtn;
TextButton kMulBtn;
TextButton kDivBtn;
TextButton kSubBtn;
TextButton kAddBtn;
TextButton kExcBtn;
TextButton kClrBtn;
TextButton kBSBtn;

#define BTN_DIST 12

Container* R4T[]{
  &kClrBtn, &kMvLBtn, &kMvRBtn, &kDelBtn, &kBSBtn
};

FieldSet r4FSet(HORIZONTAL, BTN_DIST, 0, 0, R4T, 5);

Container* R3T[]{
  &kLParBtn, &k07Btn, &k08Btn, &k09Btn, &kDivBtn
};

FieldSet r3FSet(HORIZONTAL, BTN_DIST, 0, 0, R3T, 5);

Container* R2T[]{
  &kRParBtn, &k04Btn, &k05Btn, &k06Btn, &kMulBtn
};

FieldSet r2FSet(HORIZONTAL, BTN_DIST, 0, 0, R2T, 5);

Container* R1T[]{
  &kModBtn, &k01Btn, &k02Btn, &k03Btn, &kSubBtn
};

FieldSet r1FSet(HORIZONTAL, BTN_DIST, 0, 0, R1T, 5);

Container* R0T[]{
  &kExcBtn, &kInsBtn, &k00Btn, &kDotBtn, &kAddBtn
};

FieldSet r0FSet(HORIZONTAL, BTN_DIST, 0, 0, R0T, 5);

Container* CT[]{
  &operBtn, &sumBtn, &r4FSet, &r3FSet, &r2FSet, &r1FSet, &r0FSet
};

FieldSet desktop(0, 0, ALIGN_CLIENT, ALIGN_CLIENT, CT, 7);

// ----------------------------------------------------------------

void calc_sum_btn(TextButton* b)
{
  b->setFontSize(2);
  b->setRadius(10);
  b->setBorderColor(SILVER);
//  b->setTransparent(true);
  b->setTextColor(TX_COLOR);
  b->setBackground(BG_COLOR);
  b->setAlignClientHoriz();
}

void btn_click(Container* c)
{
  int id = c->getId();
  switch (id) {
  case 1:
  {
    TextButton* b = static_cast<TextButton*>(c);
    operBtn.cmdInsChar(*b->getText());
    break;
  }
  case 2:
  case 4:
    operBtn.moveCursor(id == 4 ? -1 : +1);
    break;
  case -4:  // Ins
    operBtn.swapCursorInsMode();
    break;
  case -5:  // Del
  case -8:  // BS
    operBtn.cmdDelBS(id == -8);
    break;
  case -3:  // CC
    operBtn.setText("");
    break;
  }
  desktop.draw();
}

void calc_btn(TextButton* b, const int id, const char* text)
{
  b->setId(id);
  b->setText(text);
  int BTN_SIZE = display.width() / 7;
  b->setSize(BTN_SIZE, BTN_SIZE);
  b->setFontSize(2);
  b->setRadius(8);
  b->setBorderSize(2);
  b->setBorderColor(SILVER);
//  b->setTransparent(true);
  b->setTextColor(TX_COLOR);
  b->setBackground(BG_COLOR);
  b->setOnClick(btn_click);
  b->setEnabled();
}

void calc_row(FieldSet* t)
{
  t->setTransparent(true);
  t->setBorderSize(0);
}

uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;
int pin13state = LOW;

void setup() {

  init_hardware();
  init_spi();

  media_begin(ROTATION);

  pinMode(LED_PIN, OUTPUT);

  calc_btn(&kDelBtn, -5, "Del");
  kDelBtn.setFontSize(1);
  calc_btn(&kMvRBtn, 2, "->");
  calc_btn(&kModBtn, 1, "%");
  calc_btn(&kMvLBtn, 4, "<-");
  calc_btn(&kInsBtn, -4, "Ins");
  kInsBtn.setFontSize(1);
  calc_btn(&k09Btn, 1, "9");
  calc_btn(&k08Btn, 1, "8");
  calc_btn(&k07Btn, 1, "7");
  calc_btn(&k06Btn, 1, "6");
  calc_btn(&k05Btn, 1, "5");
  calc_btn(&k04Btn, 1, "4");
  calc_btn(&k03Btn, 1, "3");
  calc_btn(&k02Btn, 1, "2");
  calc_btn(&k01Btn, 1, "1");
  calc_btn(&k00Btn, 1, "0");
  calc_btn(&kLParBtn, 1, "(");
  calc_btn(&kRParBtn, 1, ")");
  calc_btn(&kDotBtn, 1, ".");
  calc_btn(&kMulBtn, 1, "*");
  calc_btn(&kDivBtn, 1, "/");
  calc_btn(&kSubBtn, 1, "-");
  calc_btn(&kAddBtn, 1, "+");
  calc_btn(&kExcBtn, -7, "=");
  calc_btn(&kClrBtn, -3, "CC");
  calc_btn(&kBSBtn, -8, "BS");

  calc_sum_btn(&operBtn);
  calc_sum_btn(&sumBtn);

  calc_row(&r4FSet);
  calc_row(&r3FSet);
  calc_row(&r2FSet);
  calc_row(&r1FSet);
  calc_row(&r0FSet);

  desktop.setBackground(CBG_COLOR);
  desktop.setOrientation(VERTICAL, BTN_DIST);

  screenEnabled = true;

  desktop.draw();
}

void loop() {

  point_t p;
  if (touch.getTouch(&p)) {
    /*Container* b =*/ desktop.pressed(p.x, p.y);
  }

  delay(50);

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
