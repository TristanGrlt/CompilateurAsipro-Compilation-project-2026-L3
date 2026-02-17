%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <limits.h>
  #include "utils.h"
  #include "compil.tab.h"
  void string_to_int(int *r, const char *s);
  void yytext_copy(char **copy, const char *s);
%}

%option noyywrap

%%

"\\begin{algo}"          {return BEGIN_ALGO;}
"\\end{algo}"            {return END_ALGO;}

"\\SET"                  {return SET;}

"\\IF"                   {return IF;}
"\\ELSE"                 {return ELSE;}
"\\FI"                   {return FI;}

"\\DOWHILE"              {return DOWHILE;}
"\\DOFORI"               {return FORI;}
"\\OD"                   {return OD;}

"\\RETURN"               {return RETURN;}
"\\CALL"                 {return CALL;}

[1-9][0-9]*             {string_to_int(&yylval.integer, yytext); return INT;}
"+"|"-"|"*"|"/"         {return yytext[0];}
"("|")"                 {return yytext[0];}
"{"|"}"                 {return yytext[0];}
","|"="                 {return yytext[0];}
"<"|">"                 {return yytext[0];}
"<="                    {return LEQ;}
">="                    {return GEQ;}

"true"                  {return TRUE;}
"false"                 {return FALSE;}

[a-zA-Z_][a-zA-Z0-9_]* {yytext_copy(&yylval.string, yytext); return ID;}

[[:blank:]]             {}
.                       {fprintf(stderr, "Bad character: %c\n", yytext[0]);
                          yyterminate();}
\n                      {}

%%

void string_to_int(int *r, const char *s) {
  char *p;
  long v;
  errno = 0;
  v = strtol(s, &p, 10);
  if ( ( *p != '\0' || ( errno == ERANGE 
                     && ( v == LONG_MIN || v == LONG_MAX ) ) ) 
       || ( v < INT_MIN || v > INT_MAX ) ) {
    fprintf(stderr, "Error converting string to int\n");
    exit(EXIT_FAILURE);
  } 
  *r = v;
}

void yytext_copy(char **id, const char *s) {
  char *copy = malloc(strlen(s) + 1);
  if (copy == nullptr) {
    fprintf(stderr, "Memory allocation problem.\n");
    exit(EXIT_FAILURE);
  }
  strcpy(copy,s);
  *id = copy;
}

