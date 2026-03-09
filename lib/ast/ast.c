#include "ast.h"
#include "asm.h"
#include <stdio.h>
#include <stdlib.h>

extern void yyerror(const char *s);

ASTNode *create_node(NodeType type) {
  ASTNode *node = malloc(sizeof(ASTNode));
  if (node == nullptr) {
    return nullptr;
  }
  node->type = type;
  node->left = nullptr;
  node->right = nullptr;
  node->name = nullptr;
  return node;
}

ASTNode *make_const(int val) {
  ASTNode *node = create_node(NODE_CONST);
  if (node == nullptr) {
    return nullptr;
  }
  node->val = val;
  node->expr_type = INT_T;
  return node;
}

ASTNode *make_var(char *name) {
  ASTNode *node = create_node(NODE_VAR);
  if (node == nullptr) {
    return nullptr;
  }
  node->name = name;
  return node;
}

ASTNode *make_add(ASTNode *left, ASTNode *right) {
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror("Erreur de type : Addition uniquement entre entiers");
    exit(EXIT_FAILURE);
  }
  ASTNode *node = create_node(NODE_ADD);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
  node->expr_type = INT_T;
  return node;
}

ASTNode *make_set(char *var_name, ASTNode *expr) {
  ASTNode *node = create_node(NODE_SET);
  if (node == nullptr) {
    return nullptr;
  }
  node->name = var_name;
  node->left = expr;
  return node;
}

ASTNode *make_algo(char *name, ASTNode *params, ASTNode *body) {
  ASTNode *node = create_node(NODE_ALGO);
  if (node == nullptr) {
    return nullptr;
  }
  node->name = name;
  node->left = params;
  node->right = body;
  return node;
}

ASTNode *make_call(char *func_name, ASTNode *arg) {
  ASTNode *node = create_node(NODE_CALL);
  if (node == nullptr) {
    return nullptr;
  }
  node->name = func_name;
  node->left = arg;
  return node;
}

ASTNode *make_return(ASTNode *expr) {
  ASTNode *node = create_node(NODE_RETURN);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = expr;
  return node;
}

ASTNode *make_seq(ASTNode *left, ASTNode *right) {
  if (left == nullptr) {
    return right;
  }
  ASTNode *node = create_node(NODE_SEQ);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
  return node;
}

void generate_asm(ASTNode *node) {
  if (node == nullptr) {
    printf("\n\n----------------------\nErreur : Noeud AST nul\nRien n'as été "
           "relié à ast_root\nIl faut "
           "encore implémenter cette partie pour que le code présent ici "
           "puisse être exécuté\n\n\n\n");
    return;
  }
  switch (node->type) {

  case NODE_ADD:
    generate_asm(node->left);
    generate_asm(node->right);
    asm_add();
    break;

  case NODE_CONST:
    _("ENTIER");
    const_int(ax, node->val);
    push(ax);
    break;

  default:
    break;
  }
}
