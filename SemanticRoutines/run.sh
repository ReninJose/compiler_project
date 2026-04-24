bison --defines=token.h --output=parser.c parser.bison -v
flex --outfile=scanner.c scanner.l
g++ main.cpp parser.c scanner.c ast.cpp symbol_table.cpp -o compiler

./compiler test/test1.cpp
./compiler test/test2.cpp
./compiler test/test3.cpp
./compiler test/test4.cpp
./compiler test/test5.cpp