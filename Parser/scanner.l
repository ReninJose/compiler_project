
%option noyywrap

%{
#include <iostream>
#include "token.h"
%}

DIGITS [0-9]
LETTERS [a-zA-Z]
IDENTIFIERS {LETTERS}({LETTERS}|{DIGITS}|_)*
TYPE (int|float|char|char*|bool|void)
IFELSE (if|else)

%%
"#include"[<\"]{IDENTIFIERS}"."{LETTERS}[>\"]   { return TOKEN_LIB; }
{TYPE}          { return TOKEN_TYPE; }
{IFELSE}        { return TOKEN_IFELSE; }
(true|false)    { return TOKEN_BOOL; }
"return"        { return TOKEN_RETURN; }
{IDENTIFIERS}   { return TOKEN_IDENT; }
"'"[a-zA-Z0-9]"'" { return TOKEN_CHAR; }
{DIGITS}+       { return TOKEN_INT; }
"+"             { return TOKEN_PLUS; }
"="             { return TOKEN_ASSIGN; }
"-"             { return TOKEN_MINUS; }
"*"             { return TOKEN_MUL; }
"/"             { return TOKEN_DIV; }
";"             { return TOKEN_SEMI; }
"=="            { return TOKEN_EQEQ; }
"!="            { return TOKEN_NEQ; }
"<="            { return TOKEN_LTE; }
">="            { return TOKEN_GTE; }
"<"             { return TOKEN_LT; }
">"             { return TOKEN_GT; }
"&&"            { return TOKEN_AND; }
"||"            { return TOKEN_OR; }
"!"             { return TOKEN_NOT; }
"("             { return TOKEN_LPAREN; }
")"             { return TOKEN_RPAREN; }
"{"             { return TOKEN_LCURLY; }
"}"             { return TOKEN_RCURLY; }
(" "|\t|\n)+    { /* ignore whitespace */ }
.               { return TOKEN_ERROR; }
%%