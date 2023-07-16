/*
  TouchScreenDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TSD_SCREEN.h>

extern bool screenEnabled;

#define ALIGN_CLIENT  -101
#define ALIGN_COMPACT -102

#define NO_BACKGROUND_COLOR 1

typedef struct {
  uint8_t type;
} container_t;

#define TYPE_CONTAINER   container_t{0}
#define TYPE_BUTTON      container_t{1}
#define TYPE_TEXTBUTTON  container_t{2}
#define TYPE_CHECKBOX    container_t{3}
#define TYPE_RADIOBUTTON container_t{4}
#define TYPE_FIELDSET    container_t{5}
#define TYPE_GFXBUTTON   container_t{6}

enum TEXT_ALIGN {
  TEXT_ALIGN_LEFT = 0,
  TEXT_ALIGN_RIGHT = 1,
  TEXT_ALIGN_CENTER = 2
};

enum DECOR_ALIGN {
  DECOR_ALIGN_NONE = 0,
  DECOR_ALIGN_TOP = 1,
  DECOR_ALIGN_BOTTOM = 2
};

enum DECOR_POS {
  DECOR_POS_LEFT = 0,
  DECOR_POS_CENTER = 1,     // if TOP or BOTTOM
  DECOR_POS_RIGHT = 2
};

typedef struct {
  bool vertical;
} orient_t;

#define  HORIZONTAL orient_t{false}
#define  VERTICAL orient_t{true}

// which scroller
#define SCROLL_STEP 0x01
#define SCROLL_PAGE 0x02
#define SCROLL_HOME 0x04
#define SCROLL_PROGRESS 0x08

/// @Container

class Container {
public:
  Container(
    container_t aType,
    const int16_t aLeft,
    const int16_t aTop,
    const int16_t aWidth,
    const int16_t aHeight)
    : type(aType), orgLeft(aLeft), orgTop(aTop), orgWidth(aWidth), orgHeight(aHeight)
  {
    parent = NULL;
    updLeft = aLeft >= 0 ? aLeft : 0;
    updTop = aTop >= 0 ? aTop : 0;
    updWidth = aWidth >= 0 ? aWidth : 10;
    updHeight = aHeight >= 0 ? aHeight : 10;
  }
  // user specified tags
  void setId(const int16_t aId) { id = aId; }
  void setGroupId(const int16_t aGroupId) { groupId = aGroupId; }
  void setTag(const int16_t aTag) { tag = aTag; }  // used by RadioButton
  void setName(const char* aName) { name = aName; }

  const int16_t getId() { return id; }
  const int16_t getGroupId() { return groupId; }
  const int16_t getTag() { return tag; }  // used by RadioButton
  const char* getName() { return name; }
  //
  Container* getParent() { return parent; }

  container_t getType() { return type; }

  void setVisible(const bool aVisible);
  void setHidden(const bool aHidden);
  void setDisabled(const bool aDisabled) { disabled = aDisabled; }
  void setDisabled() { setDisabled(true); }
  void setEnabled() { setDisabled(false); };

  void setMargin(const uint8_t aLeft, const uint8_t aTop, const uint8_t aRight, const uint8_t aBottom);
  void setMargin(const uint8_t aMargin)
  {
    setMargin(aMargin, aMargin, aMargin, aMargin);
  }
  void setMargin(const uint8_t aMarginHoriz, const uint8_t aMarginVert)
  {
    setMargin(aMarginHoriz, aMarginVert, aMarginHoriz, aMarginVert);
  }
  void setLeftMargin(const uint8_t aLeftMargin);
  void setTopMargin(const uint8_t aTopMargin);
  void setRightMargin(const uint8_t aRightMargin);
  void setBottomMargin(const uint8_t aBottomMargin);

  void setAlignLeft(const bool aAlignLeft);
  void setAlignTop(const bool aAlignTop);
  void setAlignRight(const bool aAlignRight);
  void setAlignBottom(const bool aAlignBottom);
  void setAlignCenter(const bool aAlignCenter);
  void setAlignCenterHoriz(const bool aAlignCenterHoriz);
  void setAlignCenterVert(const bool aAlignCenterVert);
  void setAlignClient();
  void setAlignClientHoriz();
  void setAlignClientVert();
  void setAlignCompact();
  void setAlignCompactHoriz();
  void setAlignCompactVert();

  void setLeft(const int16_t aLeft);
  void setTop(const int16_t aTop);
  void setPos(const int16_t aLeft, const int16_t aTop)
  {
    setLeft(aLeft);
    setTop(aTop);
  }
  void setWidth(const int16_t aWidth);
  void setHeight(const int16_t aHeight);
  void setSize(const int16_t aWidth, const int16_t aHeight)
  {
    setWidth(aWidth);
    setHeight(aHeight);
  }
  void setCoord(const int16_t aLeft, const int16_t aTop, const int16_t aWidth, const int16_t aHeight)
  {
    setPos(aLeft, aTop);
    setSize(aWidth, aHeight);
  }

  const bool getVisible() { return visible; }
  const bool getHidden() { return hidden; }
  const bool getDisabled() { return disabled; }
  const bool getEnabled() { return !disabled; }

  const uint8_t getLeftMargin() { return marginLeft; }
  const uint8_t getTopMargin() { return marginTop; }
  const uint8_t getRightMargin() { return marginRight; }
  const uint8_t getBottomMargin() { return marginBottom; }

  const bool getAlignLeft() { return alignLeft; }
  const bool getAlignRight() { return alignRight; }
  const bool getAlignTop() { return alignTop; }
  const bool getAlignBottom() { return alignBottom; }
  const bool getAlignCenterHoriz() { return alignCenterHoriz; }
  const bool getAlignCenterVert() { return alignCenterVert; }

  const bool getAlignClientHoriz() { return orgWidth == ALIGN_CLIENT; }
  const bool getAlignCompactHoriz() { return orgWidth == ALIGN_COMPACT; }
  const bool getAlignClientVert() { return orgHeight == ALIGN_CLIENT; }
  const bool getAlignCompactVert() { return orgHeight == ALIGN_COMPACT; }

  const int16_t getOrgLeft() { return orgLeft; }
  const int16_t getOrgTop() { return orgTop; }
  const int16_t getOrgWidth() { return orgWidth; }
  const int16_t getOrgHeight() { return orgHeight; }

  const int16_t getUpdLeft() { return updLeft; }
  const int16_t getUpdTop() { return updTop; }
  const int16_t getUpdWidth() { return updWidth; }
  const int16_t getUpdHeight() { return updHeight; }

  // absolute position of container - offsetLeft
  const int16_t getAbsOuterLeft();
  // absolute position of container - offsetTop
  const int16_t getAbsOuterTop();

  // absolute position in container + marginLeft - offsetLeft
  virtual const int16_t getAbsInnerLeft(const int16_t pos);

  // absolute position in container + marginTop - offsetTop
  virtual const int16_t getAbsInnerTop(const int16_t pos);

  const bool getAbsVisible();     // is container finally visible

  virtual void draw(const bool redraw) = 0;

  virtual void hide() { setNotWasDrawn(); }

  void setOnClick(void (*aFuncOnClick)(Container* aContainer)) { funcOnClick = aFuncOnClick; }

  // 0 - no efffect
  // 1 - default:
  //      draw border (and text) in a clickHighlightColor
  //      delay a clickHighlightDelay
  //      restore border (and text) color
  void setClickEffect(const int16_t aClickEffect) { clickEffect_ = aClickEffect; }

  // default is 100
  void setClickHighlightDelay(const int16_t aClickHighlightDelay) { clickHighlightDelay = aClickHighlightDelay; }

  virtual const rgb_t getBackgroundColor();    // or NO_BACKGROUND_COLOR
  const rgb_t getBackground() { return getBackgroundColor(); }

  virtual const bool isBorderRadius() { return false; }
  const uint8_t getBorderSize() { return borderSize; }

  virtual Container* pressed(const int16_t xScreen, const int16_t yScreen);

  virtual void clickEffect(const int16_t posX, const int16_t posY) {}

  void scroll(const uint8_t tp, const bool up, const bool vert);
  void drawScroller();
  const bool scrollerPressed(const int16_t xScreen, const int16_t yScreen);

  void disableScroll(const uint8_t scrollBtn) { scroll_btns &= ~scrollBtn; }
  void enableScroll(const uint8_t scrollBtn) { scroll_btns |= scrollBtn; }

  const int16_t getClipLeft(int16_t l);
  const int16_t getClipTop(int16_t t);
  const int16_t getClipRight(int16_t r, int16_t m2);
  const int16_t getClipBottom(int16_t b, int16_t m2);

  const int16_t getOffsetLeft() { return offsetLeft; }
  const int16_t getOffsetTop() { return offsetTop; }

  clip_t* getInnerClip(clip_t& clip);
  clip_t* getOuterClip(clip_t& clip);

//  clip_t* getClip(clip_t& clip);

  virtual void setNotWasDrawn() {}
  virtual TSD_SCREEN* screen();

  friend class Button;
  friend class TextButton;
  friend class FieldSet;

private:
  void setParent(const Container* aParent);

  virtual void updCompactWidth(const bool recalc) {}
  virtual void updCompactHeight(const bool recalc) {}
  virtual void updateCoord(const bool recalc) { updated = true; }

  void setUpdLeft(const int16_t aLeft);
  void setUpdTop(const int16_t aTop);
  void setUpdPos(const int16_t aLeft, const int16_t aTop);
  bool setUpdWidth(const int16_t aWidth);
  bool setUpdHeight(const int16_t aHeight);

  int16_t updLeft, updTop, updWidth, updHeight;
  bool updated = false;

protected:
  void setChanged();

private:
  int16_t id = 0, groupId = 0;   // user defined
  int16_t tag = 0;  // used by RadioButton
  const char* name = NULL;   // user defined
  Container* parent = NULL;
  container_t type = TYPE_CONTAINER;
  int16_t orgLeft = 0, orgTop = 0;
  int16_t orgWidth = 0, orgHeight = 0;    // if < 0 : ALIGN_CLIENT, ALIGN_COMPACT
  uint8_t marginLeft = 0, marginTop = 0, marginRight = 0, marginBottom = 0;
  bool alignLeft = false, alignTop = false, alignRight = false, alignBottom = false, alignCenterHoriz = false, alignCenterVert = false;
  bool visible = true;
  bool hidden = false;
  bool disabled = true;
  //    bool absolutePos = false;
  uint8_t borderSize = 1;

  int16_t offsetLeft = 0;
  int16_t offsetTop = 0;

  uint8_t clickEffect_ = 1;
  int16_t clickHighlightDelay = 100;
  void (*funcOnClick)(Container* aContainer) = NULL;

  uint8_t scroll_btns = SCROLL_STEP | SCROLL_PAGE | SCROLL_HOME;	// | SCROLL_PROGRESS;
};


/// @Button

class Button : public Container {
public:
  Button()
    : Button(TYPE_BUTTON, -1, -1, ALIGN_CLIENT, ALIGN_CLIENT, BLACK, WHITE)
  {
  }

  Button(
    container_t aType,
    const int16_t aLeft,
    const int16_t aTop,
    const int16_t aWidth,
    const int16_t aHeight,
    rgb_t aBackgroundColor = BLACK,
    rgb_t aBorderColor = WHITE)
    : Container(aType, aLeft, aTop, aWidth, aHeight), backgroundColor(aBackgroundColor), borderColor(aBorderColor)
  {
    alignCenterHoriz = true;
    alignCenterVert = true;
    alignLeft = true;

    disabled = false;
  }

  Button(
    const int16_t aLeft,
    const int16_t aTop,
    const int16_t aWidth,
    const int16_t aHeight,
    rgb_t aBackgroundColor = BLACK,
    rgb_t aBorderColor = WHITE)
    : Button(TYPE_BUTTON, aLeft, aTop, aWidth, aHeight, aBackgroundColor, aBorderColor)
  {
  }

  void setRadius(const int8_t aRadius);
  const int8_t getRadius() { return radius; }

  void setTransparent(const bool aTransparent);
  const bool getTransparent() { return transparent; }

  void setBackgroundColor(const rgb_t aBackgroundColor);
  void setBackground(const rgb_t aBackgroundColor);
  const rgb_t getBackgroundColor() override;    // or NO_BACKGROUND_COLOR

  void setGradient(gradient_t* aGradient);
  gradient_t* getGradient() { return &gradient; }

  void setBorderSize(const uint8_t aBorderSize) { borderSize = aBorderSize; }
  void setBorderColor(const rgb_t aBorderColor);
  const rgb_t getBorderColor() { return borderColor; }
  const bool isBorderRadius() override { return radius > 0 && borderSize > 0; }

  void setClickHighlightColor(const rgb_t aClickHighlightColor) { clickHighlightColor = aClickHighlightColor; }
  const rgb_t getClickHighlightColor() { return clickHighlightColor; }

  virtual void innerDraw(const bool redraw) = 0;
  void draw(const bool redraw) override;
  void draw() { draw(false); }

  void hide() override;

  virtual void setNotWasDrawn() { wasDrawn = false; }

  friend class Container;
  friend class TextButton;
  friend class DecorTextButton;
  friend class FieldSet;

private:
  bool wasDrawn = false;

  void drawBackground(const rgb_t color);
  virtual void drawBackground();

  void drawBorder();
  void drawBorder(const rgb_t color);

  virtual void clickEffect(const int16_t posX, const int16_t posY);

private:
  bool transparent = false;
  int8_t radius = 0;
  rgb_t backgroundColor = 0;
  gradient_t gradient{0};
  rgb_t borderColor = 0;
  rgb_t clickHighlightColor = RED;
};


/// @TextButton

class TextButton : public Button {
public:
  TextButton()
    : TextButton("")
  {
  }

  TextButton(const char* aText)
    : TextButton(aText, -1, -1, ALIGN_COMPACT, ALIGN_COMPACT)
  {
  }

  TextButton(
    const char* aText,
    const int16_t aLeft,
    const int16_t aTop,
    const int16_t aWidth,
    const int16_t aHeight,
    const rgb_t aTextColor = WHITE,
    const rgb_t aBackgroundColor = BLACK,
    const rgb_t aBorderColor = WHITE)
    : Button(TYPE_TEXTBUTTON, aLeft, aTop, aWidth, aHeight, aBackgroundColor, aBorderColor),
    textp(aText), textColorh(aTextColor), textColorl(BLACK)
  {
    marginLeft = 2;
    marginTop = 2;
    marginRight = 2;
    marginBottom = 2;

	disabled = true;
  }

  TextButton(
    const char* aText,
    const int16_t aWidth,
    const int16_t aHeight)
    : TextButton(aText, -1, -1, aWidth, aHeight)
  {
  }

  TextButton(
    const int16_t aWidth,
    const int16_t aHeight)
    : TextButton("", -1, -1, aWidth, aHeight)
  {
  }

  friend class DecorTextButton;

private:
  TextButton(
    container_t aType,
    const char* aText,
    const rgb_t aTextColor = WHITE,
    const rgb_t aBackgroundColor = BLACK,
    const rgb_t aBorderColor = WHITE)
    : Button(aType, -1, -1, ALIGN_COMPACT, ALIGN_COMPACT, aBackgroundColor, aBorderColor),
    textp(aText), textColorh(aTextColor), textColorl(BLACK)
  {
    marginLeft = 2;
    marginTop = 2;
    marginRight = 2;
    marginBottom = 2;
  }

public:
  void setFont(const GFXfont** aFont = NULL);
  const GFXfont** getFont() { return font.getFont(); }


  /**
   * setText - safe mothod to set text,
   * text is moved to allocated memory
  */
  void setText(const char* aText) {
    setTempText((const void*)aText, false, true);
  }

  void setText(const uint16_t* aText) {
    setTempText((const void*)aText, true, true);
  }

  /**
   * setStaticText - for static, permanent texts, not from the stack,
   * this method avoids memmory allocation
  */
  void setStaticText(const char* aText) {
    setTempText((const void*)aText, false, false);
  }

  void setStaticText(const uint16_t* aText) {
    setTempText((const void*)aText, true, false);
  }

  void setTextCoord(const int16_t aLeft, const int16_t aTop);
  void setFontSize(const int8_t aFontSize);
  void setFontSize(const int8_t aFontSizeX, const int8_t aFontSizeY);
  void setTextColor(const rgb_t aTextColorh, const rgb_t atextColorl = BLACK);
  void setTextAlign(TEXT_ALIGN aTextAlign);

