#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>
#include "ast.h"
#include "ir.h"
#include "optimizer.h"

using namespace std;

static FILE *g_asm;                   // output .s file
static map<string,int> var_offset;    // variable name → offset from fp
static int next_reg   = 0;            // next available scratch register index
static int label_cnt  = 0;            // label counter for branches
static int cur_off    = -4;           // next stack slot (grows downward from fp)

// Simple register allocator: r4..r10 are callee-saved; r0..r3 for small temps.
// We use a linear counter and reset per-statement so depth stays low.
static int  alloc_reg()           { return next_reg++; }
static void free_last_reg()       { if (next_reg > 0) next_reg--; }
static string new_label()         { return ".L" + to_string(label_cnt++); }


static int codegen_expr(struct expr *e)
{
    if (!e) return 0;

    int r, rl, rr;
    switch (e->kind) {

        case EXPR_INT_LITERAL:
            r = alloc_reg();
            fprintf(g_asm, "\tmov r%d, #%d\n", r, e->int_value);
            return r;

        case EXPR_CHAR_LITERAL:
            r = alloc_reg();
            fprintf(g_asm, "\tmov r%d, #%d\n", r, (int)e->char_value);
            return r;

        case EXPR_BOOL_LITERAL:
            r = alloc_reg();
            fprintf(g_asm, "\tmov r%d, #%d\n", r, e->bool_value ? 1 : 0);
            return r;

        case EXPR_NAME:
            r = alloc_reg();
            if (var_offset.count(e->name)) {
                fprintf(g_asm, "\tldr r%d, [fp, #%d]   @ %s\n",
                        r, var_offset[e->name], e->name);
            } else {
                fprintf(g_asm, "\t@ WARNING: '%s' not found in frame\n", e->name);
            }
            return r;

        case EXPR_ASSIGN:
            rr = codegen_expr(e->right);
            if (e->left && e->left->kind == EXPR_NAME &&
                var_offset.count(e->left->name)) {
                fprintf(g_asm, "\tstr r%d, [fp, #%d]   @ %s =\n",
                        rr, var_offset[e->left->name], e->left->name);
            }
            return rr;

        case EXPR_ADD:
            rl = codegen_expr(e->left);
            rr = codegen_expr(e->right);
            fprintf(g_asm, "\tadd r%d, r%d, r%d\n", rl, rl, rr);
            free_last_reg();
            return rl;

        case EXPR_SUB:
            rl = codegen_expr(e->left);
            rr = codegen_expr(e->right);
            fprintf(g_asm, "\tsub r%d, r%d, r%d\n", rl, rl, rr);
            free_last_reg();
            return rl;

        case EXPR_MUL:
            rl = codegen_expr(e->left);
            rr = codegen_expr(e->right);
            fprintf(g_asm, "\tmul r%d, r%d, r%d\n", rl, rr, rl);
            free_last_reg();
            return rl;

        case EXPR_DIV:
            rl = codegen_expr(e->left);
            rr = codegen_expr(e->right);
            fprintf(g_asm, "\tsdiv r%d, r%d, r%d\n", rl, rl, rr);
            free_last_reg();
            return rl;

        case EXPR_LT:
            rl = codegen_expr(e->left); rr = codegen_expr(e->right);
            fprintf(g_asm, "\tcmp r%d, r%d\n",   rl, rr);
            fprintf(g_asm, "\tmovlt r%d, #1\n",  rl);
            fprintf(g_asm, "\tmovge r%d, #0\n",  rl);
            free_last_reg(); return rl;

        case EXPR_GT:
            rl = codegen_expr(e->left); rr = codegen_expr(e->right);
            fprintf(g_asm, "\tcmp r%d, r%d\n",   rl, rr);
            fprintf(g_asm, "\tmovgt r%d, #1\n",  rl);
            fprintf(g_asm, "\tmovle r%d, #0\n",  rl);
            free_last_reg(); return rl;

        case EXPR_LTE:
            rl = codegen_expr(e->left); rr = codegen_expr(e->right);
            fprintf(g_asm, "\tcmp r%d, r%d\n",   rl, rr);
            fprintf(g_asm, "\tmovle r%d, #1\n",  rl);
            fprintf(g_asm, "\tmovgt r%d, #0\n",  rl);
            free_last_reg(); return rl;

        case EXPR_GTE:
            rl = codegen_expr(e->left); rr = codegen_expr(e->right);
            fprintf(g_asm, "\tcmp r%d, r%d\n",   rl, rr);
            fprintf(g_asm, "\tmovge r%d, #1\n",  rl);
            fprintf(g_asm, "\tmovlt r%d, #0\n",  rl);
            free_last_reg(); return rl;

        case EXPR_EQEQ:
            rl = codegen_expr(e->left); rr = codegen_expr(e->right);
            fprintf(g_asm, "\tcmp r%d, r%d\n",   rl, rr);
            fprintf(g_asm, "\tmoveq r%d, #1\n",  rl);
            fprintf(g_asm, "\tmovne r%d, #0\n",  rl);
            free_last_reg(); return rl;

        case EXPR_NEQ:
            rl = codegen_expr(e->left); rr = codegen_expr(e->right);
            fprintf(g_asm, "\tcmp r%d, r%d\n",   rl, rr);
            fprintf(g_asm, "\tmovne r%d, #1\n",  rl);
            fprintf(g_asm, "\tmoveq r%d, #0\n",  rl);
            free_last_reg(); return rl;

        case EXPR_AND:
            rl = codegen_expr(e->left); rr = codegen_expr(e->right);
            fprintf(g_asm, "\tand r%d, r%d, r%d\n", rl, rl, rr);
            free_last_reg(); return rl;

        case EXPR_OR:
            rl = codegen_expr(e->left); rr = codegen_expr(e->right);
            fprintf(g_asm, "\torr r%d, r%d, r%d\n", rl, rl, rr);
            free_last_reg(); return rl;

        case EXPR_NOT:
            rr = codegen_expr(e->right);
            fprintf(g_asm, "\tcmp r%d, #0\n",   rr);
            fprintf(g_asm, "\tmoveq r%d, #1\n", rr);
            fprintf(g_asm, "\tmovne r%d, #0\n", rr);
            return rr;

        default:
            r = alloc_reg();
            fprintf(g_asm, "\t@ unsupported expr kind %d\n", e->kind);
            return r;
    }
}

