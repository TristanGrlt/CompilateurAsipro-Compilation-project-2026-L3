#ifndef ASM__H
#define ASM__H

#include "ast.h"
#include "utils.h"

#define ax "ax"
#define bx "bx"
#define cx "cx"
#define dx "dx"
#define bp "bp"
#define sp "sp"

//---- [PRINT       ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
#define _(msg)                                                                 \
  { printf(";;;;;;;;;; " msg " ;;;;;;;;;;\n"); }

//---- [PILE        ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
#define pop(r)                                                                 \
  { printf("\tpop " r "\n"); }
#define push(r)                                                                \
  { printf("\tpush " r "\n"); }

//---- [CALCUL      ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
#define add(r1, r2)                                                            \
  { printf("\tadd " r1 "," r2 "\n"); }
#define sub(r1, r2)                                                            \
  { printf("\tsub " r1 "," r2 "\n"); }
#define mul(r1, r2)                                                            \
  { printf("\tmul " r1 "," r2 "\n"); }
#define div(r1, r2)                                                            \
  { printf("\tdiv " r1 "," r2 "\n"); }

void asm_add(void);
void asm_sub(void);
void asm_mul(void);
void asm_div(void);
void asm_lt(void);
void asm_gt(void);
void asm_geq(void);
void asm_leq(void);
void asm_eq(void);

//---- [COMPARAISON ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
#define sless(r1, r2)                                                          \
  { printf("\tsless " r1 "," r2 "\n"); }
#define cmp(r1, r2)                                                            \
  { printf("\tcmp " r1 "," r2 "\n"); }

//---- [CONSTANTE   ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
#define const(r, v)                                                            \
  { printf("\tconst " r "," v "\n"); }
#define const_int(r, v)                                                        \
  { printf("\tconst " r ",%d\n", v); }
#define const_string(r, v)                                                     \
  { printf("\tconst " r ",%s\n", v); }

//---- [JUMP COND   ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
#define jmp(r)                                                                 \
  { printf("\tjmp %s\n", r); }
#define jmpe(r)                                                                \
  { printf("\tjmpe %s\n", r); }
#define jmpc(r)                                                                \
  { printf("\tjmpc %s\n", r); }

//---- [LABEL       ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
#define label(name)                                                            \
  { printf(":%s\n", name); }

//---- [MEMEORIE     ] -------------------------------------------------------//
//----------------------------------------------------------------------------//
#define cp(r1, r2)                                                             \
  { printf("\tcp " r1 "," r2 "\n"); }

//---- [ALGO          ] ------------------------------------------------------//
//----------------------------------------------------------------------------//
#define ret                                                                    \
  { printf("\tret\n"); }
#define call(r)                                                                \
  { printf("\tcall %s\n", r); }

void asm_start_algo(ASTNode *node);
void asm_end_algo();
void asm_start_call_algo(info_algo *algo_info);

#define toto

#endif