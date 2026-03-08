#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

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
  ASTNode *node = create_node(NODE_ADD);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = left;
  node->right = right;
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