//  const int getText(char* buf, const int bufsize); not implemented yet
//  const int getText(uint16_t* buf, const int bufsize); not implemented yet

  /**
   * getUtf8Text
   *  returns utf-8 text if the internal representation is utf-8, else ""
  */
  const char* getUtf8Text() {
    return unicode ? "" : (const char*)textp;
  }

  /**
   * getUnicodeText
   *  returns unicode text if the internal representation is unicode, else ^0
  */
  const uint16_t* getUnicodeText() {
    static const uint16_t EMPTY_1 = 0;
    return unicode ? (const uint16_t*)textp : (const uint16_t*)&EMPTY_1;
  }

  /**
   * textToUtf8
   *  converts internally unicode to utf-8 (if needed) and returns utf-8 text
  */
  const char* textToUtf8();

  /**
   * textToUnicode
   *  converts internally utf-8 text to unicode (if needed) and returns unicode text
  */
  const uint16_t* textToUnicode();

  const int16_t getTextLeft() { return textLeft; }
  const int16_t getTextTop() { return textTop; }
  const int16_t getTextWidth();
  const int16_t getTextHeight();
  const int8_t getFontSizeX() { return font.fontSizeX; }
  const int8_t getFontSizeY() { return font.fontSizeY; }
  const rgb_t getTextColor() { return textColorh; }

  void drawBackground() override;
  void innerDraw(const bool redraw) override;

  friend class Editable;

