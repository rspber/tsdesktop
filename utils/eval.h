/*
  Eval

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <inttypes.h>

typedef struct {
  const char* s;
  int len;
  int k;
  int8_t maxd;

  const bool is(const char c);
  const bool is09();
  void skipSp();
  void skip09();
  int nextToken();
  void reduce(int64_t* e,  int8_t* dec);
  int number(int64_t* e,  int8_t* dec);
  int expr(int64_t* e, int8_t* dec);
  int strong(int64_t* e, int8_t* dec);
  int weak(int64_t* e, int8_t* dec);
  int eval(int64_t* e, int8_t* dec, const char* str, const int8_t prec);
  const char* eval(const char* text);
} eval_t;
