#include <stdio.h>
#include <iostream>
#include "ast.h"
#include "symbol_table.h"

extern struct stmt *parser_result;
extern int yyparse();
extern FILE *yyin;

int main(int argc, char **argv)
{
    if (argc < 2) { 
        printf("Usage: %s <file>\n", argv[0]); 
        return -1; 
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) { 
        printf("Could not open '%s'\n", argv[1]); 
        return -1; 
    }

    if (yyparse() != 0) { 
        printf("Parse failed.\n"); 
        return -1; 
    }
    printf("Parse successful!\n");

    ast_print_tree(parser_result);

    scope_enter();            
    stmt_resolve(parser_result);
    scope_exit();

    if (resolve_errors > 0)
        std::cerr << resolve_errors << " resolve error(s) found.\n";
    else
        std::cout << "\nName resolution: OK\n";

    stmt_typecheck(parser_result, nullptr);

    if (type_errors > 0)
        std::cerr << type_errors << " type error(s) found.\n";
    else
        std::cout << "Type checking:   OK\n";

    return (resolve_errors + type_errors) > 0 ? 1 : 0;
}