%start prog

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "expr.h"

    extern int yylex();
    extern char* yytext;
    struct expr * parser_result = 0;
    int yyerror(const char *s);
%}

%union {
    int val;
    struct expr* expr;
}

%token TOKEN_INT
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_MUL
%token TOKEN_DIV 
%token TOKEN_KEYWORD
%token TOKEN_SEMI
%token TOKEN_ERROR
%token TOKEN_EQEQ
%token TOKEN_NEQ
%token TOKEN_LTE
%token TOKEN_GTE
%token TOKEN_LT
%token TOKEN_GT
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_NOT
%token TOKEN_TRUE
%token TOKEN_FALSE
%token TOKEN_LPAREN
%token TOKEN_RPAREN

%type <expr> expr term factor
%type <expr> rel_expr
%type <expr> bool_expr
%type <expr> logical_expr

%%

prog : expr TOKEN_SEMI { parser_result = $1; expr_print(parser_result); } 
    ; 


/*ARITHMETIC*/

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

int yyerror(const char *s) {
    printf("Parse error: %s\n", s);
    return 1;
}