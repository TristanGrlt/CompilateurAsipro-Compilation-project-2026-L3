#define ax "ax"
#define bx "bx"
#define cx "cx"
#define dx "dx"

#define _(msg)                                                                 \
  { printf(";;;;;;;;;; " msg " ;;;;;;;;;;\n"); }

#define pop(r)                                                                 \
  { printf("\tpop " r "\n"); }
#define push(r)                                                                \
  { printf("\tpush " r "\n"); }

#define add(r1, r2)                                                            \
  { printf("\tadd " r1 "," r2 "\n"); }
#define sub(r1, r2)                                                            \
  { printf("\tsub " r1 "," r2 "\n"); }
#define mul(r1, r2)                                                            \
  { printf("\tmul " r1 "," r2 "\n"); }
#define div(r1, r2)                                                            \
  { printf("\tdiv " r1 "," r2 "\n"); }

#define const(r, v)                                                            \
  { printf("\tconst " r "," v "\n"); }

#define jmpe(r)                                                                \
  { printf("\tjmpe " r "\n"); }

#define toto