protected:
  const void* reallocTextTo(const int size);

  virtual const uint8_t getTextMarginLeft() { return 0; }
  virtual const uint8_t getTextMarginRight() { return 0; }
  virtual const uint8_t getTextMarginTop() { return 0; }
  virtual const uint8_t getTextMarginBottom() { return 0; }

  void clickEffect(const int16_t posX, const int16_t posY) override;

  virtual void drawText(const rgb_t colorh, const rgb_t colorl);
  virtual void drawText();

  virtual void hideText();

private:
  void setTempText(const void* aText, const bool aUnicode, const bool temp);
  virtual void innerSetText(const void* aText, const bool aUnicode, const bool temp);
  void textUpdateCoord();

  void updateCoord(const bool recalc) override;

  void updCompactWidth(const bool recalc) override;
  void updCompactHeight(const bool recalc) override;

private:
  font_t font{ NULL, 1, 1 };
  static const uint16_t EMPTY_0 = 0;
  const void* textp = (const void *)&EMPTY_0;
  bool unicode = 0;
  int16_t allocated = 0;
  int16_t textLeft = 0, textTop = 0, textWidth = 0, textHeight = 0;
  rgb_t textColorh = 0, textColorl = 0;
  TEXT_ALIGN textAlign = TEXT_ALIGN_LEFT;
};