static int count_locals(struct stmt *s)
{
    int n = 0;
    for (; s; s = s->next) {
        if (s->kind == STMT_DECL)     n++;
        if (s->kind == STMT_IF_ELSE) {
            n += count_locals(s->body);
            n += count_locals(s->else_body);
        }
    }
    return n;
}

// NOTE: AI GENERATED CODE STARTS FROM HERE

// Forward declaration
static void codegen_stmt(struct stmt *s, const string &ret_lbl);

// ─────────────────────────────────────────────────────────────────────────────
//  Function code generation (prologue → body → epilogue)
// ─────────────────────────────────────────────────────────────────────────────
static void codegen_func(struct stmt *s)
{
    const string fname = s->decl->name;
    struct type  *ft   = s->decl->type;   // DECL_FUNCTION

    // Reset per-function state
    var_offset.clear();
    next_reg  = 0;
    label_cnt = 0;
    cur_off   = -4;   // first slot below fp

    // Count params and locals to size the frame
    int n_params = 0;
    for (struct param_list *p = ft->params; p; p = p->next) n_params++;
    int n_locals = count_locals(s->body);
    // Allocate space for both; align to 8 bytes
    int alloc_sz = ((n_params + n_locals) * 4 + 7) & ~7;
    if (alloc_sz < 8) alloc_sz = 8;

    // Assign stack offsets for parameters (saved from r0..r3)
    int pi = 0;
    for (struct param_list *p = ft->params; p; p = p->next, pi++) {
        var_offset[p->name] = cur_off;
        cur_off -= 4;
    }

    // ── Prologue ──────────────────────────────────────────────────────────────
    fprintf(g_asm, "\n.global %s\n%s:\n", fname.c_str(), fname.c_str());
    fprintf(g_asm, "\tpush {fp, lr}\n");
    fprintf(g_asm, "\tmov fp, sp\n");
    fprintf(g_asm, "\tsub sp, sp, #%d\n", alloc_sz);

    // Save incoming parameter registers to their stack slots
    pi = 0;
    for (struct param_list *p = ft->params; p && pi < 4; p = p->next, pi++) {
        fprintf(g_asm, "\tstr r%d, [fp, #%d]   @ param %s\n",
                pi, var_offset[p->name], p->name);
    }

    // ── Body ──────────────────────────────────────────────────────────────────
    const string ret_lbl = ".Lret_" + fname;
    codegen_stmt(s->body, ret_lbl);

    // ── Epilogue ──────────────────────────────────────────────────────────────
    fprintf(g_asm, "%s:\n", ret_lbl.c_str());
    fprintf(g_asm, "\tmov sp, fp\n");
    fprintf(g_asm, "\tpop {fp, pc}\n");
}

