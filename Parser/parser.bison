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
%token TOKEN_CHAR
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_MUL
%token TOKEN_DIV 
%token TOKEN_BOOL
%token TOKEN_TYPE
%token TOKEN_IFELSE
%token TOKEN_IDENT
%token TOKEN_ASSIGN
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
%token TOKEN_LCURLY
%token TOKEN_RCURLY
%token TOKEN_RETURN
%token TOKEN_LIB

%type <expr> expr term factor TOKEN_IDENT statement rel_expr logical_expr decl assignment

%left TOKEN_OR
%left TOKEN_AND
%right TOKEN_NOT
%left TOKEN_EQEQ TOKEN_NEQ
%left TOKEN_LT TOKEN_LTE TOKEN_GT TOKEN_GTE
%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_MUL TOKEN_DIV

%%

prog : statement  { parser_result = $1; } 
    ; 

statement : assignment                                                              { $$ = $1; }
    | decl                                                                          { $$ = $1; }
    | TOKEN_LIB                                                                     { $$ = nullptr; }
    | TOKEN_TYPE TOKEN_IDENT TOKEN_LPAREN TOKEN_RPAREN                              { $$ = nullptr; }
    | TOKEN_TYPE TOKEN_IDENT TOKEN_LPAREN TOKEN_RPAREN TOKEN_LCURLY                 { $$ = nullptr; }
    | TOKEN_TYPE TOKEN_IDENT TOKEN_LPAREN TOKEN_RPAREN TOKEN_LCURLY TOKEN_RCURLY    { $$ = nullptr; }
    | TOKEN_RCURLY                                                                  { $$ = nullptr; }
    | TOKEN_LCURLY                                                                  { $$ = nullptr; }
    | TOKEN_RETURN TOKEN_IDENT TOKEN_SEMI                                           { $$ = nullptr; }
    | TOKEN_RETURN TOKEN_INT TOKEN_SEMI                                             { $$ = nullptr; }
    | TOKEN_RETURN TOKEN_BOOL TOKEN_SEMI                                            { $$ = nullptr; }
    | TOKEN_RETURN logical_expr TOKEN_SEMI                                          { $$ = $2; }
    | TOKEN_IFELSE TOKEN_LPAREN logical_expr TOKEN_RPAREN TOKEN_LCURLY              { $$ = $3; }
    ;

assignment : TOKEN_IDENT TOKEN_ASSIGN logical_expr TOKEN_SEMI { $$ = expr_create_assign($1, $3); }
    ;

decl : TOKEN_TYPE TOKEN_IDENT TOKEN_ASSIGN logical_expr TOKEN_SEMI  { $$ = expr_create_assign($2, $4); }
    | TOKEN_TYPE TOKEN_IDENT TOKEN_SEMI                             { $$ = expr_create_assign($2, NULL); }
    ;

/*LOGIC*/

logical_expr : logical_expr TOKEN_AND logical_expr  { $$ = expr_create_logic(EXPR_AND, $1, $3); }
    | logical_expr TOKEN_OR logical_expr            { $$ = expr_create_logic(EXPR_OR, $1, $3); }
    | TOKEN_NOT logical_expr                        { $$ = expr_create_unary(EXPR_NOT, $2); }
    | rel_expr                                      { $$ = $1; }
    ;

/*RELATION*/

rel_expr : expr TOKEN_LT expr           { $$ = expr_create_rel(EXPR_LT, $1, $3); }
    | expr TOKEN_GT expr                { $$ = expr_create_rel(EXPR_GT, $1, $3); }
    | expr TOKEN_LTE expr               { $$ = expr_create_rel(EXPR_LTE, $1, $3); }
    | expr TOKEN_GTE expr               { $$ = expr_create_rel(EXPR_GTE, $1, $3); }
    | expr TOKEN_EQEQ expr              { $$ = expr_create_rel(EXPR_EQEQ, $1, $3); }
    | expr TOKEN_NEQ expr               { $$ = expr_create_rel(EXPR_NEQ, $1, $3); }
    | TOKEN_LPAREN expr TOKEN_RPAREN    { $$ = $2; }
    | expr                              { $$ = $1; }
    ;

/*ARITHMETIC*/

expr : expr TOKEN_PLUS term     { $$ = expr_create_arithmetic(EXPR_ADD, $1, $3);}
    | expr TOKEN_MINUS term     { $$ = expr_create_arithmetic(EXPR_SUB, $1, $3);}
    | term                      { $$ = $1; }
    ;

term : term TOKEN_MUL factor    { $$ = expr_create_arithmetic(EXPR_MUL, $1, $3);}
    | term TOKEN_DIV factor     { $$ = expr_create_arithmetic(EXPR_DIV, $1, $3);} 
    | factor                    { $$ = $1; }
    ;

factor : TOKEN_INT              { $$ = expr_create_value(atoi(yytext)); }
    | TOKEN_CHAR                { $$ = expr_create_char(yytext); }
    | TOKEN_IDENT               { $$ = expr_create_identifier(yytext); }
    | TOKEN_BOOL                { $$ = expr_create_bool(yytext); }
    ;

%%

int yyerror(const char *s) {
    printf("Parse error: %s\n", s);
    return 1;
}