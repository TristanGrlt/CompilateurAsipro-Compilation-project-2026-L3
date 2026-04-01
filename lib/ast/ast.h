#ifndef AST_H
#define AST_H

#include "utils.h"

typedef enum {
  NODE_PROGRAM,
  NODE_CONST, // (ex: 5)
  NODE_VAR,   // (ex: p)
  NODE_SUB,
  NODE_ADD,
  NODE_MUL,
  NODE_DIV,
  NODE_LT,
  NODE_GT,
  NODE_GEQ,
  NODE_LEQ,
  NODE_EQ,
  NODE_TRUE,
  NODE_FALSE,
  NODE_SET,    // (\SET)
  NODE_CALL,   // (\CALL)
  NODE_RETURN, // (\RETURN)
  NODE_ALGO,   // (begin{algo})
  NODE_SEQ,    // (le bloc de code)
  NODE_IF,
  NODE_SHIFTL,
  NODE_DOWHILE,
} NodeType;

typedef struct ASTNode {
  NodeType type;

  int val;    // Utilisé si type == NODE_CONST
  char *name; // Utilisé si NODE_VAR, NODE_CALL, ou NODE_ALGO
  type_s expr_type;

  struct ASTNode *left;
  struct ASTNode *middle;
  struct ASTNode *right;
} ASTNode;

ASTNode *make_programe(ASTNode *algos, ASTNode *main_call);
ASTNode *make_const(int val);
ASTNode *make_var(char *name);
ASTNode *make_add(ASTNode *left, ASTNode *right);
ASTNode *make_sub(ASTNode *left, ASTNode *right);
ASTNode *make_mul(ASTNode *left, ASTNode *right);
ASTNode *make_div(ASTNode *left, ASTNode *right);
ASTNode *make_lt(ASTNode *left, ASTNode *right);
ASTNode *make_gt(ASTNode *left, ASTNode *right);
ASTNode *make_geq(ASTNode *left, ASTNode *right);
ASTNode *make_leq(ASTNode *left, ASTNode *right);
ASTNode *make_eq(ASTNode *left, ASTNode *right);
ASTNode *make_true(void);
ASTNode *make_false(void);
ASTNode *make_set(char *var_name, ASTNode *expr);
ASTNode *make_call(char *func_name, ASTNode *arg);
ASTNode *make_return(ASTNode *expr);
ASTNode *make_algo(char *name, ASTNode *params, ASTNode *body);
ASTNode *make_seq(ASTNode *instr, ASTNode *next);
ASTNode *make_if(ASTNode *condition, ASTNode *if_block, ASTNode *else_block);
void ensure_fori_var_int(const char *var_name);
ASTNode *make_fori(char *var_name, ASTNode *start_expr, ASTNode *end_expr,
                   ASTNode *body);
ASTNode *make_dowhile(ASTNode *condition, ASTNode *body);
void free_ast(ASTNode *node);

void generate_asm(ASTNode *node);
#endif
