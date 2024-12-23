%option noyywrap
%{

#include <stdio.h>
#include <stdlib.h>

%}

digit [0-9]

%%


{digit}* { 
    printf ("%s, %d\n", yytext, yyleng);
}

. {
    printf ("%s: ?\n", yytext);
}

%%

int main(int argc, char *argv[]) {
    yyin = stdin;
    yylex();
    return 0;
}