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
    // EXPR_REL,
    // EXPR_BOOL,
    // EXPR_LOGIC,
    EXPR_NOT,
    // EXPR_IF,
    EXPR_VALUE
} expr_type;

typedef struct expr
{
    expr_type kind;
    struct expr *left;
    struct expr *right;
    int value;
}expr;

struct expr* expr_arithmetic_create(expr_type kind, struct expr *left, struct expr *right);
struct expr* expr_create_value(int value);
struct expr* expr_create_rel(expr_type kind, expr* a, expr* b);
struct expr* expr_create_logic(expr_type kind, expr* a, expr* b);
struct expr* expr_create_bool(int v);
struct expr* expr_create_unary(expr_type kind, expr* a);
void expr_print(struct expr *e);

#endif