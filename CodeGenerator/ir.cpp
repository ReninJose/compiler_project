#include "ir.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

static FILE *g_in;
static char  g_tok[256];

static void emit_type(FILE *out, struct type *t)
{
    if (!t) { fprintf(out, "VOID\n"); return; }
    switch (t->kind) {
        case DECL_INT:   fprintf(out, "INT\n");   return;
        case DECL_FLOAT: fprintf(out, "FLOAT\n"); return;
        case DECL_CHAR:  fprintf(out, "CHAR\n");  return;
        case DECL_BOOL:  fprintf(out, "BOOL\n");  return;
        case DECL_VOID:  fprintf(out, "VOID\n");  return;
        case DECL_FUNCTION: {
            int n = 0;
            for (struct param_list *p = t->params; p; p = p->next) n++;
            fprintf(out, "FUNCTION %d\n", n);
            emit_type(out, t->subtype);                    // return type
            for (struct param_list *p = t->params; p; p = p->next) {
                fprintf(out, "PARAM %s\n", p->name);
                emit_type(out, p->type);
            }
            return;
        }
    }
}

static void emit_expr(FILE *out, struct expr *e)
{
    if (!e) { fprintf(out, "NULL\n"); return; }
    switch (e->kind) {
        case EXPR_INT_LITERAL:   fprintf(out, "INT_LIT %d\n",  e->int_value);            return;
        case EXPR_FLOAT_LITERAL: fprintf(out, "FLOAT_LIT %g\n", e->float_value);         return;
        case EXPR_CHAR_LITERAL:  fprintf(out, "CHAR_LIT %d\n",  (int)e->char_value);     return;
        case EXPR_BOOL_LITERAL:  fprintf(out, "BOOL_LIT %d\n",  e->bool_value ? 1 : 0); return;
        case EXPR_NAME:          fprintf(out, "NAME %s\n",       e->name);               return;
        case EXPR_NOT:
            fprintf(out, "NOT\n");
            emit_expr(out, e->right);   // unary: only right child
            return;
        // Binary ops: tag then left then right
        case EXPR_ADD:    fprintf(out, "ADD\n");    break;
        case EXPR_SUB:    fprintf(out, "SUB\n");    break;
        case EXPR_MUL:    fprintf(out, "MUL\n");    break;
        case EXPR_DIV:    fprintf(out, "DIV\n");    break;
        case EXPR_AND:    fprintf(out, "AND\n");    break;
        case EXPR_OR:     fprintf(out, "OR\n");     break;
        case EXPR_EQEQ:   fprintf(out, "EQEQ\n");  break;
        case EXPR_NEQ:    fprintf(out, "NEQ\n");    break;
        case EXPR_LT:     fprintf(out, "LT\n");     break;
        case EXPR_GT:     fprintf(out, "GT\n");     break;
        case EXPR_LTE:    fprintf(out, "LTE\n");    break;
        case EXPR_GTE:    fprintf(out, "GTE\n");    break;
        case EXPR_ASSIGN: fprintf(out, "ASSIGN\n"); break;
    }
    emit_expr(out, e->left);
    emit_expr(out, e->right);
}

static void emit_stmt(FILE *out, struct stmt *s)
{
    if (!s) { fprintf(out, "NULL_STMT\n"); return; }

    switch (s->kind) {
        case STMT_DECL:
            fprintf(out, "STMT_DECL %s\n", s->decl->name);
            emit_type(out, s->decl->type);
            emit_expr(out, s->decl->value);       // NULL if no initialiser
            break;

        case STMT_EXPR:
            fprintf(out, "STMT_EXPR\n");
            emit_expr(out, s->expr);
            break;

        case STMT_RETURN:
            fprintf(out, "STMT_RETURN\n");
            emit_expr(out, s->expr);
            break;

        case STMT_FUNCTION:
            fprintf(out, "STMT_FUNCTION %s\n", s->decl->name);
            emit_type(out, s->decl->type);
            emit_stmt(out, s->body);              // inner list → ends with NULL_STMT
            break;

        case STMT_IF_ELSE:
            fprintf(out, "STMT_IF\n");
            emit_expr(out, s->init_expr);
            emit_stmt(out, s->body);              // then-body → ends with NULL_STMT
            emit_stmt(out, s->else_body);         // else-body → ends with NULL_STMT
            break;
    }

    emit_stmt(out, s->next);                      // sibling at same level
}

void ir_emit(FILE *out, struct stmt *root)
{
    fprintf(out, "# IR - generated AST\n");
    emit_stmt(out, root);
}

static void next_tok()
{
    // Skip comment lines (lines starting with '#')
    while (fscanf(g_in, "%255s", g_tok) == 1) {
        if (g_tok[0] == '#') {
            char buf[1024];
            if (fgets(buf, sizeof(buf), g_in) == nullptr) {}
            continue;
        }
        return;
    }
    g_tok[0] = '\0';
}

