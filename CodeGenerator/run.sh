bison --defines=token.h --output=parser.c parser.bison -v
flex --outfile=scanner.c scanner.l
g++ main.cpp parser.c scanner.c ast.cpp symbol_table.cpp ir.cpp -o compiler
g++ codegen.cpp ir.cpp ast.cpp -o code_gen

./compiler test/test1.cpp test.ir       
./code_gen test.ir output/test1.s        

./compiler test/test2.cpp test.ir       
./code_gen test.ir output/test2.s         

./compiler test/test3.cpp test.ir       
./code_gen test.ir output/test3.s         

./compiler test/test4.cpp test.ir       
./code_gen test.ir output/test4.s        

./compiler test/test5.cpp test.ir      
./code_gen test.ir output/test5.s        