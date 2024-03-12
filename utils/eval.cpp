/*
  Eval

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include "eval.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const bool eval_t::is(const char c)
{
  return k < len && s[k] == c;
}

const bool eval_t::is09()
{
  if (k < len) {
    const char c = s[k];
    return c >= '0' && c <= '9';
  }
  else {
    return false;
  }
}

void eval_t::skipSp()
{
  while (is(' ')) {
    ++k;
  }
}

void eval_t::skip09()
{
  while (is09()) {
    ++k;
  }
}

int eval_t::nextToken()
{
  while (true) {
    ++k;
    if (k >= len) {
      return k;
    }
    if (!is(' ')) {
      break;
    }
  }
  return -1;
}

int64_t incDec(int64_t v, int mod)
{
  while (mod--) {
    v *= 10;
  }
  return v;
}

void eval_t::reduce(int64_t* e,  int8_t* dec)
{
  while (*dec > 0) {
    if (*e % 10) {
      break;
    }
    *e /= 10;
    --*dec;
  }
}

int eval_t::number(int64_t* e,  int8_t* dec)
{
  char buf[30];
  char* p = buf;
  int k1 = k;
  skip09();
  strncpy(p, &s[k1], k - k1);
  p += k - k1;
  *dec = 0;
  if (is('.')) {
    ++k;
    k1 = k;
    skip09();
    *dec = k - k1;
    strncpy(p, &s[k1], *dec );
    p += *dec;
  }
  *p = 0;
  *e = strtoll(buf, NULL, 10);
  return -1;
}

int eval_t::expr(int64_t* e, int8_t* dec)
{
  if (is09() || is('.')) {
    int err = number(e, dec);
    if (err >= 0) {
      return err;
    }
    skipSp();
    return -1;
  }
  else {
    if (is('(')) {
      int err = nextToken();
      if (err >= 0) {
        return err;
      }
      err = weak(e, dec);
      if (err >= 0) {
        return err;
      }
      if (is(')')) {
        ++k;
        skipSp();
        return -1;
      } else {
        return k;
      }
    }
    else {
      return k;
    }
  }
}

int eval_t::strong(int64_t* e, int8_t* dec)
{
  int err = expr(e, dec);
  if (err >= 0) {
    return err;
  }
  while (k < len) {
    const bool mul = is('*');
    const bool div = is('/');
    if (mul || div) {
//      const int k1 = k;
      int err = nextToken();
      if (err >= 0) {
        return err;
      }
      int64_t e2; int8_t dec2;
      err = expr(&e2, &dec2);
      if (err >= 0) {
        return err;
      }
      if (mul) {
        *e *= e2;
        *dec += dec2;
      }
      else {
        if (div) {
          if (e2 == 0) {
            return k;
          } else {
            int64_t v2 = e2 >= 0 ? e2 : -e2;
            while (v2 > 0) {
              *e *= 10;
              ++*dec;
              v2 /= 10;
            }
            *dec -= dec2;
            if (*dec < maxd) {
              int mod = maxd - *dec;
              *e = incDec(*e, mod);
              *dec += mod;
            }
            *e /= e2;
          }
        }
      }
      reduce(e, dec);
    }
    else {
      break;
    }
  }
  return -1;
}

int eval_t::weak(int64_t* e, int8_t* dec)
{
  const char c1 = s[k];
  if (c1 == '-' || c1 == '+') {
    int err = nextToken();
    if (err >= 0) {
      return err;
    }
  }
  int err = strong(e, dec);
  if (err >= 0 ) {
    return err;
  }
  if (c1 == '-') {
    *e = -*e;
  }
  while (k < len) {
    const bool plus = is('+');
    const bool minus = is('-');
    if (plus || minus) {
      int err = nextToken();
      if (err >= 0) {
        return err;
      }
      int64_t e2; int8_t dec2;
      err = strong(&e2, &dec2);
      if (err >= 0) {
        return err;
      }
      if (plus) {
        const int mdec = *dec > dec2 ? *dec : dec2;
        *e = incDec(*e, mdec - *dec) + incDec(e2, mdec - dec2);
        *dec = mdec;
      }
      else {
        if (minus) {
          const int mdec = *dec > dec2 ? *dec : dec2;
          *e = incDec(*e, mdec-*dec) - incDec(e2, mdec - dec2);
          *dec = mdec;
        }
      }
      reduce(e, dec);
    }
    else {
      break;
    }
  }
  return -1;
}

int eval_t::eval(int64_t* e, int8_t* dec, const char* str, const int8_t prec)
{
  s = str;
  len = strlen(s);
  k = 0;
  maxd = prec;

  skipSp();
  if (k < len) {
    int err = weak(e, dec);
    if (err >= 0 || k < len ) {
      return k;
    }
    return 0;
  }
  return k;
}

char buf[80];

const char* evalToStr(int64_t e, int8_t dec)
{
  char*p = &buf[40];
  int l = snprintf(p, 40, "%lld", e);
  if (dec > 0 ) {
    if (dec < l) {
      char *q = p + l;
      q[1] = 0;
      while (--dec >= 0) {
        *q = q[-1];
        --q;
      }
      *q = '.';
    }
    else {
      dec -= l;
      while (--dec > 0) {
        *--p = '0';
      }
      *--p = '.';
      *--p = '0';
    }
  }
  return p;
}

const char* eval_t::eval(const char* text)
{
  if (!text || *text == 0) {
    return "";
  }
  int64_t e; int8_t d;
  int r = eval(&e, &d, text, 8);
  if (r == 0) {
    return evalToStr(e, d);
  }
  else {
    snprintf(buf, 40, "error at pos: %d", r);
    return buf;
  }
}
