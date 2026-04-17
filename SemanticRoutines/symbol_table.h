#include "ast.h"  

typedef enum{
    SYMBOL_LOCAL,
    SYMBOL_PARAM,
    SYMBOL_GLOBAL
}symbol_t;

struct symbol{
    symbol_t kind;
    struct type *type;
    char *name;
    int which;
};

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name){
    struct symbol *s = (symbol*)malloc(sizeof(*s));
    s->kind = kind;
    s->name = name;
    s->type = type;
    return s; 
}

struct scope {
    struct symbol *scope[10];
    int index_next_symbol;	
}