#include <stdio.h>
#include "ast.cpp"

extern struct decl * parser_result;
extern int yyparse();
extern void decl_print(struct decl *e );
extern FILE *yyin;

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("File name is missing!\n");
		return -1;
	}
		
	yyin = fopen(argv[1], "r"); // Set Flex's input stream to your file
	if(!yyin) {
		printf("Could not open the program!\n");
		return -1;
	}
	
	if(yyparse()==0) { 		// Start parsing
	  printf("Parse successful!\n AST: \n ------- \n");
	  decl_print(parser_result);
	  printf("\n");
	} else {
	  printf("Parse failed.\n");
	}
}