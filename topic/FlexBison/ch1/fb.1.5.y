%{
#include <stdio.h>
%}

%token NUMBER
%token ADD SUB MUL DIV ABS
%token EOL
%token OP CP
%token AND OR XOR

%%
/* BNF */
calclist:
| calclist exp EOL {printf("= %d\n", $2); }
/* exer1.1 suppoer comment on entire line */
| calclist EOL
;

/* exer1.3 bitwise operation is lower precedence than ADD or SUB */
exp: exp1 { $$ = $1; }
   | exp AND exp1 { $$ = $1 & $3; }
   | exp XOR exp1 { $$ = $1 ^ $3; }
   | exp ABS exp1 { $$ = $1 | $3; }
   ;

exp1: factor {  $$ = $1; }
| exp1 ADD factor { $$ = $1 + $3; }
| exp1 SUB factor { $$ = $1 - $3; }
;

factor: term {  $$ = $1; }
| factor MUL term { $$ = $1 * $3; }
| factor DIV term { $$ = $1 / $3; }
;

term: NUMBER { $$ = $1; }
| ABS term { $$ = $2 > 0 ? $2 : - $2; }
| OP exp CP { $$ = $2; }
;

%%

int main() {
    yyparse();
}

int yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
}