/// @ValueButton

class ValueButton : public TextButton {
public:
  ValueButton()
    : ValueButton(-1, -1, ALIGN_COMPACT, ALIGN_COMPACT, WHITE, BLACK, WHITE)
  {
  }

  ValueButton(
    const int16_t aLeft,
    const int16_t aTop,
    const int16_t aWidth,
    const int16_t aHeight,
    const rgb_t aTextColor = WHITE,
    const rgb_t aBackgroundColor = BLACK,
    const rgb_t aBorderColor = WHITE)
    : TextButton("", aLeft, aTop, aWidth, aHeight, aTextColor, aBackgroundColor, aBorderColor)
  {
  }

  ValueButton(
    const int16_t aWidth,
    const int16_t aHeight,
    const rgb_t aTextColor = WHITE,
    const rgb_t aBackgroundColor = BLACK,
    const rgb_t aBorderColor = WHITE)
    : ValueButton(-1, -1, aWidth, aHeight, aTextColor, aBackgroundColor, aBorderColor)
  {
  }

  virtual void setValue(const int aValue);
  virtual void setProp(const char* aName, const int aValue);
  virtual void setProp(const char* aName, const char* aValue);

  /*
    use permanent texts only, not from the stack
  */
  void setValueFormat(const char* aValueFormat) { valueFormat = aValueFormat; }  // TODO
  const char* getValueFormat() { return valueFormat; }

private:
  const char* valueFormat = "%d";
};


