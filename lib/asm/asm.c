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
  jmp(dx);
  label(is_lt);
  const_int(ax, 1);
  label(end_lt);
  push(ax);
}

void asm_gt(void) {
  _("GT");
  char *is_gt = get_label("is_gt");
  char *end_gt = get_label("end_gt");
  pop(ax);
  pop(bx);
  const_string(cx, is_gt);
  sless(ax, bx);
  jmpc(cx);
  const_int(ax, 0);
  const_string(dx, end_gt);
  jmp(dx);
  label(is_gt);
  const_int(ax, 1);
  label(end_gt);
  push(ax);
}

void asm_geq(void) {
  _("GEQ");
  char *is_geq = get_label("is_geq");
  char *end_geq = get_label("end_geq");
  pop(ax);
  pop(bx);
  const_string(cx, is_geq);
  cmp(ax, bx);
  jmpc(cx);
  sless(ax, bx);
  jmpc(cx);
  const_int(ax, 0);
  const_string(dx, end_geq);
  jmp(dx);
  label(is_geq);
  const_int(ax, 1);
  label(end_geq);
  push(ax);
}

void asm_leq(void) {
  _("LEQ");
  char *is_leq = get_label("is_leq");
  char *end_leq = get_label("end_leq");
  pop(bx);
  pop(ax);
  const_string(cx, is_leq);
  cmp(ax, bx);
  jmpc(cx);
  sless(ax, bx);
  jmpc(cx);
  const_int(ax, 0);
  const_string(dx, end_leq);
  jmp(dx);
  label(is_leq);
  const_int(ax, 1);
  label(end_leq);
  push(ax);
}

void asm_eq(void) {
  _("EQ");
  char *is_eq = get_label("is_eq");
  char *end_eq = get_label("end_eq");
  pop(bx);
  pop(ax);
  const_string(cx, is_eq);
  cmp(ax, bx);
  jmpc(cx);
  const_int(ax, 0);
  const_string(dx, end_eq);
  jmp(dx);
  label(is_eq);
  const_int(ax, 1);
  label(end_eq);
  push(ax);
}

void asm_start_algo(ASTNode *node) {
  _("CONFIGURATION ALGO");
  label(node->name);
  push(bp);   // Sauvegarde adresse de base de pile
  cp(bp, sp); // Remonte l'adresse de base de pile
  // Sauvgarde du contexte
  push(ax);
  push(bx);
  push(cx);
  push(dx);
  _("DEBUT ALGO");
}

void asm_end_algo() {
  _("RESTAURATION ALGO");
  // Restauration du contexte
  pop(dx);
  pop(cx);
  pop(bx);
  pop(ax);
  pop(bp); // Restaure adresse de base de pile
  ret;     // Retour à l'appelant
  _("FIN ALGO");
  _("")
}

void asm_start_call_algo(info_algo *algo_info) {
  // Réserver valeur de retour
  const_int(ax, 0);
  push(ax);
  // Réserver espace pour les variables locales
  printf("; Réserver espace pour %d variables locales\n", algo_info->nb_varloc);
  for (int i = 0; i < algo_info->nb_varloc; i++) {
    push(ax);
  }
  // Évaluer et empiler les arguments
  // A FAIRE
  // Appeler l'algorithme
  const_string(ax, algo_info->id);
  call(ax);
}