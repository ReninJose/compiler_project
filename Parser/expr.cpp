#include "expr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct expr* expr_create_arithmetic(expr_type kind, expr *left, expr *right){
    struct expr *buffer = (expr*) malloc(sizeof(expr));
    buffer->kind = kind;
    buffer->value = 0;  //init value 
    buffer->bool_value = NULL;
    buffer->left = left;
    buffer->right = right;
    return buffer;
}

struct expr* expr_create_value(int value){
    struct expr *buffer = (expr*) malloc(sizeof(expr));
    buffer->kind = EXPR_VALUE;
    buffer->value = value;
    buffer->char_value = NULL;
    buffer->bool_value = NULL;
    buffer->name = NULL;
    buffer->left = NULL;
    buffer->right = NULL;
    return buffer;
}

struct expr* expr_create_char(char* value){
    struct expr *buffer = (expr*) malloc(sizeof(expr));
    buffer->kind = EXPR_CHAR;
    buffer->value = NULL;
    buffer->char_value = value;
    buffer->bool_value = NULL;
    buffer->name = NULL;
    buffer->left = NULL;
    buffer->right = NULL;
    return buffer;
}

struct expr* expr_create_bool(bool value){
    struct expr *buffer = (expr*) malloc(sizeof(expr));
    buffer->kind = EXPR_BOOL;
    buffer->value = NULL;
    buffer->char_value = NULL;
    buffer->bool_value = value;
    buffer->name = NULL;
    buffer->left = NULL;
    buffer->right = NULL;
    return buffer;
}

struct expr* expr_create_identifier(char* name){
    printf("test");
    struct expr *buffer = (expr*) malloc(sizeof(expr));
    buffer->kind = EXPR_NAME;
    buffer->value = NULL;
    buffer->name = strdup(name);
    buffer->bool_value = NULL;
    buffer->left = NULL;
    buffer->right = NULL;
    return buffer;
}

struct expr* expr_create_rel(expr_type kind, expr* l, expr* r) {
    expr *n = (expr*)malloc(sizeof(expr));
    n->kind = kind;
    n->left = l;
    n->right = r;
    return n;
}

struct expr* expr_create_logic(expr_type kind, expr* l, expr* r) {
    expr *n = (expr*)malloc(sizeof(expr));
    n->kind = kind;
    n->left = l;
    n->right = r;
    return n;
}

struct expr* expr_create_unary(expr_type kind, expr* a) {
    expr *n = (expr*)malloc(sizeof(expr));
    n->kind = kind;
    n->left = a;
    n->right = NULL;
    return n;
}

void expr_print(struct expr *e) {
    if (!e) return;

    printf("(");
    expr_print(e->left);

    switch (e->kind) {
        case EXPR_VALUE:
            printf("%d", e->value);
            break;

        case EXPR_CHAR:
            printf("%s", e->char_value);
            break;
        
        case EXPR_BOOL:
            printf("%d", e->bool_value);
            break;

        case EXPR_NAME:
            printf("%s", e->name);
            break;

        case EXPR_NOT:
            printf("(!");
            // expr_print(e->left);
            // printf(")");
            break;

        case EXPR_ASSIGN:   printf("Assignment/Declaration line: "); break;
        case EXPR_ADD:      printf("+"); break;
        case EXPR_SUB:      printf("-"); break;
        case EXPR_MUL:      printf("*"); break;
        case EXPR_DIV:      printf("/"); break;
        case EXPR_AND:      printf("&&"); break;
        case EXPR_OR:       printf("||"); break;
        case EXPR_EQEQ:     printf("=="); break;
        case EXPR_NEQ:      printf("!="); break;
        case EXPR_LTE:      printf("<="); break;
        case EXPR_GTE:      printf(">="); break;
        case EXPR_LT:       printf("<"); break;
        case EXPR_GT:       printf(">"); break;

    }

    expr_print(e->right);
    printf(")");
    return;
}