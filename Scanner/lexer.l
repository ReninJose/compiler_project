%option c++
%option noyywrap

%{
#include<iostream>
#include"token.h"
%}

WHITESPACE [ ]
DIGITS [0-9]
LETTERS [a-zA-Z]
INT [+-]?{DIGITS}+
FLOAT [+-]?{DIGITS}+\.{DIGITS}+
CHAR \'({LETTERS}|{DIGITS}|{WHITESPACE}|_)\'
IDENTIFIERS {LETTERS}({LETTERS}|{DIGITS}|_)*
STR \"({LETTERS}|{DIGITS}|{WHITESPACE}|_)+\"
OPERATORS (=|==|<|>|<=|>=|\+|\-|\*|\/)
KEYWORDS (int|float|char|bool|if|else|void)
BOOLEANS (true|false)

/* Ranking matters */
%%
char\[{DIGITS}*\] { return KEYWORD; } /* Standalone string keyword to avoid mixing with char */
{KEYWORDS} { return KEYWORD; }
{BOOLEANS} { return BOOLEANS; }
{IDENTIFIERS} { return IDENTIFIERS; }
{OPERATORS} { return OPERATORS; }
{FLOAT} { return FLOAT_LITERAL; }
{INT} { return INT_LITERAL; }
{STR} { return STRING_LITERAL; }
{CHAR} { return CHAR_LITERAL; }
[ \t\n]+ {}
";" {}
. { return TOKEN_ERROR; }
%%
