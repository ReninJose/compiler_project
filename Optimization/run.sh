bison --defines=token.h --output=parser.c parser.bison -v
flex --outfile=scanner.c scanner.l
g++ main.cpp parser.c scanner.c ast.cpp symbol_table.cpp ir.cpp -o compiler
g++ code_gen.cpp ir.cpp ast.cpp optimizer.cpp -o code_gen

./compiler test/cf.cpp test_cf.ir
./code_gen test_cf.ir output/test_cf.s

./compiler test/dce.cpp dce.ir
./code_gen dce.ir output/dce.s

./compiler test/hoist.cpp hoist.ir
./code_gen hoist.ir output/hoist.s

./compiler test/sr.cpp sr.ir
./code_gen sr.ir output/sr.s
