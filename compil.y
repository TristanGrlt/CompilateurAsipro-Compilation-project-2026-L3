%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <limits.h>
  #include "utils.h"
  #include "asm.h"
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
%token BEGIN_ALGO END_ALGO SET IF ELSE FI DOWHILE FORI OD RETURN CALL LEQ QEQ
%left '='
%left '<' '>' LEQ QEQ
%left '+' '-'
%left '*' '/'

%type<type> EXPR
%type<type> LALGO
%type<type> LINSTRU
%type<type> LPARAM
%start EXPR

%%
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
      printf(";;;;;;;;;;  ADDITION  ;;;;;;;;;;\n");
      pop(bx);
      pop(ax);
      add(ax, bx);
      push(ax);
      $$ = INT_T;
    } else {
      yyerror("Addition uniquement entre entiers");
    }
  }
| EXPR '-' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      printf(";;;;;;;;;;  SOUSTRACTION  ;;;;;;;;;;\n");
      pop(bx);
      pop(ax);
      sub(ax, bx);
      push(ax);
      $$ = INT_T;
    } else {
      yyerror("Soustraction uniquement entre entiers");
    }
  }
| EXPR '*' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      printf(";;;;;;;;;;  MULTIPLICATION  ;;;;;;;;;;\n");
      pop(bx);
      pop(ax);
      mul(ax, bx);
      push(ax);
      $$ = INT_T;
    } else {
      yyerror("Multiplication uniquement entre entiers");
    }
  }
| EXPR '/' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      printf(";;;;;;;;;;  DIVISION  ;;;;;;;;;;\n");
      pop(bx);
      pop(ax);
      const(cx, "err0");
      div(ax, bx);
      jmpe(cx);
      push(ax);
      $$ = INT_T;
    } else {
      yyerror("Division uniquement entre entiers");
    }
  }
| INT             {
    printf(";;;;;;;;;;  SAUVGARDE ENTIER %d  ;;;;;;;;;;\n", $1);
    printf("\tconst ax,%d\n", $1);
    printf("\tpush ax\n");
    $$ = INT_T;
  }
| '(' EXPR ')' {
    // printf("Expression parenthésée\n");
    $$ = $2;
}
| EXPR '<' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      printf(";;;;;;;;;;  LESS THAN  ;;;;;;;;;;\n");
      printf("\tpop bx\n");
      printf("\tpop ax\n");
      printf("\tsless ax,bx\n");
      printf("\tpush ax\n");
      $$ = BOOL_T;
    }  else {
      yyerror("Inférieur uniquement entre entiers");
    }
}
| EXPR '>' EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      //code asm
      printf("Supérieur\n");
      $$ = BOOL_T;
    } else {
      yyerror("Supérieur uniquement entre entiers");
    }
}
| EXPR LEQ EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      //code asm
      printf("Inférieur ou égal\n");
      $$ = BOOL_T;
    } else {
      yyerror("Inférieur ou égale uniquement entre entiers");
    }
}
| EXPR QEQ EXPR   {
    if ($1 == INT_T && $3 == INT_T) {
      //code asm
      printf("Supérieur ou égal\n");
      $$ = BOOL_T;
    } else {
      yyerror("Supérieur ou égale uniquement entre entiers");
    }
}
| EXPR '=' EXPR   {
    if (($1 == $3 )) {
      //code asm
      printf("Égal\n");
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
