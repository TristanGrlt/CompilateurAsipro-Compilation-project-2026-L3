#include "ast.h"
#include "asm.h"
#include "symboletable.h"
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

// Opération :

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

ASTNode *make_sub(ASTNode *left, ASTNode *right) {
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror("Erreur de type : Soustraction uniquement entre entiers");
    exit(EXIT_FAILURE);
  }
  ASTNode *node = create_node(NODE_SUB);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
  node->expr_type = INT_T;
  return node;
}

ASTNode *make_mul(ASTNode *left, ASTNode *right) {
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror("Erreur de type : Multiplication uniquement entre entiers");
    exit(EXIT_FAILURE);
  }
  ASTNode *node = create_node(NODE_MUL);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
  node->expr_type = INT_T;
  return node;
}

ASTNode *make_div(ASTNode *left, ASTNode *right) {
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror("Erreur de type : Division uniquement entre entiers");
    exit(EXIT_FAILURE);
  }
  ASTNode *node = create_node(NODE_DIV);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
  node->expr_type = INT_T;
  return node;
}

ASTNode *make_lt(ASTNode *left, ASTNode *right) {
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror("Erreur de type : Comparaison uniquement entre entiers");
    exit(EXIT_FAILURE);
  }
  ASTNode *node = create_node(NODE_LT);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
  node->expr_type = BOOL_T;
  return node;
}

ASTNode *make_gt(ASTNode *left, ASTNode *right) {
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror("Erreur de type : Comparaison uniquement entre entiers");
    exit(EXIT_FAILURE);
  }
  ASTNode *node = create_node(NODE_GT);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
  node->expr_type = BOOL_T;
  return node;
}

ASTNode *make_geq(ASTNode *left, ASTNode *right) {
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror("Erreur de type : Comparaison uniquement entre entiers");
    exit(EXIT_FAILURE);
  }
  ASTNode *node = create_node(NODE_GEQ);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
  node->expr_type = BOOL_T;
  return node;
}

ASTNode *make_leq(ASTNode *left, ASTNode *right) {
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror("Erreur de type : Comparaison uniquement entre entiers");
    exit(EXIT_FAILURE);
  }
  ASTNode *node = create_node(NODE_LEQ);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
  node->expr_type = BOOL_T;
  return node;
}

ASTNode *make_eq(ASTNode *left, ASTNode *right) {
  if (!(left->expr_type == INT_T && right->expr_type == INT_T) ||
      !(left->expr_type != BOOL_T && right->expr_type != BOOL_T)) {
    yyerror("Erreur de type : Comparaison uniquement entre deux entiers ou "
            "booleens");
    exit(EXIT_FAILURE);
  }
  ASTNode *node = create_node(NODE_EQ);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
  node->expr_type = BOOL_T;
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

ASTNode *make_true() {
  ASTNode *node = create_node(NODE_TRUE);
  if (node == nullptr) {
    return nullptr;
  }
  node->expr_type = BOOL_T;
  return node;
}

ASTNode *make_false() {
  ASTNode *node = create_node(NODE_FALSE);
  if (node == nullptr) {
    return nullptr;
  }
  node->expr_type = BOOL_T;
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
  // printf("Noeud de type : %d\n", node->type);
  switch (node->type) {

  case NODE_SET:
    generate_asm(node->left);
    // Ici on devrait générer le code pour stocker la valeur dans la variable
    // node->name
    break;
  case NODE_ADD:
    generate_asm(node->left);
    generate_asm(node->right);
    asm_add();
    break;
  case NODE_SUB:
    generate_asm(node->left);
    generate_asm(node->right);
    asm_sub();
    break;
  case NODE_MUL:
    generate_asm(node->left);
    generate_asm(node->right);
    asm_mul();
    break;
  case NODE_DIV:
    generate_asm(node->left);
    generate_asm(node->right);
    asm_div();
    break;
  case NODE_LT:
    generate_asm(node->left);
    generate_asm(node->right);
    asm_lt();
    break;
  case NODE_GT:
    generate_asm(node->left);
    generate_asm(node->right);
    asm_gt();
    break;
  case NODE_GEQ:
    generate_asm(node->left);
    generate_asm(node->right);
    asm_geq();
    break;
  case NODE_LEQ:
    generate_asm(node->left);
    generate_asm(node->right);
    asm_leq();
    break;
  case NODE_EQ:
    generate_asm(node->left);
    generate_asm(node->right);
    asm_eq();
    break;
  case NODE_TRUE:
    _("BOOLEEN VRAI");
    const_int(ax, 1);
    push(ax);
    break;
  case NODE_FALSE:
    _("BOOLEEN FAUX");
    const_int(ax, 0);
    push(ax);
    break;
  case NODE_CONST:
    _("ENTIER");
    const_int(ax, node->val);
    push(ax);
    break;
  case NODE_SEQ:
    generate_asm(node->left);
    generate_asm(node->right);
    break;
  case NODE_ALGO:
    _("ALGO");
    asm_start_algo(node);
    generate_asm(node->right);
    asm_end_algo();
    break;
  case NODE_CALL:
    _("CALL");
    info_algo *algo_info = symboletable_get(node->name);
    if (algo_info == nullptr) {
      yyerror("Appel d'un algorithme non défini");
      exit(EXIT_FAILURE);
    }
    asm_start_call_algo(algo_info);

  default:
    break;
  }
}
