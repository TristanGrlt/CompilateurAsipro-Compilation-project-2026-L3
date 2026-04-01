#include "ast.h"
#include "asm.h"
#include "symboletable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void yyerror(const char *s);

static const char *type_to_string(type_s type) {
  switch (type) {
  case INT_T:
    return "entier";
  case BOOL_T:
    return "booléen";
  case UNDEF:
    return "indéfini";
  default:
    return "inconnu";
  }
}

static void yyerror_binary_types(const char *context, ASTNode *left,
                                 ASTNode *right, const char *expected) {
  char message[256];
  snprintf(message, sizeof(message),
           "Erreur de type : %s (attendu : %s, obtenu : gauche=%s, droite=%s)",
           context, expected, type_to_string(left->expr_type),
           type_to_string(right->expr_type));
  yyerror(message);
}

static void yyerror_unary_type(const char *context, ASTNode *node,
                               type_s expected) {
  char message[256];
  snprintf(message, sizeof(message),
           "Erreur de type : %s (attendu : %s, obtenu : %s)", context,
           type_to_string(expected), type_to_string(node->expr_type));
  yyerror(message);
}

static void sync_param_type_if_needed(info_var *var) {
  if (var == nullptr) {
    return;
  }
  info_algo *current = symboletable_get_current();
  if (current == nullptr || current->param_types == nullptr) {
    return;
  }
  info_var *param_var = symboletable_get_var_param(var->id);
  if (param_var == var && param_var->nb >= 0 &&
      param_var->nb < current->nb_param) {
    current->param_types[param_var->nb] = param_var->type;
  }
}

static void validate_call_args_rec(info_algo *algo_info, ASTNode *arg,
                                   int *arg_index) {
  if (arg == nullptr) {
    return;
  }
  if (arg->type == NODE_SEQ) {
    validate_call_args_rec(algo_info, arg->left, arg_index);
    validate_call_args_rec(algo_info, arg->right, arg_index);
    return;
  }

  if (*arg_index >= algo_info->nb_param) {
    char message[256];
    snprintf(message, sizeof(message),
             "Erreur d'appel : trop d'arguments pour '%s' (attendu : %d)",
             algo_info->id, algo_info->nb_param);
    yyerror(message);
    exit(EXIT_FAILURE);
  }

  type_s expected_type = UNDEF;
  if (algo_info->param_types != nullptr) {
    expected_type = algo_info->param_types[*arg_index];
  }

  if (expected_type == UNDEF) {
    if (algo_info->param_types != nullptr) {
      algo_info->param_types[*arg_index] = arg->expr_type;
    }
  } else if (arg->expr_type != expected_type) {
    char message[256];
    snprintf(message, sizeof(message),
             "Erreur d'appel : type invalide pour l'argument %d de '%s' "
             "(attendu : %s, obtenu : %s)",
             *arg_index + 1, algo_info->id, type_to_string(expected_type),
             type_to_string(arg->expr_type));
    yyerror(message);
    exit(EXIT_FAILURE);
  }

  (*arg_index)++;
}

ASTNode *create_node(NodeType type) {
  ASTNode *node = calloc(1, sizeof(ASTNode));
  if (node == nullptr) {
    return nullptr;
  }
  node->type = type;
  return node;
}

ASTNode *make_programe(ASTNode *algos, ASTNode *main_call) {
  if (main_call != nullptr && main_call->type != NODE_CALL) {
    yyerror("Erreur de type : Le programme doit se terminer par un appel à "
            "l'algorithme principal");
    exit(EXIT_FAILURE);
  }
  ASTNode *node = create_node(NODE_PROGRAM);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = algos;
  node->right = main_call;
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
  info_var *var = symboletable_get_var(name);
  if (var == nullptr) {
    char message[256];
    snprintf(message, sizeof(message), "Variable '%s' non déclarée", name);
    yyerror(message);
    exit(EXIT_FAILURE);
  }
  node->expr_type = var->type;

  return node;
}

static void force_type(ASTNode *node, type_s exp_type) {
  if (node->type == NODE_VAR && node->expr_type == UNDEF) {
    info_var *var = symboletable_get_var(node->name);
    if (var == nullptr) {
      char message[256];
      snprintf(message, sizeof(message), "Variable '%s' non déclarée",
               node->name);
      yyerror(message);
      exit(EXIT_FAILURE);
    }
    if (var->type == UNDEF) {
      var->type = exp_type;
      sync_param_type_if_needed(var);
    } else if (var->type != exp_type) {
      char message[256];
      snprintf(message, sizeof(message),
               "Erreur de type : la variable '%s' est de type %s mais est "
               "utilisée comme %s",
               node->name, type_to_string(var->type), type_to_string(exp_type));
      yyerror(message);
      exit(EXIT_FAILURE);
    }
    node->expr_type = var->type;
  }
}

