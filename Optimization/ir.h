#ifndef IR_H
#define IR_H

#include "ast.h"
#include <stdio.h>

// Serialize the AST to a text IR file
void ir_emit(FILE *out, struct stmt *root);

// Reconstruct the AST from an IR file
struct stmt *ir_read(FILE *in);

#endif