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

%type<node> EXPR ALGO LALGO LINSTRU INSTRU PARAM LPARAM SETER 

%start START         // A remplacer par START

%%
//---- [ALGO        ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
START:
  LALGO | CALL

//---- [ALGO        ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
ALGO:
  BEGIN_ALGO '{' ID '}' '{' LPARAM '}' LINSTRU END_ALGO {
    symboletable_add($3);
    $$ = make_algo($3, $6, $8);
  }
  ;
LALGO:
  LALGO ALGO  {$$ = make_seq($1, $2);}
  | ALGO      {$$ = $1;}
  ;

//---- [INSTRU      ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
LINSTRU:
  LINSTRU INSTRU  {$$ = make_seq($1, $2);}
  | INSTRU        {$$ = $1;}
  |               {$$ = nullptr;}
  ;

INSTRU:
  COND | LOOP_FOR_I | SETER | RETURN;

//---- [PARAM       ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
PARAM:
  ID {
    symboletable_add_param($1, UNDEF);
    $$ = make_var($1);
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
    symboletable_add($3);
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

//---- [CALL       ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
EXEC_CALL :
  CALL '{' ID '}' '{' LPARAM '}'

//---- [EXPR        ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
EXPR:
  EXPR '+' EXPR   {
    $$ = make_add($1, $3);
  }
| EXPR '-' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_sub();
      //$$ = INT_T;
    } else {
      yyerror("Soustraction uniquement entre entiers");
    }
  }
| EXPR '*' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_mul();
      //$$ = INT_T;
    } else {
      yyerror("Multiplication uniquement entre entiers");
    }
  }
| EXPR '/' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_div();
      //$$ = INT_T;
    } else {
      yyerror("Division uniquement entre entiers");
    }
  }
| INT             {
    // _("ENTIER");
    // const_int(ax, $1);
    // push(ax);
    $$ = make_const($1);
  }
| '(' EXPR ')' {
    //$$ = $2;
}
| EXPR '<' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_lt();
      //$$ = BOOL_T;
    }  else { 
      yyerror("Inférieur uniquement entre entiers");
    }
}
| EXPR '>' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_gt();
      //$$ = BOOL_T;
    } else {
      yyerror("Supérieur uniquement entre entiers");
    }
}
| EXPR LEQ EXPR   { 
    if ($1 == INT_T && $3 == INT_T) {
      asm_leq();
      //$$ = BOOL_T;
    } else {
      yyerror("Inférieur ou égale uniquement entre entiers");
    }
}
| EXPR GEQ EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_geq();
      //$$ = BOOL_T;
    } else {
      yyerror("Supérieur ou égale uniquement entre entiers");
    }
}
| EXPR '=' EXPR   {
    if (($1 == $3 )) {
      asm_eq();
      //$$ = BOOL_T;
    } else {
      yyerror("Égalité uniquement entre deux entiers ou deux booléen");
    }
}
| TRUE            {
  // code asm
  printf("Booléen : true\n");
  //$$ = BOOL_T;
}
| FALSE            {
  // code asm
  printf("Booléen : false\n");
  //$$ = BOOL_T;
}
;
%%

void yyerror (char const *s) {fprintf(stderr, "\033[1;31m[!] : %s\n\033[0m", s); exit(EXIT_FAILURE);}

int main() {
  symboletable_init();

  printf("\tconst ax,main\n");
  printf("\tjmp ax\n");

  printf(":msgerr0\n");
  printf("@string \"[!] Erreur : Division par 0\\n\"\n");                    //
  printf(":err0\n");
  printf("\tconst ax,msgerr0\n");
  printf("\tcallprintfs ax\n");
  printf("\tend\n");
  printf(":main\n");
  printf("\tconst bp,stack\n");
  printf("\tconst sp,stack\n");
  printf("\tconst ax,2\n"); 
  printf("\tsub sp,ax\n");

  if (yyparse() == 0) {
    generate_asm(ast_root);
  } 

  printf(";;;;;;;;;;  AFFICHAGE  ;;;;;;;;;;\n");
  printf("\tcp ax,sp\n");
  printf("\tcallprintfd ax\n");
  printf("\tend\n");
  printf(":stack\n");
  printf("@int 0\n");
  return EXIT_SUCCESS;
}