/// @DecoratedTextButton

class DecorTextButton : public TextButton {
public:
  DecorTextButton(
    container_t aType,
    const char* aText,
    const rgb_t aTextColor = WHITE,
    const rgb_t aBackgroundColor = BLACK,
    const rgb_t aBorderColor = WHITE)
    : TextButton(aType, aText, aTextColor, aBackgroundColor, aBorderColor)
  {
    activeColor = aTextColor;
    inActiveColor = GRAY;
  }

  void setDecorPos(const DECOR_POS aDecorPosMode);
  void setDecorAlign(const DECOR_ALIGN aDecorAlign);

  // margins should be set after above

  void setDecorMarginLeft(const uint8_t aDecorMarginLeft);
  void setDecorMarginRight(const uint8_t aDecorMarginRight);
  void setDecorMarginHoriz(const uint8_t aDecorMarginHoriz)
  {
    setDecorMarginLeft(aDecorMarginHoriz);
    setDecorMarginRight(aDecorMarginHoriz);
  }
  void setDecorMarginTop(const uint8_t aDecorMarginTop);
  void setDecorMarginBottom(const uint8_t aDecorMarginBottom);
  void setDecorMarginVert(const uint8_t aDecorMarginVert)
  {
    setDecorMarginTop(aDecorMarginVert);
    setDecorMarginBottom(aDecorMarginVert);
  }
  void setDecorMargin(const uint8_t aDecorMargin)
  {
    setDecorMarginHoriz(aDecorMargin);
    setDecorMarginVert(aDecorMargin);
  }

