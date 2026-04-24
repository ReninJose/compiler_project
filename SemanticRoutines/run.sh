bison --defines=token.h --output=parser.c parser.bison -v
flex --outfile=scanner.c scanner.l
g++ main.cpp parser.c scanner.c ast.cpp symbol_table.cpp -o compiler