#include "asm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---- [CALCUL      ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
char *get_label(const char *prefix) {
  static int counter = 0;
  char *label = malloc(20);
  sprintf(label, "%s_%d", prefix, counter++);
  return label;
}

//---- [CALCUL      ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
void asm_add(void) {
  _("ADDITION")
  pop(bx);
  pop(ax);
  add(ax, bx);
  push(ax);
}

void asm_sub(void) {
  _("SOUSTRACTION")
  pop(bx);
  pop(ax);
  sub(ax, bx);
  push(ax);
}

void asm_mul(void) {
  _("MULTIPLICATION")
  pop(bx);
  pop(ax);
  mul(ax, bx);
  push(ax);
}

void asm_div(void) {
  _("DIVISION")
  pop(bx);
  pop(ax);
  const(cx, "err0");
  div(ax, bx);
  jmpe(cx);
  push(ax);
}

void asm_lt(void) {
  _("LT");
  char *is_lt = get_label("is_lt");
  char *end_lt = get_label("end_lt");
  pop(bx);
  pop(ax);
  const_string(cx, is_lt);
  sless(ax, bx);
  jmpc(cx);
  const_int(ax, 0);
  const_string(dx, end_lt);
  jmpc(dx);
  label(is_lt);
  const_int(ax, 1);
  label(end_lt);
  push(ax);
}