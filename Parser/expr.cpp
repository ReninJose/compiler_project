#include "expr.h"
#include <stdio.h>
#include <stdlib.h>

struct expr* expr_arithmetic_create(expr_type kind, expr *left, expr *right){
    struct expr *buffer = (expr*) malloc(sizeof(expr));
    buffer->kind = kind;
    buffer->value = 0;  //init value 
    buffer->left = left;
    buffer->right = right;
    return buffer;
}

struct expr* expr_create_value(int value){
    struct expr *buffer = (expr*) malloc(sizeof(expr));
    buffer->value = value;
    buffer->left = NULL;
    buffer->right = NULL;
    return buffer;
}

void expr_print(struct expr *e){
    if(!e) return;
    printf("(");
    expr_print(e->left);
    switch (e->kind)
    {
        case EXPR_VALUE:    printf("%d",e->value); break;
        case EXPR_ADD:      printf("+"); break;
        case EXPR_SUB:      printf("-"); break;
		case EXPR_MUL:      printf("*"); break;
		case EXPR_DIV: 	    printf("/"); break;
    default:
        break;
    }
    expr_print(e->right);
    printf(")");
}