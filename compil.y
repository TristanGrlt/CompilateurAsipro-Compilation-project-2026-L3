%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <limits.h>
  #include "utils.h"
  #include "asm.c"
  #include "hashtable.h"
  #include "symboletable.h"
  #include "ast.h"
  int yylex();
  void yyerror (char const *);
  extern int yylineno;
  ASTNode *ast_root = nullptr; 
%}

%union {
  int integer;
  type_s type;
  char *string;
  struct ASTNode *node;
}
%token<integer> INT
%token<integer> TRUE FALSE
%token<string> ID
%token BEGIN_ALGO END_ALGO SET IF ELSE FI DOWHILE FORI OD RETURN CALL LEQ GEQ
%left '='
%left '<' '>' LEQ GEQ
%left '+' '-'
%left '*' '/'

// %type<type> EXPR
// %type<type> LALGO
// %type<type> LINSTRU
// %type<type> LPARAM

%type<node> EXPR ALGO LALGO LINSTRU INSTRU PARAM LPARAM SETER LEXPR
%type<node> COND LOOP_FOR_I LOOP_DOWHILE NEXT_IF EXEC_CALL RETURNER

%start START         // A remplacer par START

%%
//---- [ALGO        ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
START:
  LALGO EXEC_CALL   { ast_root = make_programe($1, $2); }
  ;

//---- [ALGO        ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
ALGO:
  BEGIN_ALGO '{' ID '}' {
    if (symboletable_add($3) == nullptr) {
      yyerror("Erreur interne : Impossible d'ajouter l'algorithme à la table de "
              "symboles");
      exit(EXIT_FAILURE);
    } else {
      printf("; Algorithme %s ajouté à la table de symboles\n", $3);
    }
  } '{' LPARAM '}' LINSTRU END_ALGO {
    $$ = make_algo($3, $7, $9);
  }
  ;
LALGO:
  LALGO ALGO  { $$ = make_seq($1, $2); }
  | ALGO      { $$ = $1; }
  ;

//---- [INSTRU      ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
LINSTRU:
  LINSTRU INSTRU  { $$ = make_seq($1, $2); }
  |               { $$ = nullptr; }
  ;

INSTRU:
  COND | LOOP_FOR_I | SETER | RETURNER | EXEC_CALL;

//---- [PARAM       ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
PARAM:
  ID {
    symboletable_add_param($1, UNDEF);
    // $$ = make_var($1);
  }
  ;

LPARAM:
  LPARAM ',' PARAM  { $$ = make_seq($1, $3); }
  | PARAM           { $$ = $1; }
  |                 { $$ = nullptr; } 
  ;

//---- [SET         ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
SETER:
  SET '{' ID '}' '{' EXPR '}' {
    $$ = make_set($3, $6);
  }
  ;

//---- [BOULCE FORI ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
LOOP_FOR_I:
  FORI '{' ID '}' '{' EXPR '}' '{' EXPR '}'
    LINSTRU
  OD; 

//---- [BOULCE FORI ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
LOOP_DOWHILE:
  DOWHILE '{' EXPR '}'
    LINSTRU
  OD;

//---- [COND        ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
COND :
  IF '{' EXPR '}'
    LINSTRU
  NEXT_IF FI;

NEXT_IF:
  ELSE LINSTRU | ;

//---- [CALL       ] ---------------------------------------------------------//
//----------------------------------------------------------------------------//
EXEC_CALL :
  CALL '{' ID '}' '{' LEXPR '}' { $$ = make_call($3, $6); }
;

//---- [RETURN      ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
RETURNER:
  RETURN '{' EXPR '}' { $$ = make_return($3); }
;

//---- [EXPR        ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
EXPR:
  EXPR '+' EXPR { $$ = make_add($1, $3); }
| EXPR '-' EXPR { $$ = make_sub($1, $3); }
| EXPR '*' EXPR { $$ = make_mul($1, $3); }
| EXPR '/' EXPR { $$ = make_div($1, $3); }
| EXPR '<' EXPR { $$ = make_lt($1, $3); }
| EXPR '>' EXPR { $$ = make_gt($1,$3); }
| EXPR LEQ EXPR { $$ = make_leq($1,$3); }
| EXPR GEQ EXPR { $$ = make_geq($1,$3); }
| EXPR '=' EXPR { $$ = make_eq($1,$3); }
| '(' EXPR ')'  { $$ = $2; }
| INT           { $$ = make_const($1); }
| TRUE          { $$ = make_true(); }
| FALSE         { $$ = make_false(); }
| ID            { $$ = make_var($1); } 
;

LEXPR:
  LEXPR ',' EXPR { $$ = make_seq($1, $3); }
| EXPR           { $$ = $1; }
|                { $$ = nullptr; }
;
%%

void yyerror (char const *s) {
  fprintf(stderr, "\033[1;31m[!] Ligne %d : %s\n\033[0m", yylineno, s); 
  exit(EXIT_FAILURE);
}

int main() {
  symboletable_init();
  if (yyparse() == 0) {
    printf("\tconst ax,main\n");
    printf("\tjmp ax\n");

    printf(":msgerr0\n");
    printf("@string \"[!] Erreur : Division par 0\\n\"\n");
    printf(":err0\n");
    printf("\tconst ax,msgerr0\n");
    printf("\tcallprintfs ax\n");
    printf("\tend\n");
    // printf(":main\n");
    // printf("\tconst bp,stack\n");
    // printf("\tconst sp,stack\n");
    // printf("\tconst ax,2\n"); 
    // printf("\tsub sp,ax\n");

    generate_asm(ast_root);

    printf(";;;;;;;;;;  AFFICHAGE  ;;;;;;;;;;\n");
    printf("\tcp ax,sp\n");
    printf("\tcallprintfd ax\n");
    printf("\tend\n");
    printf(":stack\n");
    printf("@int 0\n");
    return EXIT_SUCCESS;
  }
}