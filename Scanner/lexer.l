%option c++
%option noyywrap

%{
#include<iostream>
#include<string>
#include"token.h"
std::string yylval;
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
KEYWORDS (int|float|char|if|else|void)
BOOLEANS (true|false)

/* Ranking matters */
%%
char\[{DIGITS}*\] { yylval = YYText(); return static_cast<int>(Token::KEYWORD); } /* Standalone string keyword to avoid mixing with char */
{KEYWORDS} { yylval = YYText(); return static_cast<int>(Token::KEYWORD); }
{BOOLEANS} { yylval = YYText(); return static_cast<int>(Token::BOOLEANS); }
{IDENTIFIERS} { yylval = YYText(); return static_cast<int>(Token::IDENTIFIERS); }
{OPERATORS} { yylval = YYText(); return static_cast<int>(Token::OPERATORS); }
{FLOAT} { yylval = YYText(); return static_cast<int>(Token::FLOAT_LITERAL); }
{INT} { yylval = YYText(); return static_cast<int>(Token::INT_LITERAL); }
{STR} { yylval = YYText(); return static_cast<int>(Token::STRING_LITERAL); }
{CHAR} { yylval = YYText(); return static_cast<int>(Token::CHAR_LITERAL); }
[ \t\n]+ {}
";" {}
. { yylval = YYText(); return static_cast<int>(Token::TOKEN_ERROR); }
%%


