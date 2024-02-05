/*
  Programmer project on TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include <Media.h>
#include <TSDesktop.h>

#include <TSD_DS3231.h>
#include <TSD_PCF8575.h>
#include "Pgms.h"

#include <Fonts/TSD_FreeSans_9pt.h>

#define FONT FreeSans_9pt

#define GRAY_88  RGB( 0x88, 0x88, 0x88)
#define BG_COLOR OLIVE

#define ROTATION ROTATION_VTB

Display display;
bool screenEnabled = false;

Touch touch;

from_to_t PGMS[N_OF_PGMS];

TSD_DS3231 ds3231;
TSD_PCF8575 pcf8575(1);

dttm_t tm;
dttm_t oldDT;

// --------------------- Date & Time ---------------------------------
// --------------------- Date ----------------------------------------
TextButton dtDowBtn;
ValueButton dtYearBtn;
TextButton dtDist1Btn;
ValueButton dtMonBtn;
TextButton dtDist2Btn;
ValueButton dtDayBtn;

Container* TDATET[]{
  &dtDowBtn, &dtYearBtn, &dtDist1Btn, &dtMonBtn, &dtDist2Btn, &dtDayBtn
};

FieldSet tdateFSet(HORIZONTAL, 4, 4, 4, TDATET, 6, ORANGE);

// --------------------- Time ----------------------------------------
ValueButton tmHourBtn;
TextButton tmDist1Btn;
ValueButton tmMinBtn;
TextButton tmDist2Btn;
ValueButton tmSecBtn;

Container* TTIMET[]{
  &tmHourBtn, &tmDist1Btn, &tmMinBtn, &tmDist2Btn, &tmSecBtn
};

FieldSet ttimeFSet(HORIZONTAL, 4, 4, 4, TTIMET, 5, BLUE);

// --------------------- Date & Time ---------------------------------
Container* DTTMT[]{
  &tdateFSet,
  &ttimeFSet
};

FieldSet dttmFSet = FieldSet(VERTICAL, 8, 0, 4, DTTMT, 2, DARK_CYAN);

// --------------------- Aging ---------------------------------------
TextButton agingName;
ValueButton agingBtn;

Container* AGNGT[]{
  &agingName,
  &agingBtn
};

FieldSet agngFSet(HORIZONTAL, 20, 20, 10, AGNGT, 2, DARK_CYAN);

// --------------------- 8 Times programmer----------------------------
// ----------Programmer RadioButtons selector ------------------------
RadioButton r1Btn(1, "1");
RadioButton r2Btn(2, "2");
RadioButton r3Btn(3, "3");
RadioButton r4Btn(4, "4");
RadioButton r5Btn(5, "5");
RadioButton r6Btn(6, "6");
RadioButton r7Btn(7, "7");
RadioButton r8Btn(8, "8");

Container* RPGMT[]{
  &r1Btn, &r2Btn, &r3Btn, &r4Btn, &r5Btn, &r6Btn, &r7Btn, &r8Btn
};

// has activeRadio
FieldSet rpgmFSet(VERTICAL, 0, 0, 0, RPGMT, 8, YELLOW);

// --------------------- Time From -----------------------------------
ValueButton frHourBtn;
TextButton frSepBtn;
ValueButton frMinBtn;

Container* FRPGMT[]{
  &frHourBtn, &frSepBtn, &frMinBtn
};

FieldSet frPgmFSet(HORIZONTAL, 2, 4, 4, FRPGMT, 3, GRAY);

// --------------------- Time To -------------------------------------
ValueButton toHourBtn;
TextButton toSepBtn;
ValueButton toMinBtn;

Container* TOPGMT[]{
  &toHourBtn, &toSepBtn, &toMinBtn
};

FieldSet toPgmFSet(HORIZONTAL, 2, 4, 4, TOPGMT, 3, GRAY);

// --------------------- Time From & To ------------------------------
Container* TPGMT[]{
  &frPgmFSet,
  &toPgmFSet
};

FieldSet tpgmFSet(VERTICAL, 10, 0, 0, TPGMT, 2, BLUE);

// --------------------- 8 Times programer ---------------------------
Container* PGMT[]{
  &rpgmFSet,
  &tpgmFSet
};

FieldSet pgmFSet(HORIZONTAL, 20, 20, 10, PGMT, 2, DARK_CYAN);

// ------------- Desktop operational center --------------------------
Container* D1T[]{
  &dttmFSet,
  &agngFSet,
  &pgmFSet
};

FieldSet d1FSet = FieldSet(VERTICAL, 6, 0, 4, D1T, 3, VIOLET, RED);

// ----------------- Down main buttons -------------------------------
TextButton mainBtn;
TextButton pgmDistBtn;
TextButton prevBtn;
TextButton nextBtn;

Container* D2T[]{
  &mainBtn, &pgmDistBtn, &prevBtn, &nextBtn
};

FieldSet d2FSet(HORIZONTAL, 10, 10, 4, D2T, 4, NAVY, GRAY);

// ------------------------- Desktop ---------------------------------
Container* DESKTOPT[]{
  &d1FSet,
  &d2FSet
};

//FieldSet desktop(0, 10, display.width(), display.height() - 10, DESKTOPT, 2, BG_COLOR);
FieldSet desktop(0, 0, ALIGN_CLIENT, ALIGN_CLIENT, DESKTOPT, 2, BG_COLOR);

// ------------------------- ^^^^^^ ---end of objects-----------------

// ----------------------------- Services ----------------------------

void dtdist_btn(TextButton* b, const char* text, const int fontSize)
{
  b->setStaticText(text);
  b->setFontSize(fontSize);
  b->setTextColor(GRAY);
  b->setBorderSize(0);
  b->setDisabled();
}

// ----------------------------- Date & Time Service -----------------
// selected cell in edition
Button* selectedDTBtn = NULL;

Button* select_click(Container* b, Button* selected)
{
  if (selected) {
    selected->setBorderColor(GRAY);
    selected = NULL;
  }
  if (b) {
    selected = static_cast<Button*>(b);
    selected->setBorderColor(RED);
  }
  prevBtn.setVisible(b);
  nextBtn.setVisible(b);
  return selected;
}

void dttm_select_click(Container* b)
{
  selectedDTBtn = select_click(b, selectedDTBtn);
}

void dttm_btn(TextButton* b, const int id, const int fontSize)
{
  b->setId(id);
  b->setOnClick(dttm_select_click);
  b->setTextColor(GRAY_88);
  b->setFont(FONT);
  b->setFontSize(fontSize);
//  b->setFontSize(fontSize + 1);
  b->setBorderColor(GRAY);
}

void dt_btn(ValueButton* b, const int id, const char* valueFormat = "%02d")
{
  dttm_btn(static_cast<TextButton*>(b), id, 1);
  b->setValueFormat(valueFormat);
}

void tm_btn(ValueButton* b, const int id)
{
  dttm_btn(static_cast<TextButton*>(b), id, 2);
  b->setValueFormat("%02d");
}

void enable_dttm(const bool enable)
{
  dttm_select_click(NULL);
  dttmFSet.setBorderColor(enable ? WHITE : GRAY);
  dtDowBtn.setDisabled(!enable);
  dtYearBtn.setDisabled(!enable);
  dtMonBtn.setDisabled(!enable);
  dtDayBtn.setDisabled(!enable);
  tmHourBtn.setDisabled(!enable);
  tmMinBtn.setDisabled(!enable);
  tmSecBtn.setDisabled(!enable);
  agngFSet.setHidden(!enable);
  agingBtn.setDisabled(!enable);
}

void selected_dttm_up(const int up)
{
  if (selectedDTBtn) {
    int what = selectedDTBtn->getId();
    int value = ds3231.up(&tm, what, up);
    if (selectedDTBtn == &dtDowBtn) {
      dtDowBtn.setStaticText(dow(value));
    }
    else {
      static_cast<ValueButton*>(selectedDTBtn)->setValue(value);
    }
    memcpy(&oldDT, &tm, sizeof(oldDT));
  }
}

void minimize_time(const bool smallTime)
{
  tdateFSet.setHidden(smallTime);
  ttimeFSet.setAlignCompactHoriz();
  dttmFSet.setAlignCompactHoriz();
  ttimeFSet.setOrientation(smallTime && (ROTATION & 1) ? VERTICAL : HORIZONTAL, 4);
  d1FSet.setOrientation(smallTime && (ROTATION & 1) ? HORIZONTAL : VERTICAL, 8);
  tmHourBtn.setFontSize(smallTime ? 1 : 2);
  tmMinBtn.setFontSize(smallTime ? 1 : 2);
  tmSecBtn.setFontSize(smallTime ? 1 : 2);
}

// --------------------------- Programmer Service --------------------

void pgm_radio_click(Container* c)
{
  int tag = c->getTag();
  from_to_t* p = &PGMS[tag - 1];
  frHourBtn.setValue(p->start.hour);
  frMinBtn.setValue(p->start.min);
  toHourBtn.setValue(p->stop.hour);
  toMinBtn.setValue(p->stop.min);
}

void rpgm_radio_btn(RadioButton* b)
{
  b->setMargin(4);
  b->setTextColor(GRAY);
  b->setBorderSize(0);
  b->setDecorInActiveColor(GRAY);
  b->setOnClick(pgm_radio_click);
}

// selected cell in edition
Button* selectedPgmBtn = NULL;

void pgm_select_click(Container* b)
{
  selectedPgmBtn = select_click(b, selectedPgmBtn);
}

void tpgm_btn(ValueButton* b, const int id)
{
  b->setId(id);
  b->setOnClick(pgm_select_click);
  b->setTextColor(GRAY_88);
  b->setFont(FONT);
  b->setFontSize(2);
  b->setBorderColor(GRAY);
  b->setDisabled();
  b->setText("  ");
  b->setValueFormat("%02d");
}

bool pgmsChanged = false;

void enable_pgm(const bool enable)
{
  if (pgmsChanged) {
    storePgmsInEEProm(PGMS);
    pgmsChanged = false;
  }
  rpgmFSet.setActiveRadio(NULL);
  pgm_select_click(NULL);
  pgmFSet.setHidden(!enable);
  frHourBtn.setDisabled(!enable);
  frHourBtn.setText("  ");
  frMinBtn.setDisabled(!enable);
  frMinBtn.setText("  ");
  toHourBtn.setDisabled(!enable);
  toHourBtn.setText("  ");
  toMinBtn.setDisabled(!enable);
  toMinBtn.setText("  ");
}

void selected_pgm_up(const int up)
{
  if (selectedPgmBtn) {
    RadioButton* r = rpgmFSet.getActiveRadio();
    if (r) {
      static_cast<ValueButton*>(selectedPgmBtn)->setValue(pgm_value_up(PGMS, r->getTag(), selectedPgmBtn->getId(), up));
      pgmsChanged = true;
    }
  }
}

// ------------------------- Main buttons Service --------------------

void main_btn(TextButton* b, const int tag, const char* aText, void(*onClick)(Container* c))
{
  b->setTag(tag);
  b->setStaticText(aText);
  b->setRadius(5);
//  b->setBorderSize(2);
  b->setFontSize(2);
  b->setTextColor(YELLOW);
  b->setBackground(GRAY);
  b->setBorderColor(MAGENTA);
  int BTNSIZE = 40;
  b->setSize(BTNSIZE, BTNSIZE);
  b->setOnClick(onClick);
  b->setEnabled();
}

void prev_next_click(Container* c)
{
  int up = c->getTag();
  selected_dttm_up(up);
  selected_pgm_up(up);
}

// ----------------------------- Main -----------------------------

int mainMode = 0;

void main_click(Container* c)
{
  mainMode = (mainMode + 1) % 3;
  enable_dttm(mainMode == 1);
  minimize_time(mainMode == 2);
  enable_pgm(mainMode == 2);
  desktop.draw(true);
}

// ----------------------------------------------------------------

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

void setup() {

  ds3231.begin();
  pcf8575.begin();

  media_begin(ROTATION);

  pinMode(LED_PIN, OUTPUT);

  tdateFSet.setBorderSize(0);
  tdateFSet.setTransparent(true);
  dttm_btn(&dtDowBtn, 1, 1);
  dt_btn(&dtYearBtn, 2, "%04d");
  dtdist_btn(&dtDist1Btn, "-", 1);
  dt_btn(&dtMonBtn, 3);
  dtdist_btn(&dtDist2Btn, "-", 1);
  dt_btn(&dtDayBtn, 4);

  ttimeFSet.setBorderSize(0);
  ttimeFSet.setTransparent(true);
  tm_btn(&tmHourBtn, 5);
  dtdist_btn(&tmDist1Btn, ":", 2);
  tm_btn(&tmMinBtn, 6);
  dtdist_btn(&tmDist2Btn, ":", 2);
  tm_btn(&tmSecBtn, 7);
  dt_btn(&agingBtn, 8);

  agingName.setStaticText("aging");
  agingName.setTextColor(BLACK);
  agingName.setFont(FONT);
//  agingName.setFontSize(1);
  agingName.setBorderSize(0);
  agingName.setTransparent(true);
  agingName.setBottomMargin(4);   // for gg visibility

  agngFSet.setHidden(true);

  rpgmFSet.setBorderSize(0);
  rpgm_radio_btn(&r1Btn);
  rpgm_radio_btn(&r2Btn);
  rpgm_radio_btn(&r3Btn);
  rpgm_radio_btn(&r4Btn);
  rpgm_radio_btn(&r5Btn);
  rpgm_radio_btn(&r6Btn);
  rpgm_radio_btn(&r7Btn);
  rpgm_radio_btn(&r8Btn);

  tpgm_btn(&frHourBtn, 1);
  dtdist_btn(&frSepBtn, ":", 2);
  tpgm_btn(&frMinBtn, 2);
  tpgm_btn(&toHourBtn, 3);
  dtdist_btn(&toSepBtn, ":", 2);
  tpgm_btn(&toMinBtn, 4);

  d1FSet.setTransparent(true);
  d1FSet.setAlignClient();

  enable_dttm(false);
  enable_pgm(false);

  pgmDistBtn.setVisible(false);

  main_btn(&mainBtn, 0, "*", main_click);
  main_btn(&pgmDistBtn, 0, "^", NULL);
  main_btn(&prevBtn, -1, "<", prev_next_click);
  main_btn(&nextBtn, 1, ">", prev_next_click);

  d2FSet.setAlignCenterHoriz(true);
// d2FSet.setTransparent(true);

  desktop.setAlignCenter(true);
  desktop.setAlignBottom(true);
  desktop.setOrientation(VERTICAL, 10);
  desktop.setMargin(2, 2, 2, 10);
//  desktop.setTransparent(true);

  gradient_t g1{GNT_LR, OLIVE, 100, YELLOW};
  desktop.setGradient(&g1);

  readPgmsFromEEProm(PGMS);

  screenEnabled = true;
  desktop.draw();
}

// ----------------------------------------------------------------

void updateDate()
{
  dtDowBtn.setStaticText(dow(tm.dow));
  dtYearBtn.setValue(2000 + tm.year);
  dtMonBtn.setValue(tm.month);
  dtDayBtn.setValue(tm.day);

  tmHourBtn.setValue(tm.hour);
  tmMinBtn.setValue(tm.min);
  tmSecBtn.setValue(tm.sec);

  agingBtn.setValue(ds3231.getAging(&tm));
}

// ----------------------------------------------------------------

void loop()
{
  ds3231.getDateTime(&tm);

  if (!dttm_eq(tm, oldDT)) {
    updateDate();
    memcpy(&oldDT, &tm, sizeof(oldDT));
    desktop.draw();
  }

  onOffPgms(&pcf8575, PGMS, tm);

  point_t p;
  if (touch.getTouch(&p)) {
    /*Container* b =*/ desktop.pressed(p.x, p.y);
  }

  delay(50);

  pin13state = 1 - pin13state;
  digitalWrite(LED_PIN, pin13state);
}
// ----------------------------------------------------------------
