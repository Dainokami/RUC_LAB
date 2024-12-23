%option noyywrap
%{
#include <stdio.h>

int lineno = 1; // 行号
int colno = 1;  // 列号
%}
%x COMMENT

%%

"/*"          { colno += strlen(yytext);BEGIN(COMMENT); }     
<COMMENT>"*/" { colno += strlen(yytext);BEGIN(INITIAL); }      
<COMMENT>.    {colno += strlen(yytext);}
<COMMENT>\n   {lineno++; colno = 1;}


\n      {lineno++; colno = 1; } 
"//".*    {}


"int" {
    printf("%-25s%-3d   (%d,%d)\n", "int", lineno, colno, colno + strlen(yytext) - 1); 
    colno += strlen(yytext);
}
"void" {
    printf("%-25s%-3d   (%d,%d)\n", "void", lineno, colno, colno + strlen(yytext) - 1); 
    colno += strlen(yytext);
}
"const" {
    printf("%-25s%-3d   (%d,%d)\n", "const", lineno, colno, colno + strlen(yytext) - 1); 
    colno += strlen(yytext);
}
"if" {
    printf("%-25s%-3d   (%d,%d)\n", "if", lineno, colno, colno + strlen(yytext) - 1); 
    colno += strlen(yytext);
}
"while" {
    printf("%-25s%-3d   (%d,%d)\n", "while", lineno, colno, colno + strlen(yytext) - 1); 
    colno += strlen(yytext);
}
"break" {
    printf("%-25s%-3d   (%d,%d)\n", "break", lineno, colno, colno + strlen(yytext) - 1); 
    colno += strlen(yytext);
}
"continue" {
    printf("%-25s%-3d   (%d,%d)\n", "continue", lineno, colno, colno + strlen(yytext) - 1); 
    colno += strlen(yytext);
}
"return" {
    printf("%-25s%-3d   (%d,%d)\n", "return", lineno, colno, colno + strlen(yytext) - 1); 
    colno += strlen(yytext);
}
"+"|"-"|"*"|"/"|"="|"%"|"=="|"!="|"<"|">"|"<="|">="|"!"|"&&"|"||"|"("|")"|";"|"{"|"}"|"["|"]"|"," {
    printf("%-25s%-3d   (%d,%d)\n", yytext, lineno, colno, colno + strlen(yytext) - 1); 
    colno += strlen(yytext);
}

[a-zA-Z_][a-zA-Z0-9_]* {
            printf("%-25s%-3d   (%d,%d)             Ident\n", yytext, lineno, colno,colno + strlen(yytext));    
            colno += strlen(yytext); 
            }


0[0-7]*|[1-9][0-9]*|0x[1-9a-fA-F][0-9a-fA-F]*|0x0|0X0|0X[1-9a-fA-F][0-9a-fA-F]*  {
            printf("%-25s%-3d   (%d,%d)             IntConst\n", yytext, lineno, colno,colno + strlen(yytext));
            colno += strlen(yytext); 
            }

[0-9][a-zA-Z0-9_]+ {
            printf("%s  This is a wrong Ident\n",yytext);
            colno += strlen(yytext); 
            }

.       { colno += strlen(yytext); } 

%%


int main(int argc, char *argv[]) {
    printf("Text                  Line   Column(Start,End)   Type\n");
    yyin = fopen("1.sy", "r");
    yylex();
    return 0;
}