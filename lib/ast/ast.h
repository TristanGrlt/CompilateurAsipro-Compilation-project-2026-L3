#ifndef AST_H
#define AST_H

#include "utils.h"

typedef enum {
  NODE_CONST,  // (ex: 5)
  NODE_VAR,    // (ex: p)
  NODE_ADD,    // (+)
  NODE_SET,    // (\SET)
  NODE_CALL,   // (\CALL)
  NODE_RETURN, // (\RETURN)
  NODE_ALGO,   // (begin{algo})
  NODE_SEQ     // (le bloc de code)
} NodeType;

typedef struct ASTNode {
  NodeType type;

  int val;    // Utilisé si type == NODE_CONST
  char *name; // Utilisé si NODE_VAR, NODE_CALL, ou NODE_ALGO
  type_s expr_type;

  struct ASTNode *left;
  struct ASTNode *right;
} ASTNode;

ASTNode *make_const(int val);
ASTNode *make_var(char *name);
ASTNode *make_add(ASTNode *left, ASTNode *right);
ASTNode *make_set(char *var_name, ASTNode *expr);
ASTNode *make_call(char *func_name, ASTNode *arg);
ASTNode *make_return(ASTNode *expr);
ASTNode *make_algo(char *name, ASTNode *params, ASTNode *body);
ASTNode *make_seq(ASTNode *instr, ASTNode *next);

void generate_asm(ASTNode *node);
#endif
