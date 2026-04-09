
%option noyywrap

%{
#include <iostream>
#include "token.h"
%}

%%

[0-9]+      { return TOKEN_INT; }

"+"         { return TOKEN_PLUS; }
"-"         { return TOKEN_MINUS; }
"*"         { return TOKEN_MUL; }
"/"         { return TOKEN_DIV; }

[ \t\n]+    { /* ignore whitespace */ }

.           { return TOKEN_ERROR; }

%%