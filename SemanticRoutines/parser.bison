%start prog

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "ast.h"
    #include "symbol_table.h"
    #include "token.h"

    extern int yylex();
    extern char* yytext;
    struct stmt* parser_result = 0;
    int yyerror(const char *s);
%}

%token TOKEN_INT_TYPE
%token TOKEN_CHAR_TYPE
%token TOKEN_FLOAT_TYPE
%token TOKEN_VOID_TYPE
%token TOKEN_BOOL_TYPE
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_INT
%token TOKEN_FLOAT
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
%token TOKEN_COMMA
//%token stmt

%union {
    int val;
    char* name;
    struct type* type;
    struct decl* decl; 
    struct expr* expr;
    struct param_list* param;
    struct stmt* stmt;
}

%type <stmt> prog code stmt
%type <decl> decl func_decl
%type <expr> expr term factor conditions
%type <param> param_list
%type <type> type
%type <name> TOKEN_IDENT

%%

prog : code { parser_result = $1; }
    ;

code : stmt code    { $$ = $1; $1->next = $2; }
    |               { $$ = 0; }
    ;

stmt : decl TOKEN_SEMI                                                                                                          { $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0); }
    | func_decl TOKEN_LCURLY code TOKEN_RCURLY                                                                                  { $$ = stmt_create(STMT_FUNCTION, $1, 0, 0, 0, $3, 0, 0); }
    | TOKEN_IF TOKEN_LPAREN conditions TOKEN_RPAREN TOKEN_LCURLY code TOKEN_RCURLY                                              { $$ = stmt_create(STMT_IF_ELSE, 0, $3, 0, 0, $6, 0, 0); }
    | TOKEN_IF TOKEN_LPAREN conditions TOKEN_RPAREN TOKEN_LCURLY code TOKEN_RCURLY TOKEN_ELSE TOKEN_LCURLY code TOKEN_RCURLY    { $$ = stmt_create(STMT_IF_ELSE, 0, $3, 0, 0, $6, $10, 0); }
    | TOKEN_RETURN expr TOKEN_SEMI                                                                                              { $$ = stmt_create(STMT_RETURN, 0, 0, $2, 0, 0, 0, 0); }
    | TOKEN_IDENT TOKEN_ASSIGN expr TOKEN_SEMI                                                                                  { $$ = stmt_create(STMT_EXPR, 0, 0, expr_create(EXPR_ASSIGN, expr_create_name($1), $3), 0, 0, 0, 0); }
    ;

decl : type TOKEN_IDENT                                         { $$ = decl_create($2, $1, 0, 0, 0); }
    | type TOKEN_IDENT TOKEN_ASSIGN expr                        { $$ = decl_create($2, $1, $4, 0, 0); }
    ;

func_decl : type TOKEN_IDENT TOKEN_LPAREN param_list TOKEN_RPAREN     { $$ = decl_create($2, type_create(DECL_FUNCTION, $1, $4), 0, 0, 0); }
    ;

conditions : expr TOKEN_LT expr             { $$ = expr_create(EXPR_LT, $1, $3); }
    | expr TOKEN_GT expr                    { $$ = expr_create(EXPR_GT, $1, $3); }
    | expr TOKEN_LTE expr                   { $$ = expr_create(EXPR_LTE, $1, $3); }
    | expr TOKEN_GTE expr                   { $$ = expr_create(EXPR_GTE, $1, $3); }
    | expr TOKEN_EQEQ expr                  { $$ = expr_create(EXPR_EQEQ, $1, $3); }
    | expr TOKEN_NEQ expr                   { $$ = expr_create(EXPR_NEQ, $1, $3); }
    ;

param_list : type TOKEN_IDENT                       { $$ = param_list_create($2, $1, 0); }
    | type TOKEN_IDENT TOKEN_COMMA param_list       { $$ = param_list_create($2, $1, $4); }
    |                                               { $$ = 0; }
    ;

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
    | TOKEN_FLOAT               { $$ = expr_create_float_literal(atof(yytext));}
    | TOKEN_CHAR                { $$ = expr_create_char_literal(yytext[1]);}
    | TOKEN_IDENT               { $$ = expr_create_name($1);}
    | TOKEN_BOOL                { $$ = expr_create_bool_literal(yytext);}
    ;

%%

int yyerror(const char *s) {
    printf("Parse error: %s\n", s);
    return 1;
}