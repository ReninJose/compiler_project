%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "expr.h"

    extern int yylex();
    extern char* yytext;
    struct expr * parser_result = 0;
    void yyerror(const char *s);
%}

%union {
    struct expr* expr;
}

%token TOKEN_INT
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_MUL
%token TOKEN_DIV
%token TOKEN_ERROR

%type <expr> expr term factor

%%
expr : expr TOKEN_PLUS term     { $$ = expr_arithmetic_create(EXPR_ADD, $1, $3);}
    | expr TOKEN_MINUS term     { $$ = expr_arithmetic_create(EXPR_SUB, $1, $3);}
    | term                      { $$ = $1; }
    ;

term : term TOKEN_MUL factor    { $$ = expr_arithmetic_create(EXPR_MUL, $1, $3);}
    | term TOKEN_DIV factor     { $$ = expr_arithmetic_create(EXPR_DIV, $1, $3);} 
    | factor                    { $$ = $1; }
    ;

factor : TOKEN_INT              { $$ = expr_create_value(atoi(yytext)); }
    ;
%%

void yyerror(const char *s) {
    printf("Parse error: %s\n", s);
}