  virtual const bool toggle() { return false; }
  void clickEffect(const int16_t posX, const int16_t posY) override;

  void setDecorActiveColor(const rgb_t aActiveColor);
  void setDecorInActiveColor(const rgb_t aInActiveColor);

  const DECOR_POS getDecorPos() { return decorPosMode; }
  const DECOR_ALIGN getDecorAlign() { return decorAlign; }

  const uint8_t getTextMarginLeft() override;
  const uint8_t getTextMarginRight() override;
  const uint8_t getTextMarginTop() override;
  const uint8_t getTextMarginBottom() override;

  virtual const int8_t getDecorWidth() = 0;
  virtual const int8_t getDecorHeight() = 0;

  virtual const int16_t getDecorPosLeft();
  virtual const int16_t getDecorPosTop();

  virtual void drawDecor() = 0;

  virtual void hideDecor() = 0;

  void drawBackground() override { Button::drawBackground(); }
  void innerDraw(const bool redraw) override;

  friend class CheckBox;
  friend class RadioButton;

private:
  DECOR_POS decorPosMode = DECOR_POS_LEFT;
  DECOR_ALIGN decorAlign = DECOR_ALIGN_NONE;
  uint8_t decorMarginLeft = 2, decorMarginRight = 10;
  uint8_t decorMarginTop, decorMarginBottom = 0;
  rgb_t inActiveColor, activeColor;
};


/// @CheckBox

class CheckBox : public DecorTextButton {
public:
  CheckBox()
    : CheckBox("")
  {
  }

  CheckBox(
    const char* aText,
    const rgb_t aTextColor = WHITE,
    const rgb_t aBackgroundColor = BLACK,
    const rgb_t aBorderColor = WHITE)
    : DecorTextButton(TYPE_CHECKBOX, aText, aTextColor, aBackgroundColor, aBorderColor)
  {
    boxSize = getTextHeight();
  }

  bool setChecked(const bool aChecked);
  bool getChecked() { return checked; }

  const bool toggle() override { return setChecked(!checked); }

  const int8_t getDecorWidth() override { return boxSize; }
  const int8_t getDecorHeight() override { return boxSize; }

protected:
  void drawBox(const rgb_t aBoxColor);
  void drawBox();

  void drawCheck(const rgb_t aCheckColor);
  void drawCheck();

  void drawDecor() override;
  void hideDecor() override;

private:
  int8_t boxSize = -1;
  bool checked = false;
};


/// @RadioButton

class RadioButton : public DecorTextButton {
public:
  RadioButton()
    : RadioButton(0, "")
  {
  }

  RadioButton(
    const int16_t aTag,
    const char* aText,
    const rgb_t aTextColor = WHITE,
    const rgb_t aBackgroundColor = BLACK,
    const rgb_t aBorderColor = WHITE)
    : DecorTextButton(TYPE_RADIOBUTTON, aText, aTextColor, aBackgroundColor, aBorderColor)
  {
    setTag(aTag);
  }

  void setDotRadius(const int8_t aDotRadius);
  void setCircleRadius(const int8_t aCircleRadius);

