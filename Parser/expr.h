#ifndef EXPR_H
#define EXPR_H
#include <cstdlib>

typedef enum{
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
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
void expr_print(struct expr *e);

#endif