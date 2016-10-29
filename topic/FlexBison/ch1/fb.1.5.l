%{
#include "fb.1.5.tab.h"
%}

%%
"+" {return ADD;}
"-" {return SUB;}
"*" {return MUL;}
"/" {return DIV;}
"(" {return OP;}
")" {return CP;}
"|" {return ABS;}
"&" {return AND;}
"^" {return XOR;}
[-+]?[0-9]+ {yylval = atoi(yytext); return NUMBER; /* support sign char */}
0x[a-f0-9]+ {
    yylval = strtol(yytext, NULL, 16); return NUMBER;
    /* exer1.2 support hex number */
}
\n {return EOL;}
[ \t]
"//".*
. {printf("unknown char %c\n", *yytext);}
%%
