/*
  CheckBoxes on TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include <Media.h>
#include <TSDesktop.h>
#include <Fonts/TSD_FreeSerif_18pt.h>

#include <TSD_PCF8575.h>

#define GRAY_A0  RGB(0xA0, 0xA0, 0xA0)

Display display;
bool screenEnabled = false;

Touch touch;

TSD_PCF8575 pcf8575(1);

#define ROTATION ROTATION_HLR

CheckBox box100;  // attached to pcf8575
CheckBox box101;  // attached to pcf8575
CheckBox box102;  // attached to pcf8575
CheckBox box103;  // attached to pcf8575
CheckBox box104;  // attached to pcf8575
CheckBox box105;  // attached to pcf8575
CheckBox box106;  // attached to pcf8575
CheckBox box107;  // attached to pcf8575
CheckBox box108;
CheckBox box109;

Container* LGT[]{
  &box100,
  &box101,
  &box102,
  &box103,
  &box104,
  &box105,
  &box106,
  &box107,
  &box108,
  &box109
};

FieldSet left(VERTICAL, 0, 0, 0, LGT, 10);

CheckBox box200;  // attached to pcf8575
CheckBox box201;  // attached to pcf8575
CheckBox box202;  // attached to pcf8575
CheckBox box203;  // attached to pcf8575
CheckBox box204;  // attached to pcf8575
CheckBox box205;  // attached to pcf8575
CheckBox box206;  // attached to pcf8575
CheckBox box207;  // attached to pcf8575
CheckBox box208;
CheckBox box209;

Container* RGT[]{
  &box200,
  &box201,
  &box202,
  &box203,
  &box204,
  &box205,
  &box206,
  &box207,
  &box208,
  &box209
};

FieldSet right(VERTICAL, 0, 0, 0, RGT, 10);

CheckBox box300;
CheckBox box301;
CheckBox box302;
CheckBox box303;
CheckBox box304;
CheckBox box305;
CheckBox box306;
CheckBox box307;
CheckBox box308;
CheckBox box309;

Container* TGT[]{
  &box300,
  &box301,
  &box302,
  &box303,
  &box304,
  &box305,
  &box306,
  &box307,
  &box308,
  &box309
};

FieldSet top(HORIZONTAL, 0, 0, 0, TGT, 10);

CheckBox box400;
CheckBox box401;
CheckBox box402;
CheckBox box403;
CheckBox box404;
CheckBox box405;
CheckBox box406;
CheckBox box407;
CheckBox box408;
CheckBox box409;

Container* BGT[]{
  &box400,
  &box401,
  &box402,
  &box403,
  &box404,
  &box405,
  &box406,
  &box407,
  &box408,
  &box409
};

FieldSet bottom(HORIZONTAL, 0, 0, 0, BGT, 10);

TextButton logo;
Container* LOGOT[]{
  &logo
};

FieldSet tcentb(VERTICAL, 0, 0, 0, LOGOT, 1);

Container* TCBT[]{
  &top, &tcentb, &bottom
};

FieldSet center(VERTICAL, 0, 0, 0, TCBT, 3);

Container* LCRT[]{
  &left, &center, &right
};

FieldSet desktop(0, 0, ALIGN_CLIENT, ALIGN_CLIENT, LCRT, 3);

// ----------------------------------------------------------------
void refreshGates()
{
  uint16_t state = 0;
  for (int i = 0; i < 8; ++i) {
    state = (state << 1) | (static_cast<CheckBox*>(LGT[i])->getChecked() ? 1 : 0);
  }
  for (int i = 0; i < 8; ++i) {
    state = (state << 1) | (static_cast<CheckBox*>(RGT[i])->getChecked() ? 1 : 0);
  }
  pcf8575.write16(~state);
}

void btn_click(Container* c)
{
  int id = c->getId();

  if ((id >= 100 && id < 108) || (id >= 200 && id < 208)) {
    refreshGates();
  }
}

void lchk_box(CheckBox* c, const int16_t id, const char* text)
{
  c->setId(100 + id);
  c->setStaticText(text);
  c->setTextColor(GRAY_A0);
  c->setDecorPos(DECOR_POS_RIGHT);
  c->setOnClick(btn_click);
  if (id >= 0 && id < 8) {
    c->setDecorActiveColor(RED);
  }
}

void rchk_box(CheckBox* c, const int16_t id, const char* text)
{
  c->setId(200 + id);
  c->setStaticText(text);
  c->setTextColor(GRAY_A0);
  c->setOnClick(btn_click);
  if (id >= 0 && id < 8) {
    c->setDecorActiveColor(RED);
  }
}

void tchk_box(CheckBox* c, const int16_t id, const char* text)
{
  c->setId(300 + id);
  c->setStaticText(text);
  c->setTextColor(GRAY_A0);
  c->setDecorAlign(DECOR_ALIGN_BOTTOM);
  c->setDecorPos(DECOR_POS_CENTER);
  c->setOnClick(btn_click);
}

void bchk_box(CheckBox* c, const int16_t id, const char* text)
{
  c->setId(400 + id);
  c->setStaticText(text);
  c->setTextColor(GRAY_A0);
  c->setDecorAlign(DECOR_ALIGN_TOP);
  c->setDecorPos(DECOR_POS_CENTER);
  c->setOnClick(btn_click);
}

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

void setup() {

  pcf8575.begin();

  media_begin(ROTATION);

  pinMode(LED_PIN, OUTPUT);

  logo.setStaticText("TSDesktop\n\nTouch\nScreen\nDesktop");
  logo.setFont(FreeSerif_18pt);
  logo.setTextAlign(TEXT_ALIGN_CENTER);
  logo.setTextColor(GRAY);
  logo.setBorderSize(0);
  logo.setTransparent(true);

  top.setAlignClientHoriz();
  top.setTransparent(true);
  tcentb.setAlignClient();
  tcentb.setAlignCenter(true);
  tcentb.setTransparent(true);
  bottom.setAlignClientHoriz();
  bottom.setTransparent(true);

  left.setAlignClientVert();
  left.setTransparent(true);
  center.setAlignClient();
  center.setTransparent(true);
  right.setAlignClientVert();
  right.setTransparent(true);

  lchk_box(&box100, 0, "0");
  lchk_box(&box101, 1, "1");
  lchk_box(&box102, 2, "2");
  lchk_box(&box103, 3, "3");
  lchk_box(&box104, 4, "4");
  lchk_box(&box105, 5, "5");
  lchk_box(&box106, 6, "6");
  lchk_box(&box107, 7, "7");
  lchk_box(&box108, 8, "8");
  lchk_box(&box109, 9, "9");

  rchk_box(&box200, 0, "0");
  rchk_box(&box201, 1, "1");
  rchk_box(&box202, 2, "2");
  rchk_box(&box203, 3, "3");
  rchk_box(&box204, 4, "4");
  rchk_box(&box205, 5, "5");
  rchk_box(&box206, 6, "6");
  rchk_box(&box207, 7, "7");
  rchk_box(&box208, 8, "8");
  rchk_box(&box209, 9, "9");

  tchk_box(&box300, 0, "0.");
  tchk_box(&box301, 1, "1.");
  tchk_box(&box302, 2, "2.");
  tchk_box(&box303, 3, "3.");
  tchk_box(&box304, 4, "4.");
  tchk_box(&box305, 5, "5.");
  tchk_box(&box306, 6, "6.");
  tchk_box(&box307, 7, "7.");
  tchk_box(&box308, 8, "8.");
  tchk_box(&box309, 9, "9.");

  bchk_box(&box400, 0, "0.");
  bchk_box(&box401, 1, "1.");
  bchk_box(&box402, 2, "2.");
  bchk_box(&box403, 3, "3.");
  bchk_box(&box404, 4, "4.");
  bchk_box(&box405, 5, "5.");
  bchk_box(&box406, 6, "6.");
  bchk_box(&box407, 7, "7.");
  bchk_box(&box408, 8, "8.");
  bchk_box(&box409, 9, "9.");

//  desktop.setBackground(AQUA);
  gradient_t g1{GNT_LR, AQUA, 80, BLUE};
  desktop.setGradient(&g1);
  desktop.setOrientation(HORIZONTAL, 2);

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

  stdio_init_all();

  setup();

  while (true) {
    loop();
  }
}

// ----------------------------------------------------------------
