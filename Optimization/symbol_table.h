#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

#define SCOPE_MAX_SYMBOLS 10
#define SCOPE_MAX_DEPTH   20
extern int resolve_errors;
extern int type_errors;

typedef enum {
    SYMBOL_LOCAL,
    SYMBOL_PARAM,
    SYMBOL_GLOBAL
} symbol_t;

struct symbol {
    symbol_t kind;
    struct type *type;
    char *name;
    int which;   
};

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name);

struct scope {
    struct symbol *symbols[SCOPE_MAX_SYMBOLS];
    int index_next_symbol;
};

void scope_enter();
void scope_exit();
void scope_bind(struct symbol *s);
struct symbol *scope_lookup(const char *name);
struct symbol *scope_lookup_current(const char *name);

void decl_resolve(struct decl *d);
void stmt_resolve(struct stmt *s);
void expr_resolve(struct expr *e);
void param_list_resolve(struct param_list *p);

struct type *expr_typecheck(struct expr *e);
void stmt_typecheck(struct stmt *s, struct type *return_type);
void decl_typecheck(struct decl *d);
void symbol_table_print();

#endif