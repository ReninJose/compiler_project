#ifndef EXPR_H
#define EXPR_H
#include <cstdlib>

typedef enum{
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_AND,
    EXPR_OR,
    EXPR_LTE,
    EXPR_GTE,
    EXPR_LT,
    EXPR_GT,
    EXPR_EQEQ,
    EXPR_NEQ,
    EXPR_NOT,
    EXPR_ASSIGN,
    EXPR_VALUE,
    EXPR_CHAR,
    EXPR_BOOL,
    EXPR_NAME
} expr_type;

typedef struct expr
{
    expr_type kind;
    struct expr *left;
    struct expr *right;
    int value;
    char* char_value;
    bool bool_value;
    char* name;
}expr;

struct expr* expr_create_arithmetic(expr_type kind, struct expr *left, struct expr *right);
struct expr* expr_create_value(int value);
struct expr* expr_create_char(char* value);
struct expr* expr_create_identifier(char* name);
struct expr* expr_create_rel(expr_type kind, expr* a, expr* b);
struct expr* expr_create_logic(expr_type kind, expr* a, expr* b);
struct expr* expr_create_assign(struct expr* id, struct expr* value);
struct expr* expr_create_bool(bool value);
struct expr* expr_create_unary(expr_type kind, expr* a);
void expr_print(struct expr *e);

#endif