// ─────────────────────────────────────────────────────────────────────────────
//  Statement code generation
// ─────────────────────────────────────────────────────────────────────────────
static void codegen_stmt(struct stmt *s, const string &ret_lbl)
{
    if (!s) return;

    switch (s->kind) {

        case STMT_DECL: {
            // Assign a new stack slot for this local variable
            if (!var_offset.count(s->decl->name)) {
                var_offset[s->decl->name] = cur_off;
                cur_off -= 4;
            }
            // Evaluate initialiser and store
            if (s->decl->value) {
                next_reg = 0;
                fprintf(g_asm, "\t@ decl %s\n", s->decl->name);
                int r = codegen_expr(s->decl->value);
                fprintf(g_asm, "\tstr r%d, [fp, #%d]   @ store %s\n",
                        r, var_offset[s->decl->name], s->decl->name);
            }
            break;
        }

        case STMT_EXPR:
            next_reg = 0;
            fprintf(g_asm, "\t@ expr stmt\n");
            codegen_expr(s->expr);
            break;

        case STMT_RETURN:
            next_reg = 0;
            fprintf(g_asm, "\t@ return\n");
            if (s->expr) {
                int r = codegen_expr(s->expr);
                if (r != 0)
                    fprintf(g_asm, "\tmov r0, r%d\n", r);
            }
            fprintf(g_asm, "\tb %s\n", ret_lbl.c_str());
            break;

        case STMT_IF_ELSE: {
            string lbl_else = new_label();
            string lbl_end  = new_label();
            next_reg = 0;
            fprintf(g_asm, "\t@ if\n");
            int r = codegen_expr(s->init_expr);
            fprintf(g_asm, "\tcmp r%d, #0\n",       r);
            fprintf(g_asm, "\tbeq %s\n",  lbl_else.c_str());
            codegen_stmt(s->body,      ret_lbl);
            fprintf(g_asm, "\tb %s\n",    lbl_end.c_str());
            fprintf(g_asm, "%s:\n",       lbl_else.c_str());
            if (s->else_body)
                codegen_stmt(s->else_body, ret_lbl);
            fprintf(g_asm, "%s:\n",       lbl_end.c_str());
            break;
        }

        case STMT_FUNCTION:
            codegen_func(s);
            break;
    }

    codegen_stmt(s->next, ret_lbl);
}

// AI generation ends here

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input.ir> <output.s>\n", argv[0]);
        return 1;
    }

    FILE *ir_file = fopen(argv[1], "r");
    if (!ir_file) { perror(argv[1]); return 1; }

    g_asm = fopen(argv[2], "w");
    if (!g_asm) { perror(argv[2]); return 1; }

    // Step 1: Read IR and reconstruct AST 
    printf("=== Reading IR: %s ===\n", argv[1]);
    struct stmt *root = ir_read(ir_file);
    fclose(ir_file);

    // Step 2: Print reconstructed AST (verification)
    printf("\n=== Reconstructed AST ===\n");
    ast_print_tree(root);

    // Step 2b: Optimize AST 
    printf("\n=== Running Optimizer ===\n");
    root = optimize(root);
    opt_print_stats();

    printf("=== Optimized AST ===\n");
    ast_print_tree(root);

    // Step 3: Emit ARM32 assembly
    printf("\n=== Generating ARM32 assembly: %s ===\n", argv[2]);
    fprintf(g_asm, "@ ARM32 Assembly  (generated by code_gen)\n");
    fprintf(g_asm, ".arch armv7-a\n");
    fprintf(g_asm, ".text\n");

    // Top-level statements that are not functions become a synthetic __init
    // (simple global declarations; full functions are handled in codegen_func)
    codegen_stmt(root, ".Lret_toplevel");

    fclose(g_asm);
    printf("Done.\n");
    return 0;
}