// Opération :

static int is_power_of_two(int x) { return x > 0 && (x & (x - 1)) == 0; }

static int get_shift_amount(int x) {
  int k = 0;
  while (x > 1) {
    x >>= 1;
    k++;
  }
  return k;
}

ASTNode *make_add(ASTNode *left, ASTNode *right) {
  force_type(left, INT_T);
  force_type(right, INT_T);
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror_binary_types("Addition", left, right, "entier + entier");
    exit(EXIT_FAILURE);
  }

  if (left->type == NODE_CONST && right->type == NODE_CONST) {
    int val = left->val + right->val;
    free_ast(left);
    free_ast(right);
    return make_const(val);
  }

  if (right->type == NODE_CONST && right->val == 0) {
    free_ast(right);
    return left;
  }
  if (left->type == NODE_CONST && left->val == 0) {
    free_ast(left);
    return right;
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
  force_type(left, INT_T);
  force_type(right, INT_T);
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror_binary_types("Soustraction", left, right, "entier - entier");
    exit(EXIT_FAILURE);
  }

  if (left->type == NODE_CONST && right->type == NODE_CONST) {
    int val = left->val - right->val;
    free_ast(left);
    free_ast(right);
    return make_const(val);
  }

  if (right->type == NODE_CONST && right->val == 0) {
    free_ast(right);
    return left;
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
  force_type(left, INT_T);
  force_type(right, INT_T);
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror_binary_types("Multiplication", left, right, "entier * entier");
    exit(EXIT_FAILURE);
  }

  if (left->type == NODE_CONST && right->type == NODE_CONST) {
    int val = left->val * right->val;
    free_ast(left);
    free_ast(right);
    return make_const(val);
  }

  if (right->type == NODE_CONST && right->val == 1) {
    free_ast(right);
    return left;
  }
  if (left->type == NODE_CONST && left->val == 1) {
    free_ast(left);
    return right;
  }
  if (right->type == NODE_CONST && right->val == 0) {
    free_ast(left);
    return right;
  }
  if (left->type == NODE_CONST && left->val == 0) {
    free_ast(right);
    return left;
  }

  if (right->type == NODE_CONST && is_power_of_two(right->val)) {
    ASTNode *node = create_node(NODE_SHIFTL);
    if (node != nullptr) {
      node->left = left;
      node->val = get_shift_amount(right->val);
      node->expr_type = INT_T;
      free_ast(right);
      return node;
    }
  }
  if (left->type == NODE_CONST && is_power_of_two(left->val)) {
    ASTNode *node = create_node(NODE_SHIFTL);
    if (node != nullptr) {
      node->left = right;
      node->val = get_shift_amount(left->val);
      node->expr_type = INT_T;
      free_ast(left);
      return node;
    }
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
  force_type(left, INT_T);
  force_type(right, INT_T);
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror_binary_types("Division", left, right, "entier / entier");
    exit(EXIT_FAILURE);
  }

  if (left->type == NODE_CONST && right->type == NODE_CONST) {
    if (right->val == 0) {
      yyerror("Erreur de compilation : Division par zéro détectée !");
      exit(EXIT_FAILURE);
    }
    int val = left->val / right->val;
    free_ast(left);
    free_ast(right);
    return make_const(val);
  }

  if (right->type == NODE_CONST && right->val == 1) {
    free_ast(right);
    return left;
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
  force_type(left, INT_T);
  force_type(right, INT_T);
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror_binary_types("Comparaison '<'", left, right, "entier < entier");
    exit(EXIT_FAILURE);
  }

  if (left->type == NODE_CONST && right->type == NODE_CONST) {
    int verified = (left->val < right->val);
    free_ast(left);
    free_ast(right);
    return verified ? make_true() : make_false();
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
  force_type(left, INT_T);
  force_type(right, INT_T);
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror_binary_types("Comparaison '>'", left, right, "entier > entier");
    exit(EXIT_FAILURE);
  }

  if (left->type == NODE_CONST && right->type == NODE_CONST) {
    int verified = (left->val > right->val);
    free_ast(left);
    free_ast(right);
    return verified ? make_true() : make_false();
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
  force_type(left, INT_T);
  force_type(right, INT_T);
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror_binary_types("Comparaison '>='", left, right, "entier >= entier");
    exit(EXIT_FAILURE);
  }

  if (left->type == NODE_CONST && right->type == NODE_CONST) {
    int verified = (left->val >= right->val);
    free_ast(left);
    free_ast(right);
    return verified ? make_true() : make_false();
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
  force_type(left, INT_T);
  force_type(right, INT_T);
  if (left->expr_type != INT_T || right->expr_type != INT_T) {
    yyerror_binary_types("Comparaison '<='", left, right, "entier <= entier");
    exit(EXIT_FAILURE);
  }

  if (left->type == NODE_CONST && right->type == NODE_CONST) {
    int verified = (left->val <= right->val);
    free_ast(left);
    free_ast(right);
    return verified ? make_true() : make_false();
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
  if (left->expr_type == UNDEF && right->expr_type != UNDEF) {
    force_type(left, right->expr_type);
  } else if (right->expr_type == UNDEF && left->expr_type != UNDEF) {
    force_type(right, left->expr_type);
  } else if (left->expr_type == UNDEF && right->expr_type == UNDEF) {
    force_type(left, INT_T);
    force_type(right, INT_T);
  }
  if (left->expr_type != right->expr_type) {
    yyerror_binary_types("Comparaison '='", left, right,
                         "même type des deux côtés");
    exit(EXIT_FAILURE);
  }

  if (left->type == NODE_CONST && right->type == NODE_CONST) {
    int verified = (left->val == right->val);
    free_ast(left);
    free_ast(right);
    return verified ? make_true() : make_false();
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
  info_var *var = symboletable_get_var(var_name);
  if (var == nullptr) {
    // La variable n'existe pas encore, on l'ajoute à la table de symboles
    var = symboletable_add_varloc(var_name, expr->expr_type);
    if (var == nullptr) {
      yyerror("Erreur interne : Impossible d'ajouter la variable locale à la "
              "table de symboles");
      exit(EXIT_FAILURE);
    }
  } else {
    // La variable existe déjà, on vérifie que le type de l'expression
    // correspond
    if (var->type == UNDEF) {
      var->type = expr->expr_type;
      sync_param_type_if_needed(var);
    } else if (var->type != expr->expr_type) {
      char message[256];
      snprintf(message, sizeof(message),
               "Erreur de type : affectation invalide sur '%s' (attendu : %s, "
               "obtenu : %s)",
               var_name, type_to_string(var->type),
               type_to_string(expr->expr_type));
      yyerror(message);
      exit(EXIT_FAILURE);
    }
  }
  ASTNode *node = create_node(NODE_SET);
  if (node == nullptr) {
    return nullptr;
  }
  node->name = var_name;
  node->left = expr;
  node->type = NODE_SET;
  node->expr_type = var->type;
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
  info_algo *algo_info = symboletable_get(func_name);
  if (algo_info == nullptr) {
    yyerror("Appel d'un algorithme non défini");
    exit(EXIT_FAILURE);
  }

  int arg_count = 0;
  validate_call_args_rec(algo_info, arg, &arg_count);
  if (arg_count != algo_info->nb_param) {
    char message[256];
    snprintf(message, sizeof(message),
             "Erreur d'appel : nombre d'arguments invalide pour '%s' "
             "(attendu : %d, obtenu : %d)",
             func_name, algo_info->nb_param, arg_count);
    yyerror(message);
    exit(EXIT_FAILURE);
  }

  ASTNode *node = create_node(NODE_CALL);
  if (node == nullptr) {
    return nullptr;
  }
  node->name = func_name;
  node->left = arg;
  node->expr_type = algo_info->type;

  /* * ATTENTION (Cas particulier de la récursivité) :
   * Si l'algorithme s'appelle lui-même AVANT que le parseur n'ait rencontré
   * son instruction RETURN, le type sera toujours UNDEF.
   * Dans ce cas, on peut postuler temporairement que c'est un INT_T.
   */
  if (node->expr_type == UNDEF) {
    node->expr_type = INT_T;
  }

  return node;
}

ASTNode *make_return(ASTNode *expr) {
  // Set type
  info_algo *current_algo = symboletable_get_current();
  if (current_algo != nullptr) {
    if (current_algo->type == UNDEF) {
      current_algo->type = expr->expr_type; // Inférence du type
    } else if (current_algo->type != expr->expr_type) {
      char message[256];
      snprintf(message, sizeof(message),
               "Erreur de type : retour incohérent dans '%s' (attendu : %s, "
               "obtenu : %s)",
               current_algo->id, type_to_string(current_algo->type),
               type_to_string(expr->expr_type));
      yyerror(message);
      exit(EXIT_FAILURE);
    }
  }

  ASTNode *node = create_node(NODE_RETURN);
  if (node == nullptr) {
    return nullptr;
  }
  node->left = expr;
  node->expr_type = expr->expr_type;
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

ASTNode *make_if(ASTNode *condition, ASTNode *if_block, ASTNode *else_block) {
  force_type(condition, BOOL_T);
  if (condition->expr_type != BOOL_T) {
    yyerror_unary_type("La condition du IF doit être booléenne", condition,
                       BOOL_T);
    exit(EXIT_FAILURE);
  }

  ASTNode *node = create_node(NODE_IF);
  if (node == nullptr) {
    return nullptr;
  }

  node->left = condition;
  node->middle = if_block;
  node->right = else_block;

  return node;
}

void ensure_fori_var_int(const char *var_name) {
  info_var *fori_var = symboletable_get_var(var_name);
  if (fori_var == nullptr) {
    if (symboletable_add_varloc(var_name, INT_T) == nullptr) {
      yyerror("Erreur interne : Impossible d'ajouter la variable de boucle");
      exit(EXIT_FAILURE);
    }
    return;
  }

  if (fori_var->type == UNDEF) {
    fori_var->type = INT_T;
  } else if (fori_var->type != INT_T) {
    char message[256];
    snprintf(message, sizeof(message),
             "Erreur de type : La variable de boucle '%s' doit être un entier",
             var_name);
    yyerror(message);
    exit(EXIT_FAILURE);
  }
}

ASTNode *make_dowhile(ASTNode *condition, ASTNode *body) {
  force_type(condition, BOOL_T);
  if (condition->expr_type != BOOL_T) {
    yyerror_unary_type("La condition du WHILE doit être booléenne", condition,
                       BOOL_T);
    exit(EXIT_FAILURE);
  }

  ASTNode *node = create_node(NODE_DOWHILE);
  if (node == nullptr) {
    return nullptr;
  }

  node->left = condition;
  node->right = body;

  return node;
}

ASTNode *make_fori(char *var_name, ASTNode *start_expr, ASTNode *end_expr,
                   ASTNode *body) {
  force_type(start_expr, INT_T);
  force_type(end_expr, INT_T);
  if (start_expr->expr_type != INT_T || end_expr->expr_type != INT_T) {
    yyerror_binary_types("Les bornes du FORI doivent être entières", start_expr,
                         end_expr, "entier .. entier");
    exit(EXIT_FAILURE);
  }

  ASTNode *init = make_set(var_name, start_expr);

  ASTNode *var_cond = make_var(strdup(var_name));
  var_cond->expr_type = INT_T;
  ASTNode *condition = make_leq(var_cond, end_expr);

  ASTNode *var_inc = make_var(strdup(var_name));
  var_inc->expr_type = INT_T;
  ASTNode *un = make_const(1);
  ASTNode *addition = make_add(var_inc, un);

  ASTNode *increment = make_set(strdup(var_name), addition);

  ASTNode *new_body = make_seq(body, increment);
  ASTNode *boucle = make_dowhile(condition, new_body);

  return make_seq(init, boucle);
}

void free_ast(ASTNode *node) {
  if (node == nullptr)
    return;

  free_ast(node->left);
  free_ast(node->middle);
  free_ast(node->right);

  if (node->type == NODE_VAR || node->type == NODE_CALL ||
      node->type == NODE_ALGO || node->type == NODE_SET) {
    if (node->name != nullptr) {
      free(node->name);
    }
  }

  free(node);
}

void generate_asm(ASTNode *node) {
  if (node == nullptr) {
    return;
  }
  // printf("Noeud de type : %d\n", node->type);
  switch (node->type) {

  case NODE_SET: {
    _("SET");
    generate_asm(node->left);
    info_var *var_loc2 = symboletable_get_var_loc(node->name);
    info_algo *al2 = symboletable_get_current();

    if (var_loc2 == nullptr) {
      info_var *var_param = symboletable_get_var_param(node->name);
      if (var_param == nullptr) {
        yyerror("Variable non reconnue");
        exit(EXIT_FAILURE);
      }
      _("Stockage valeur dans le paramètre");
      asm_compute_var_addr(al2->nb_param - var_param->nb,
                           cx); // Formule Paramètre
      pop(ax);
      storew(ax, cx);
    } else {
      _("Stockage valeur dans la variable locale");
      asm_compute_var_addr(al2->nb_param + al2->nb_varloc - var_loc2->nb + 1,
                           cx); // Formule Locale
      pop(ax);
      storew(ax, cx);
    }
    break;
  }
  case NODE_DOWHILE:
    _("BOUCLE DOWHILE");
    char *l_start = get_label("start_while");
    char *l_end = get_label("end_while");
    label(l_start);
    generate_asm(node->left);
    pop(ax);
    const_string(cx, l_end);
    const_int(bx, 0);
    cmp(ax, bx);
    jmpc(cx);

    if (node->right != nullptr) {
      generate_asm(node->right);
    }
    const_string(cx, l_start);
    jmp(cx);
    label(l_end);
    free(l_start);
    free(l_end);
    break;
  case NODE_IF:
    char *l_else = get_label("else");
    char *l_end2 = get_label("end_if");

    generate_asm(node->left);

    pop(ax);
    const_string(cx, l_else);
    const_int(bx, 0);
    cmp(ax, bx);

    jmpc(cx);

    if (node->middle != nullptr) {
      generate_asm(node->middle);
    }

    const_string(cx, l_end2);
    jmp(cx);

    label(l_else);
    if (node->right != nullptr) {
      generate_asm(node->right);
    }

    label(l_end2);

    free(l_else);
    free(l_end2);
    break;
  case NODE_SHIFTL:
    generate_asm(node->left);
    asm_shiftl(node->val);
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
  case NODE_VAR: {
    info_var *var_loc = symboletable_get_var_loc(node->name);
    info_algo *al = symboletable_get_current();

    if (var_loc == nullptr) {
      info_var *var_param = symboletable_get_var_param(node->name);
      if (var_param == nullptr) {
        yyerror("Variable non reconnue");
        exit(EXIT_FAILURE);
      }
      _("Lecture du paramètre");
      asm_compute_var_addr(al->nb_param - var_param->nb, ax);
      loadw(bx, ax);
      push(bx);
    } else {
      _("Lecture de la variable locale");
      asm_compute_var_addr(al->nb_param + al->nb_varloc - var_loc->nb + 1, ax);
      loadw(bx, ax);
      push(bx);
    }
    break;
  }
  case NODE_SEQ:
    generate_asm(node->left);
    generate_asm(node->right);
    break;
  case NODE_ALGO:
    _("ALGO");
    symboletable_set_current(node->name);
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
    generate_asm(node->left);
    // call algo
    _("CALL DE L'ALGO");
    const_string(ax, algo_info->id);
    call(ax);

    _("Restauration variable");
    // On dépile les paramètres de la fonction APPELÉE
    for (int i = 0; i < algo_info->nb_param + algo_info->nb_varloc; ++i) {
      pop(ax);
    }
    break;
  case NODE_RETURN:
    _("RETURN");
    info_algo *al4 = symboletable_get_current();
    generate_asm(node->left);
    pop(ax);

    // L'emplacement de retour est 1 élément plus bas que la première variable
    // locale
    asm_compute_var_addr(al4->nb_param + al4->nb_varloc + 1, cx);
    storew(ax, cx);
    asm_end_algo();
    break;
  case NODE_PROGRAM:
    _("PROGRAMME");
    label("msgtrue");
    printf("@string \"true\\n\"\n");
    label("msgfalse");
    printf("@string \"false\\n\"\n");
    if (node->left != nullptr) {
      generate_asm(node->left);
    }
    label("main");
    const_string(bp, "stack");
    const_string(sp, "stack");
    const_int(ax, 2);
    sub(sp, ax);
    if (node->right != nullptr) {
      generate_asm(node->right);
      if (node->right->expr_type == BOOL_T) {
        cp(ax, sp);
        loadw(bx, ax);
        const_int(cx, 0);
        const_string(dx, "print_false");
        cmp(bx, cx);
        jmpc(dx);

        const_string(ax, "msgtrue");
        callprintfs(ax);
        const_string(dx, "end_print_bool");
        jmp(dx);

        label("print_false");
        const_string(ax, "msgfalse");
        callprintfs(ax);
        label("end_print_bool");
      } else {
        cp(ax, sp);
        callprintfd(ax);
      }
    }

    break;
  default:
    break;
  }
}
