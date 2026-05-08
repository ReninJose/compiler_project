#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ast.h"

struct stmt* optimize(struct stmt* root);
struct expr* opt_constant_fold(struct expr* e);
struct expr* opt_strength_reduce(struct expr* e);
struct stmt* opt_hoist(struct stmt* s);
struct stmt* opt_dead_code_elim(struct stmt* s);

extern int opt_cf_count;    // constant folds
extern int opt_sr_count;    // strength reductions
extern int opt_hoist_count; // hoisted declarations
extern int opt_dce_count;   // dead code eliminations

void opt_print_stats();

#endif