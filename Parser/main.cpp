#include <stdio.h>
#include "expr.h"

extern int yyparse();
extern struct expr *parser_result;

int main() {
    printf("Enter expression: ");
    
    if (yyparse() == 0) {
        printf("Parsed expression: ");
        expr_print(parser_result);
        printf("\n");
    } else {
        printf("Parse failed.\n");
    }

    return 0;
}