%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <limits.h>
  #include "utils.h"
  #include "asm.c"
  int yylex();
  void yyerror (char const *);
%}

%union {
  int integer;
  type_s type;
  char *string;
}
%token<integer> INT
%token<integer> TRUE FALSE
%token<string> ID
%token BEGIN_ALGO END_ALGO SET IF ELSE FI DOWHILE FORI OD RETURN CALL LEQ GEQ
%left '='
%left '<' '>' LEQ GEQ
%left '+' '-'
%left '*' '/'

%type<type> EXPR
%type<type> LALGO
%type<type> LINSTRU
%type<type> LPARAM
%start EXPR         // A remplacer par START

%%
//---- [ALGO        ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
START:
  ALGO | CALL

//---- [ALGO        ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
ALGO:
  BEGIN_ALGO '{' ID '}' '{' LPARAM '}'
    LINSTRU
  END_ALGO;
LALGO:
  LALGO ALGO | ALGO;

//---- [INSTRU      ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
LINSTRU:
  LINSTRU INSTRU | INSTRU;
INSTRU:
  COND | LOOP_FOR_I | SET | RETURN

//---- [PARAM       ] --------------------------------------------------------//
//----------------------------------------------------------------------------//
PARAM:
  ID;
LPARAM:
  LPARAM ',' PARAM | PARAM;

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
    if ($1 == INT_T && $3 == INT_T) {
      asm_add();
      $$ = INT_T;
    } else {
      yyerror("Addition uniquement entre entiers");
    }
  }
| EXPR '-' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_sub();
      $$ = INT_T;
    } else {
      yyerror("Soustraction uniquement entre entiers");
    }
  }
| EXPR '*' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_mul();
      $$ = INT_T;
    } else {
      yyerror("Multiplication uniquement entre entiers");
    }
  }
| EXPR '/' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_div();
      $$ = INT_T;
    } else {
      yyerror("Division uniquement entre entiers");
    }
  }
| INT             {
    _("ENTIER");
    const_int(ax, $1);
    push(ax);
    $$ = INT_T;
  }
| '(' EXPR ')' {
    $$ = $2;
}
| EXPR '<' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_lt();
      $$ = BOOL_T;
    }  else { 
      yyerror("Inférieur uniquement entre entiers");
    }
}
| EXPR '>' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_gt();
      $$ = BOOL_T;
    } else {
      yyerror("Supérieur uniquement entre entiers");
    }
}
| EXPR LEQ EXPR   { 
    if ($1 == INT_T && $3 == INT_T) {
      asm_leq();
      $$ = BOOL_T;
    } else {
      yyerror("Inférieur ou égale uniquement entre entiers");
    }
}
| EXPR GEQ EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      asm_geq();
      $$ = BOOL_T;
    } else {
      yyerror("Supérieur ou égale uniquement entre entiers");
    }
}
| EXPR '=' EXPR   {
    if (($1 == $3 )) {
      asm_eq();
      $$ = BOOL_T;
    } else {
      yyerror("Égalité uniquement entre deux entiers ou deux booléen");
    }
}
| TRUE            {
  // code asm
  printf("Booléen : true\n");
  $$ = BOOL_T;
}
| FALSE            {
  // code asm
  printf("Booléen : false\n");
  $$ = BOOL_T;
}
;
%%

void yyerror (char const *s) {fprintf(stderr, "\033[1;31m[!] : %s\n\033[0m", s); exit(EXIT_FAILURE);}

int main() {
  printf("\tconst ax,main\n");
  printf("\tjmp ax\n");

  printf(":msgerr0\n");
  printf("@string \"[!] Erreur : Division par 0\\n\"\n");
  printf(":err0\n");
  printf("\tconst ax,msgerr0\n");
  printf("\tcallprintfs ax\n");
  printf("\tend\n");
  printf(":main\n");
  printf("\tconst bp,stack\n");
  printf("\tconst sp,stack\n");
  printf("\tconst ax,2\n"); 
  printf("\tsub sp,ax\n");

  yyparse();

  printf(";;;;;;;;;;  AFFICHAGE  ;;;;;;;;;;\n");
  printf("\tcp ax,sp\n");
  printf("\tcallprintfd ax\n");
  printf("\tend\n");
  printf(":stack\n");
  printf("@int 0\n");
  return EXIT_SUCCESS;
}
