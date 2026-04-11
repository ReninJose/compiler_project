
%option noyywrap

%{
#include <iostream>
#include "token.h"
%}

DIGITS [0-9]
KEYWORDS (int|float|char|bool|if|else|void)
BOOLEANS (true|false)

%%
{KEYWORDS} { return TOKEN_KEYWORD; }
{DIGITS}+      { return TOKEN_INT; }
"+"         { return TOKEN_PLUS; }
"-"         { return TOKEN_MINUS; }
"*"         { return TOKEN_MUL; }
"/"         { return TOKEN_DIV; }
";"         { return TOKEN_SEMI; }
"=="        { return TOKEN_EQEQ; }
"!="        { return TOKEN_NEQ; }
"<="        { return TOKEN_LTE; }
">="        { return TOKEN_GTE; }
"<"         { return TOKEN_LT; }
">"         { return TOKEN_GT; }
"&&"        { return TOKEN_AND; }
"||"        { return TOKEN_OR; }
"!"         { return TOKEN_NOT; }
"("         { return TOKEN_LPAREN; }
")"         { return TOKEN_RPAREN; }
[ \t\n]+    { /* ignore whitespace */ }
.           { return TOKEN_ERROR; }
%%