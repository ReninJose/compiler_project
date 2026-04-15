bison --defines=token.h --output=parser.c parser.bison -v
flex --outfile=scanner.c scanner.l  
g++ main.cpp parser.c scanner.c expr.cpp -o compiler 