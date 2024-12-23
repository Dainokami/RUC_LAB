%option noyywrap
%{
#include <stdio.h>
#include "parser.tab.hpp" // 确保包含了正确的Bison产生的头文件

int lineno = 1; // 行号
int colno = 1;  // 列号
%}
%x COMMENT

%%
"/*"          { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }      
<COMMENT>.    {}
<COMMENT>\n   { lineno++; colno = 1; }

\n      {printf("%s", yytext); lineno++; colno = 1;}
"//".* {} 

"#include"    {}
"<stdio.h>"   {}
"int"         {printf("%s", yytext); return INT; }
"void"        {printf("%s", yytext); return VOID; }
"const"       {printf("%s", yytext); return CONST; }
"if"          {printf("%s", yytext); return IF; }
"while"       {printf("%s", yytext); return WHILE; }
"break"       {printf("%s", yytext); return BREAK; }
"continue"    {printf("%s", yytext); return CONTINUE; }
"return"      {printf("%s", yytext); return RETURN; }
"else"        {printf("%s", yytext); return ELSE; }
"printf"      {printf("%s", yytext); return PRINTF; }
"scanf"       {printf("%s", yytext); return SCANF; }
"main"        {printf("%s", yytext); return MAIN; }

"==" {printf("%s", yytext); return EQ; }
"!=" {printf("%s", yytext); return NEQ; }
"<=" {printf("%s", yytext); return LEQ; } 
">=" {printf("%s", yytext); return GEQ; } 
"&&" {printf("%s", yytext); return AND; }
"||" {printf("%s", yytext); return OR; }


"+"|"-"|"*"|"/"|"="|"%"|"<"|">"|"!"|"("|")"|";"|"{"|"}"|"["|"]"|","|"&" {printf("%s", yytext);
    return yytext[0]; 
}

\"([^\"\\]|\\.)*\"   { 
    printf("%s", yytext);
    yylval.val = strdup(yytext);
    return STRING; 
}

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("%s", yytext);
    yylval.val = strdup(yytext);
    return Ident; 
}

0[0-7]*|[1-9][0-9]*|0x[1-9a-fA-F][0-9a-fA-F]*|0x0|0X0|0X[1-9a-fA-F][0-9a-fA-F]* {
    printf("%s", yytext);
    yylval.num = strtol(yytext, NULL, 0); 
    return INTCONST; 
}

[0-9][a-zA-Z0-9_]+ {
    printf("%s", yytext);
    yylval.val = strdup(yytext);
    return WrongIdent; 
}


" "       {printf("%s", yytext); colno++; } 
\t		{printf(" %s", yytext);colno += 8;}
. {printf("%s", yytext);} // 返回错误token

%%