static struct type *read_type()
{
    next_tok();
    if (!strcmp(g_tok, "INT"))   return type_create(DECL_INT,   nullptr, nullptr);
    if (!strcmp(g_tok, "FLOAT")) return type_create(DECL_FLOAT, nullptr, nullptr);
    if (!strcmp(g_tok, "CHAR"))  return type_create(DECL_CHAR,  nullptr, nullptr);
    if (!strcmp(g_tok, "BOOL"))  return type_create(DECL_BOOL,  nullptr, nullptr);
    if (!strcmp(g_tok, "VOID"))  return type_create(DECL_VOID,  nullptr, nullptr);
    if (!strcmp(g_tok, "FUNCTION")) {
        int n; fscanf(g_in, "%d", &n);
        struct type *ret = read_type();
        struct param_list *head = nullptr, *tail = nullptr;
        for (int i = 0; i < n; i++) {
            next_tok();                            // "PARAM"
            next_tok();                            // name
            char *nm = strdup(g_tok);
            struct type *pt = read_type();
            struct param_list *p = param_list_create(nm, pt, nullptr);
            if (!head) head = tail = p;
            else       { tail->next = p; tail = p; }
        }
        return type_create(DECL_FUNCTION, ret, head);
    }
    fprintf(stderr, "IR read_type: unknown token '%s'\n", g_tok);
    return nullptr;
}

static struct expr *read_expr()
{
    next_tok();
    if (!strcmp(g_tok, "NULL"))      return nullptr;
    if (!strcmp(g_tok, "INT_LIT"))   { int   v; fscanf(g_in, "%d",  &v); return expr_create_int_literal(v); }
    if (!strcmp(g_tok, "FLOAT_LIT")) { float v; fscanf(g_in, "%f",  &v); return expr_create_float_literal(v); }
    if (!strcmp(g_tok, "CHAR_LIT"))  { int   v; fscanf(g_in, "%d",  &v); return expr_create_char_literal((char)v); }
    if (!strcmp(g_tok, "BOOL_LIT"))  { int   v; fscanf(g_in, "%d",  &v); return expr_create_bool_literal(v != 0); }
    if (!strcmp(g_tok, "NAME"))      { next_tok(); return expr_create_name(strdup(g_tok)); }
    if (!strcmp(g_tok, "NOT"))       { struct expr *r = read_expr(); return expr_create(EXPR_NOT, nullptr, r); }

    expr_t k;
    if      (!strcmp(g_tok,"ADD"))    k = EXPR_ADD;
    else if (!strcmp(g_tok,"SUB"))    k = EXPR_SUB;
    else if (!strcmp(g_tok,"MUL"))    k = EXPR_MUL;
    else if (!strcmp(g_tok,"DIV"))    k = EXPR_DIV;
    else if (!strcmp(g_tok,"AND"))    k = EXPR_AND;
    else if (!strcmp(g_tok,"OR"))     k = EXPR_OR;
    else if (!strcmp(g_tok,"EQEQ"))   k = EXPR_EQEQ;
    else if (!strcmp(g_tok,"NEQ"))    k = EXPR_NEQ;
    else if (!strcmp(g_tok,"LT"))     k = EXPR_LT;
    else if (!strcmp(g_tok,"GT"))     k = EXPR_GT;
    else if (!strcmp(g_tok,"LTE"))    k = EXPR_LTE;
    else if (!strcmp(g_tok,"GTE"))    k = EXPR_GTE;
    else if (!strcmp(g_tok,"ASSIGN")) k = EXPR_ASSIGN;
    else {
        fprintf(stderr, "IR read_expr: unknown token '%s'\n", g_tok);
        return nullptr;
    }
    struct expr *l = read_expr();
    struct expr *r = read_expr();
    return expr_create(k, l, r);
}

static struct stmt *read_stmt()
{
    next_tok();
    if (g_tok[0] == '\0' || !strcmp(g_tok, "NULL_STMT")) return nullptr;

    struct stmt *s = nullptr;

    if (!strcmp(g_tok, "STMT_DECL")) {
        next_tok();
        char *nm  = strdup(g_tok);
        struct type *t   = read_type();
        struct expr *val = read_expr();
        struct decl *d   = decl_create(nm, t, val, nullptr, nullptr);
        s = stmt_create(STMT_DECL, d, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }
    else if (!strcmp(g_tok, "STMT_EXPR")) {
        struct expr *e = read_expr();
        s = stmt_create(STMT_EXPR, nullptr, nullptr, e, nullptr, nullptr, nullptr, nullptr);
    }
    else if (!strcmp(g_tok, "STMT_RETURN")) {
        struct expr *e = read_expr();
        s = stmt_create(STMT_RETURN, nullptr, nullptr, e, nullptr, nullptr, nullptr, nullptr);
    }
    else if (!strcmp(g_tok, "STMT_FUNCTION")) {
        next_tok();
        char *nm       = strdup(g_tok);
        struct type *t = read_type();
        struct stmt *body = read_stmt();           // reads inner list until NULL_STMT
        struct decl *d    = decl_create(nm, t, nullptr, nullptr, nullptr);
        s = stmt_create(STMT_FUNCTION, d, nullptr, nullptr, nullptr, body, nullptr, nullptr);
    }
    else if (!strcmp(g_tok, "STMT_IF")) {
        struct expr *cond   = read_expr();
        struct stmt *then_b = read_stmt();         // reads until NULL_STMT
        struct stmt *else_b = read_stmt();         // reads until NULL_STMT (may be just NULL_STMT → nullptr)
        s = stmt_create(STMT_IF_ELSE, nullptr, cond, nullptr, nullptr, then_b, else_b, nullptr);
    }
    else {
        fprintf(stderr, "IR read_stmt: unknown token '%s'\n", g_tok);
        return nullptr;
    }

    if (s) s->next = read_stmt();                  // chain siblings at this level
    return s;
}

struct stmt *ir_read(FILE *in)
{
    g_in = in;
    return read_stmt();
}