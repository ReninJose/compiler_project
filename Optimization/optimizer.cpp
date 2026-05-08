#include "optimizer.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <string>

using namespace std;

// This file is created using the help of AI generated code

// ── counters ─────────────────────────────────────────────────────────────────
int opt_cf_count    = 0;
int opt_sr_count    = 0;
int opt_hoist_count = 0;
int opt_dce_count   = 0;

void opt_print_stats() {
    printf("\n=== Optimizer Statistics ===\n");
    printf("  Constant Folding      : %d transformation(s)\n", opt_cf_count);
    printf("  Strength Reduction    : %d transformation(s)\n", opt_sr_count);
    printf("  Code Hoisting         : %d declaration(s) hoisted\n", opt_hoist_count);
    printf("  Dead Code Elimination : %d statement(s) removed\n", opt_dce_count);
    printf("============================\n\n");
}

// ─────────────────────────────────────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────────────────────────────────────

static bool is_int_const(struct expr *e) {
    return e && e->kind == EXPR_INT_LITERAL;
}

static bool is_bool_const(struct expr *e) {
    return e && e->kind == EXPR_BOOL_LITERAL;
}

static bool is_literal(struct expr *e) {
    return e && (e->kind == EXPR_INT_LITERAL  ||
                 e->kind == EXPR_BOOL_LITERAL ||
                 e->kind == EXPR_CHAR_LITERAL ||
                 e->kind == EXPR_FLOAT_LITERAL);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Pass 1 – Constant Folding
// ─────────────────────────────────────────────────────────────────────────────

struct expr* opt_constant_fold(struct expr* e) {
    if (!e) return nullptr;

    e->left  = opt_constant_fold(e->left);
    e->right = opt_constant_fold(e->right);

    // Unary NOT
    if (e->kind == EXPR_NOT && is_int_const(e->right)) {
        opt_cf_count++;
        return expr_create_int_literal(e->right->int_value == 0 ? 1 : 0);
    }

    if (!is_int_const(e->left) || !is_int_const(e->right)) return e;

    int L = e->left->int_value;
    int R = e->right->int_value;
    int result;
    bool is_cmp = false;

    switch (e->kind) {
        case EXPR_ADD:  result = L + R; break;
        case EXPR_SUB:  result = L - R; break;
        case EXPR_MUL:  result = L * R; break;
        case EXPR_DIV:  if (R == 0) return e; result = L / R; break;
        case EXPR_LT:   result = (L <  R) ? 1 : 0; is_cmp = true; break;
        case EXPR_GT:   result = (L >  R) ? 1 : 0; is_cmp = true; break;
        case EXPR_LTE:  result = (L <= R) ? 1 : 0; is_cmp = true; break;
        case EXPR_GTE:  result = (L >= R) ? 1 : 0; is_cmp = true; break;
        case EXPR_EQEQ: result = (L == R) ? 1 : 0; is_cmp = true; break;
        case EXPR_NEQ:  result = (L != R) ? 1 : 0; is_cmp = true; break;
        case EXPR_AND:  result = (L && R) ? 1 : 0; break;
        case EXPR_OR:   result = (L || R) ? 1 : 0; break;
        default: return e;
    }

    opt_cf_count++;
    return is_cmp ? expr_create_bool_literal(result != 0)
                  : expr_create_int_literal(result);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Pass 2 – Strength Reduction
// ─────────────────────────────────────────────────────────────────────────────

struct expr* opt_strength_reduce(struct expr* e) {
    if (!e) return nullptr;

    e->left  = opt_strength_reduce(e->left);
    e->right = opt_strength_reduce(e->right);

    struct expr *L = e->left;
    struct expr *R = e->right;

    switch (e->kind) {
        case EXPR_MUL:
            if (is_int_const(R) && R->int_value == 0) { opt_sr_count++; return expr_create_int_literal(0); }
            if (is_int_const(L) && L->int_value == 0) { opt_sr_count++; return expr_create_int_literal(0); }
            if (is_int_const(R) && R->int_value == 1) { opt_sr_count++; return L; }
            if (is_int_const(L) && L->int_value == 1) { opt_sr_count++; return R; }
            if (is_int_const(R) && R->int_value == 2) { opt_sr_count++; return expr_create(EXPR_ADD, L, L); }
            if (is_int_const(L) && L->int_value == 2) { opt_sr_count++; return expr_create(EXPR_ADD, R, R); }
            if (is_int_const(R) && R->int_value == 4) {
                opt_sr_count++;
                struct expr *xx = expr_create(EXPR_ADD, L, L);
                return expr_create(EXPR_ADD, xx, xx);
            }
            break;
        case EXPR_ADD:
            if (is_int_const(R) && R->int_value == 0) { opt_sr_count++; return L; }
            if (is_int_const(L) && L->int_value == 0) { opt_sr_count++; return R; }
            break;
        case EXPR_SUB:
            if (is_int_const(R) && R->int_value == 0) { opt_sr_count++; return L; }
            break;
        case EXPR_DIV:
            if (is_int_const(R) && R->int_value == 1) { opt_sr_count++; return L; }
            if (is_int_const(L) && L->int_value == 0) { opt_sr_count++; return expr_create_int_literal(0); }
            break;
        case EXPR_EQEQ:
            if (L && R && L->kind == EXPR_NAME && R->kind == EXPR_NAME &&
                strcmp(L->name, R->name) == 0) { opt_sr_count++; return expr_create_bool_literal(true); }
            break;
        case EXPR_NEQ:
            if (L && R && L->kind == EXPR_NAME && R->kind == EXPR_NAME &&
                strcmp(L->name, R->name) == 0) { opt_sr_count++; return expr_create_bool_literal(false); }
            break;
        default: break;
    }
    return e;
}


// Returns true if two expressions are identical constant literals with the same value
static bool same_const(struct expr *a, struct expr *b) {
    if (!a || !b) return false;
    if (a->kind != b->kind) return false;
    switch (a->kind) {
        case EXPR_INT_LITERAL:  return a->int_value   == b->int_value;
        case EXPR_BOOL_LITERAL: return a->bool_value  == b->bool_value;
        case EXPR_CHAR_LITERAL: return a->char_value  == b->char_value;
        case EXPR_FLOAT_LITERAL:return a->float_value == b->float_value;
        default: return false;
    }
}

// Try to hoist common leading STMT_DECL nodes out of an if/else.
// Returns the list of hoisted stmts (to be prepended before the if),
// and modifies s->body / s->else_body in place to remove them.
static struct stmt* hoist_from_if(struct stmt *s) {
    if (s->kind != STMT_IF_ELSE) return nullptr;
    if (!s->body || !s->else_body) return nullptr;

    struct stmt *hoisted_head = nullptr;
    struct stmt *hoisted_tail = nullptr;

    // Keep hoisting as long as both branches start with the same const decl
    while (true) {
        struct stmt *tb = s->body;
        struct stmt *eb = s->else_body;

        if (!tb || !eb) break;
        if (tb->kind != STMT_DECL || eb->kind != STMT_DECL) break;

        // Same variable name?
        if (strcmp(tb->decl->name, eb->decl->name) != 0) break;

        // Both have constant initialisers with the same value?
        if (!is_literal(tb->decl->value) || !same_const(tb->decl->value, eb->decl->value)) break;

        // Safe to hoist — take from then-branch, drop from else-branch
        struct stmt *hoisted = tb;
        s->body      = tb->next;
        s->else_body = eb->next;
        hoisted->next = nullptr;

        if (!hoisted_head) hoisted_head = hoisted_tail = hoisted;
        else { hoisted_tail->next = hoisted; hoisted_tail = hoisted; }

        opt_hoist_count++;
    }

    return hoisted_head;
}

struct stmt* opt_hoist(struct stmt* s) {
    if (!s) return nullptr;

    // Recurse into sub-bodies first
    if (s->kind == STMT_FUNCTION)
        s->body = opt_hoist(s->body);

    if (s->kind == STMT_IF_ELSE) {
        s->body      = opt_hoist(s->body);
        s->else_body = opt_hoist(s->else_body);

        // Try to hoist common declarations out of this if/else
        struct stmt *hoisted = hoist_from_if(s);
        if (hoisted) {
            // Attach the if statement after the hoisted decls
            struct stmt *tail = hoisted;
            while (tail->next) tail = tail->next;
            tail->next = s;
            // Continue processing siblings from s->next
            s->next = opt_hoist(s->next);
            return hoisted;   // return the new head of this block
        }
    }

    s->next = opt_hoist(s->next);
    return s;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Pass 4 – Dead Code Elimination
// ─────────────────────────────────────────────────────────────────────────────

static bool always_returns(struct stmt *s) {
    if (!s) return false;
    if (s->kind == STMT_RETURN) return true;
    if (s->kind == STMT_IF_ELSE)
        return always_returns(s->body) && always_returns(s->else_body);
    return false;
}

struct stmt* opt_dead_code_elim(struct stmt* s) {
    if (!s) return nullptr;

    if (s->kind == STMT_IF_ELSE && s->init_expr) {
        struct expr *cond = s->init_expr;
        if (is_int_const(cond) || is_bool_const(cond)) {
            int cval = is_bool_const(cond) ? (cond->bool_value ? 1 : 0)
                                           : cond->int_value;
            if (cval) {
                opt_dce_count++;
                struct stmt *kept = opt_dead_code_elim(s->body);
                struct stmt *tail = kept;
                if (tail) { while (tail->next) tail = tail->next; tail->next = opt_dead_code_elim(s->next); }
                else       kept = opt_dead_code_elim(s->next);
                return kept;
            } else {
                opt_dce_count++;
                struct stmt *kept = opt_dead_code_elim(s->else_body);
                struct stmt *tail = kept;
                if (tail) { while (tail->next) tail = tail->next; tail->next = opt_dead_code_elim(s->next); }
                else       kept = opt_dead_code_elim(s->next);
                return kept;
            }
        }
    }

    if (s->kind == STMT_FUNCTION)
        s->body = opt_dead_code_elim(s->body);

    if (s->kind == STMT_IF_ELSE) {
        s->body      = opt_dead_code_elim(s->body);
        s->else_body = opt_dead_code_elim(s->else_body);
    }

    if (always_returns(s) && s->next) {
        struct stmt *dead = s->next;
        while (dead) { opt_dce_count++; dead = dead->next; }
        s->next = nullptr;
    } else {
        s->next = opt_dead_code_elim(s->next);
    }

    return s;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Top-level optimizer
// ─────────────────────────────────────────────────────────────────────────────

static struct expr* opt_expr(struct expr* e) {
    if (!e) return nullptr;
    e->left  = opt_expr(e->left);
    e->right = opt_expr(e->right);
    e = opt_constant_fold(e);
    e = opt_strength_reduce(e);
    return e;
}

static struct stmt* opt_stmt(struct stmt* s) {
    if (!s) return nullptr;

    switch (s->kind) {
        case STMT_DECL:
            if (s->decl->value)
                s->decl->value = opt_expr(s->decl->value);
            break;
        case STMT_EXPR:
            s->expr = opt_expr(s->expr);
            break;
        case STMT_RETURN:
            s->expr = opt_expr(s->expr);
            break;
        case STMT_IF_ELSE:
            s->init_expr = opt_expr(s->init_expr);
            s->body      = opt_stmt(s->body);
            s->else_body = opt_stmt(s->else_body);
            s->next      = opt_stmt(s->next);
            return s;
        case STMT_FUNCTION:
            s->body = opt_stmt(s->body);
            break;
    }

    s->next = opt_stmt(s->next);
    return s;
}

struct stmt* optimize(struct stmt* root) {
    // Pass 1 + 2: Constant Folding and Strength Reduction (expression walk)
    root = opt_stmt(root);

    // Pass 3: Code Hoisting (lift common decls out of if/else branches)
    root = opt_hoist(root);

    // Pass 4: Dead Code Elimination (after folding resolves const conditions)
    root = opt_dead_code_elim(root);

    return root;
}