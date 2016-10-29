%{
int chars=0;
int words=0;
int lines=0;
%}

%%

[a-zA-Z]+	{	words++;	chars += strlen(yytext);}
\n			{	chars++;	lines++;}
.			{	chars++;}

%%

main(int argc,char **argv)
{
	yylex();
	printf("lines: %8d\twords: %8d\tchars: %8d\n",lines,words,chars);
}

