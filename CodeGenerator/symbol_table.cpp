#include <iostream>
#include <cstring>
#include "symbol_table.h"

using namespace std;

int resolve_errors = 0;
int type_errors = 0;
static struct scope st[SCOPE_MAX_DEPTH];
static int st_top = -1;

static const char *kind_str(symbol_t k)
{
    switch (k) {
        case SYMBOL_LOCAL: return "local";
        case SYMBOL_PARAM: return "param";
        case SYMBOL_GLOBAL: return "global";
        default: return "?";
    }
}   // -1 means empty

void scope_enter()
{
    st_top++;
    if (st_top >= SCOPE_MAX_DEPTH) {
        cerr << "Error: maximum scope depth exceeded\n";
        st_top--;
        return;
    }
    // clear the new scope
    st[st_top].index_next_symbol = 0;
    for (int i = 0; i < SCOPE_MAX_SYMBOLS; i++)
        st[st_top].symbols[i] = nullptr;
}

void scope_exit()
{
    if (st_top < 0) return;

    // print this scope before it disappears
    struct scope &sc = st[st_top];
    cout << "\n--- Scope level " << st_top
         << (st_top == 0 ? " [global]" : " [local]") << " ---\n";
    if (sc.index_next_symbol == 0) {
        cout << "  (empty)\n";
    } else {
        for (int i = 0; i < sc.index_next_symbol; i++) {
            struct symbol *s = sc.symbols[i];
            cout << "  [" << kind_str(s->kind) << " #" << s->which << "] "
                 << s->name << " : ";
            type_print(s->type);
            cout << "\n";
        }
    }

    st_top--;
}

void scope_bind(struct symbol *s)
{
    if (st_top < 0) return;
    struct scope &cur = st[st_top];
    if (cur.index_next_symbol >= SCOPE_MAX_SYMBOLS) {
        cerr << "Error: too many symbols in scope (max " << SCOPE_MAX_SYMBOLS << ")\n";
        return;
    }
    cur.symbols[cur.index_next_symbol++] = s;
}

// Search only the current (innermost) scope
struct symbol *scope_lookup_current(const char *name)
{
    if (st_top < 0) return nullptr;
    struct scope &cur = st[st_top];
    for (int i = 0; i < cur.index_next_symbol; i++)
        if (strcmp(cur.symbols[i]->name, name) == 0)
            return cur.symbols[i];
    return nullptr;
}

// Search all scopes, innermost first
struct symbol *scope_lookup(const char *name)
{
    for (int level = st_top; level >= 0; level--) {
        struct scope &sc = st[level];
        for (int i = 0; i < sc.index_next_symbol; i++)
            if (strcmp(sc.symbols[i]->name, name) == 0)
                return sc.symbols[i];
    }
    return nullptr;
}

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name)
{
    struct symbol *s = (struct symbol *)malloc(sizeof(*s));
    s->kind  = kind;
    s->type  = type;
    s->name  = name;
    s->which = 0;
    return s;
}

void expr_resolve(struct expr *e)
{
    if (!e) return;

    if (e->kind == EXPR_NAME) {
        struct symbol *s = scope_lookup(e->name);
        if (!s) {
            // Error 1: undeclared variable
            cerr << "Resolve error: undeclared variable '" << e->name << "'\n";
            resolve_errors++;
        } else {
            e->symbol = s;
        }
        return;
    }
    expr_resolve(e->left);
    expr_resolve(e->right);
}

void param_list_resolve(struct param_list *p)
{
    if (!p) return;
    // Error 2: duplicate param name
    if (scope_lookup_current(p->name)) {
        cerr << "Resolve error: duplicate parameter '" << p->name << "'\n";
        resolve_errors++;
    } else {
        struct symbol *s = symbol_create(SYMBOL_PARAM, p->type, p->name);
        s->which = st[st_top].index_next_symbol;
        scope_bind(s);
        p->symbol = s;
    }
    param_list_resolve(p->next);
}

void decl_resolve(struct decl *d)
{
    if (!d) return;
    // Error 3: redeclaration in same scope
    if (scope_lookup_current(d->name)) {
        cerr << "Resolve error: '" << d->name << "' already declared in this scope\n";
        resolve_errors++;
    } else {
        symbol_t kind  = (st_top == 0) ? SYMBOL_GLOBAL : SYMBOL_LOCAL;
        struct symbol *s = symbol_create(kind, d->type, d->name);
        s->which = st[st_top].index_next_symbol;
        scope_bind(s);
        d->symbol = s;
    }
    expr_resolve(d->value);
}

