%start prog

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "ast.h"
    #include "token.h"

    extern int yylex();
    extern char* yytext;
    struct expr * parser_result = 0;
    struct decl * parser_result = 0;
    int yyerror(const char *s);
%}

%union {
    int val;
    struct stmt* stmt;
    struct type* type;
    struct decl* decl; 
    struct expr* expr;
}

%token TOKEN_INT_TYPE
%token TOKEN_CHAR_TYPE
%token TOKEN_FLOAT_TYPE
%token TOKEN_VOID_TYPE
%token TOKEN_BOOL_TYPE
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_INT
%token TOKEN_CHAR
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_MUL
%token TOKEN_DIV 
%token TOKEN_BOOL
%token TOKEN_TYPE
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
%token stmt

%type <decl> prog statement decl
%type <expr> expr term factor TOKEN_IDENT
%type <stmt> stmt
%type <type> type

%%

prog : statement { parser_result = $1; }
    ;

statement : decl TOKEN_SEMI statement   { $$ = $1; $1->next = $3;}
    |                                   { $$ = 0; }
    ;

decl : type TOKEN_IDENT                       { $$ = decl_create($2, $1, 0, 0, 0); }
    | type TOKEN_IDENT TOKEN_ASSIGN expr      { $$ = decl_create($2, $1, $4, 0, 0); }    

type : TOKEN_INT_TYPE   { $$ = type_create(DECL_INT, 0, 0); }
    | TOKEN_FLOAT_TYPE  { $$ = type_create(DECL_FLOAT, 0, 0); }
    | TOKEN_CHAR_TYPE   { $$ = type_create(DECL_CHAR, 0, 0); }
    | TOKEN_BOOL_TYPE   { $$ = type_create(DECL_BOOL, 0, 0); }
    | TOKEN_VOID_TYPE   { $$ = type_create(DECL_VOID, 0, 0); }

expr : expr TOKEN_PLUS term     { $$ = expr_create(EXPR_ADD, $1, $3);}
    | expr TOKEN_MINUS term     { $$ = expr_create(EXPR_SUB, $1, $3);}
    | term                      { $$ = $1; }
    ;

term : term TOKEN_MUL factor    { $$ = expr_create(EXPR_MUL, $1, $3);}
    | term TOKEN_DIV factor     { $$ = expr_create(EXPR_DIV, $1, $3);} 
    | factor                    { $$ = $1; }
    ;

factor : TOKEN_INT              { $$ = expr_create_int_literal(atoi(yytext));}
    | TOKEN_CHAR                { $$ = expr_create_char_literal(yytext);}
    | TOKEN_IDENT               { $$ = expr_create_name(yytext);}
    | TOKEN_BOOL                { $$ = expr_create_bool_literal(yytext);}
    ;

%%

int yyerror(const char *s) {
    printf("Parse error: %s\n", s);
    return 1;
}