  const int8_t getDotRadius() { return dotRadius >= 0 ? dotRadius : getFontSizeX() * 3; }
  const int8_t getCircleRadius() { return circleRadius >= 0 ? circleRadius : getFontSizeX() * 5; }

  const int16_t getCenterX();
  const int16_t getCenterY();

  void setState(const bool aState)
  {
    state = aState;
    drawDecor();
  }
  bool geState() { return state; }

  const bool toggle() override;

  const int8_t getDecorWidth() override { return getCircleRadius() * 2; }
  const int8_t getDecorHeight() override { return getCircleRadius() * 2; }

protected:
  void drawDot(const rgb_t aDotColor);
  void drawDot();

  void drawCircle(const rgb_t aCircleColor);
  void drawCircle();

  void drawDecor() override;
  void hideDecor() override;

private:
  int8_t dotRadius = -1, circleRadius = -1;
  bool state = false;
};


/// @FieldSet
// dafault is horizontal

class FieldSet : public Button {
public:
  FieldSet(
    const int16_t aLeft,
    const int16_t aTop,
    const int16_t aWidth,
    const int16_t aHeight,
    Container** aChildren,
    const int16_t aLen,
    const rgb_t aBackgroundColor = BLACK,
    const rgb_t aBorderColor = WHITE)
    : Button(TYPE_FIELDSET, aLeft, aTop, aWidth, aHeight, aBackgroundColor, aBorderColor),
    children(aChildren),
    len(aLen)
  {
    for (int16_t i = len; --i >= 0; ) {
      children[i]->setParent(this);
    }
    disabled = true;
  }

  FieldSet(
    const int16_t aWidth,
    const int16_t aHeight,
    Container** aChildren,
    const int16_t aLen,
    const rgb_t aBackgroundColor = BLACK,
    const rgb_t aBorderColor = WHITE)
    : FieldSet(-1, -1, aWidth, aHeight, aChildren, aLen, aBackgroundColor, aBorderColor)
  {
  }

  FieldSet(
    const orient_t aOrientation,
    const int16_t aDistance,
    const int16_t aMarginWidth,
    const int16_t aMarginHeight,
    Container** aChildren,
    const int16_t aLen,
    const rgb_t aBackgroundColor = BLACK,
    const rgb_t aBorderColor = WHITE)
    : FieldSet(-1, -1, ALIGN_COMPACT, ALIGN_COMPACT, aChildren, aLen, aBackgroundColor, aBorderColor)
  {
    orientation = aOrientation;
    distance = aDistance;
    marginLeft = aMarginWidth;
    marginTop = aMarginHeight;
    marginRight = aMarginWidth;
    marginBottom = aMarginHeight;
  }

  // default is horizontal
  // min distance between children, alignClient can grow it
  void setOrientation(const orient_t aOrientation, const int8_t aDistance);

  const orient_t getOrientation() { return orientation; }

  const int8_t getDistance() { return distance; }

  const int16_t getLength() { return len; }
  Container* getItem(const int16_t aIndex)
  {
    return aIndex >= 0 && aIndex < len ? children[aIndex] : NULL;
  }

  void drawBackground() override;

  const bool isBorderRadius() override;
  void innerDraw(const bool redraw) override;

  Container* pressed(const int16_t xScreen, const int16_t yScreen) override;

  void setNotWasDrawn() override;

  friend class Container;

private:
  Container** children;
  const int16_t len = 0;
  orient_t orientation = HORIZONTAL;
  int8_t distance = 0;
  bool backgroundDrawn;

  void drawVisibleBackground();

  void updCompactWidth(const bool recalc) override;
  void updCompactHeight(const bool recalc) override;

  const int16_t updCoordHorizGetStretch();
  void updCoordHorizUpdPos(Container* b, const int16_t x);
  void  updateCoordHorizontal();

  const int16_t updCoordVertGetStretch();
  void updCoordVertUpdPos(Container* b, const int16_t y);
  void updateCoordVertical();

  void updateCoord(const bool recalc) override;

public:
  void setActiveRadio(RadioButton* aActiveRadio);
  RadioButton* getActiveRadio() { return activeRadio; }
  void markSubFolder() { isSubFolder = true; }
  bool markedSubFolder() { return isSubFolder; }

private:
  bool isSubFolder = false;
  RadioButton* activeRadio = NULL;

};