void stmt_resolve(struct stmt *s)
{
    if (!s) return;

    switch (s->kind) {
        case STMT_DECL:
            decl_resolve(s->decl);
            break;

        case STMT_FUNCTION:
            decl_resolve(s->decl);       // bind function name in outer scope
            scope_enter();               // new scope for params + body
            param_list_resolve(s->decl->type->params);
            stmt_resolve(s->body);
            scope_exit();
            break;

        case STMT_EXPR:
            expr_resolve(s->expr);
            break;

        case STMT_RETURN:
            expr_resolve(s->expr);
            break;

        case STMT_IF_ELSE:
            expr_resolve(s->init_expr);
            scope_enter();
            stmt_resolve(s->body);
            scope_exit();
            if (s->else_body) {
                scope_enter();
                stmt_resolve(s->else_body);
                scope_exit();
            }
            break;
    }
    stmt_resolve(s->next);
}

static bool types_equal(struct type *a, struct type *b)
{
    if (!a || !b) return false;
    return a->kind == b->kind;
}

struct type *expr_typecheck(struct expr *e)
{
    if (!e) return nullptr;

    struct type *lt = expr_typecheck(e->left);
    struct type *rt = expr_typecheck(e->right);

    switch (e->kind) {
        case EXPR_INT_LITERAL:   return type_create(DECL_INT,   0, 0);
        case EXPR_FLOAT_LITERAL: return type_create(DECL_FLOAT, 0, 0);
        case EXPR_CHAR_LITERAL:  return type_create(DECL_CHAR,  0, 0);
        case EXPR_BOOL_LITERAL:  return type_create(DECL_BOOL,  0, 0);
        case EXPR_NAME:
            return (e->symbol) ? e->symbol->type : nullptr;

        case EXPR_ADD: case EXPR_SUB:
        case EXPR_MUL: case EXPR_DIV:
            if (lt && rt && !types_equal(lt, rt)) {
                cerr << "Type error: type mismatch in arithmetic\n";
                type_errors++;
            }
            return lt ? lt : rt;

        case EXPR_LT: case EXPR_GT: case EXPR_LTE:
        case EXPR_GTE: case EXPR_EQEQ: case EXPR_NEQ:
            if (lt && rt && !types_equal(lt, rt)) {
                cerr << "Type error: type mismatch in comparison\n";
                type_errors++;
            }
            return type_create(DECL_BOOL, 0, 0);

        case EXPR_AND: case EXPR_OR: case EXPR_NOT:
            return type_create(DECL_BOOL, 0, 0);

        case EXPR_ASSIGN:
            if (lt && rt && !types_equal(lt, rt)) {
                cerr << "Type error: type mismatch in assignment\n";
                type_errors++;
            }
            return lt;

        default: return nullptr;
    }
}

void decl_typecheck(struct decl *d)
{
    if (!d) return;
    if (d->value) {
        struct type *vt = expr_typecheck(d->value);
        if (vt && d->type && !types_equal(vt, d->type)) {
            cerr << "Type error: initialiser does not match declared type for '"
                 << d->name << "'\n";
            type_errors++;
        }
    }
}

void stmt_typecheck(struct stmt *s, struct type *ret_type)
{
    if (!s) return;

    switch (s->kind) {
        case STMT_DECL:
            decl_typecheck(s->decl);
            break;
        case STMT_FUNCTION:
            stmt_typecheck(s->body, s->decl->type->subtype);
            break;
        case STMT_EXPR:
            expr_typecheck(s->expr);
            break;
        case STMT_RETURN: {
            struct type *got = expr_typecheck(s->expr);
            if (ret_type && got && !types_equal(got, ret_type)) {
                cerr << "Type error: return type mismatch\n";
                type_errors++;
            }
            break;
        }
        case STMT_IF_ELSE: {
            struct type *cond = expr_typecheck(s->init_expr);
            if (cond && cond->kind != DECL_BOOL) {
                cerr << "Type error: if-condition must be bool\n";
                type_errors++;
            }
            stmt_typecheck(s->body, ret_type);
            stmt_typecheck(s->else_body, ret_type);
            break;
        }
    }
    stmt_typecheck(s->next, ret_type);
}

void symbol_table_print()
{
    cout << "\n=== Symbol Table ===\n";
    for (int level = 0; level <= st_top; level++) {
        cout << "Scope " << level
             << (level == 0 ? " [global]" : " [local]") << ":\n";
        struct scope &sc = st[level];
        if (sc.index_next_symbol == 0) {
            cout << "  (empty)\n";
            continue;
        }
        for (int i = 0; i < sc.index_next_symbol; i++) {
            struct symbol *s = sc.symbols[i];
            cout << "  [" << kind_str(s->kind) << " #" << s->which << "] "
                 << s->name << " : ";
            type_print(s->type);
            cout << "\n";
        }
    }
    cout << "====================\n";
}