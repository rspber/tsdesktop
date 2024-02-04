/*
  Calculator on TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include <eval.h>
#include <Media.h>
#include <TSDesktop.h>
#include <Editable.h>

#define TX_COLOR  GREEN_YELLOW
#define BG_COLOR  DODGER_BLUE
#define CBG_COLOR  RGB(0x40, 0x40, 0xC0)

Display display;
bool screenEnabled = false;

Touch touch;

#define ROTATION ROTATION_VTB

Editable operBtn;
TextButton equalsBtn;

Container* E0T[]{
  &operBtn, &equalsBtn
};

FieldSet e0FSet(VERTICAL, 10, 2, 2, E0T, 2);

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

FieldSet r4FSet(HORIZONTAL, BTN_DIST, 2, 2, R4T, 5);

Container* R3T[]{
  &kLParBtn, &k07Btn, &k08Btn, &k09Btn, &kDivBtn
};

FieldSet r3FSet(HORIZONTAL, BTN_DIST, 2, 2, R3T, 5);

Container* R2T[]{
  &kRParBtn, &k04Btn, &k05Btn, &k06Btn, &kMulBtn
};

FieldSet r2FSet(HORIZONTAL, BTN_DIST, 2, 2, R2T, 5);

Container* R1T[]{
  &kModBtn, &k01Btn, &k02Btn, &k03Btn, &kSubBtn
};

FieldSet r1FSet(HORIZONTAL, BTN_DIST, 2, 2, R1T, 5);

Container* R0T[]{
  &kExcBtn, &kInsBtn, &k00Btn, &kDotBtn, &kAddBtn
};

FieldSet r0FSet(HORIZONTAL, BTN_DIST, 2, 2, R0T, 5);

Container* VCT[]{
  &r4FSet, &r3FSet, &r2FSet, &r1FSet, &r0FSet
};

FieldSet div01(VERTICAL, BTN_DIST-4, 2, 2, VCT, 5);

Container* CT[]{
  &e0FSet, &div01
};

FieldSet desktop(0, 0, ALIGN_CLIENT, ALIGN_CLIENT, CT, 2);

// ----------------------------------------------------------------

void calc_oper_btn(TextButton* b)
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
    operBtn.cmdInsChar(*b->getUtf8Text());
    break;
  }
  case 2:
    operBtn.moveCursor(+1);
    break;
  case 4:
    operBtn.moveCursor(-1);
    break;
  case -4:  // Ins
    operBtn.swapCursorInsMode();
    kInsBtn.setTextColor(operBtn.getCursorInsMode() ? WHITE : RED);
    break;
  case -5:  // Del
    operBtn.cmdDel();
    break;
  case -8:  // BS
    operBtn.cmdBS();
    break;
  case -3:  // CC
    operBtn.setStaticText("");
    equalsBtn.setStaticText("");
    break;
  case -7:  // =
    eval_t eval;
    equalsBtn.setText(eval.eval(operBtn.getUtf8Text()));
    break;
  }
  desktop.draw();
}

void calc_btn(TextButton* b, const int id, const char* text)
{
  b->setId(id);
  b->setStaticText(text);
  int BTN_SIZE = display.width() / 7 + 2;
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

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

void setup() {

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

  calc_oper_btn(&operBtn);
  calc_oper_btn(&equalsBtn);

  calc_row(&r4FSet);
  calc_row(&r3FSet);
  calc_row(&r2FSet);
  calc_row(&r1FSet);
  calc_row(&r0FSet);

  e0FSet.setAlignClientHoriz();
  e0FSet.setTransparent(true);
  e0FSet.setBorderSize(0);

//  r4FSet.setAlignClientHoriz();
//  r3FSet.setAlignClientHoriz();
//  r2FSet.setAlignClientHoriz();
//  r1FSet.setAlignClientHoriz();
//  r0FSet.setAlignClientHoriz();

  r4FSet.setBorderSize(0);
  r3FSet.setBorderSize(0);
  r2FSet.setBorderSize(0);
  r1FSet.setBorderSize(0);
  r0FSet.setBorderSize(0);

//  div01.setAlignClient();
  div01.setTransparent(true);
  div01.setBorderSize(0);

  desktop.setBorderSize(0);
  desktop.setBackground(CBG_COLOR);
  desktop.setOrientation(VERTICAL, 0);

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

// ----------------------------------------------------------------
