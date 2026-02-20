# compiler_project

## Scanner - version1
- Instructions to run the scanner:
1. mkdir Scanner/bin
2. flex -o Scanner/lex_out.yy.cc Scanner/lexer.l
3. clang++ Scanner/main.cpp Scanner/lex_out.yy.cc -o Scanner/bin/linker
4. ./Scanner/bin/linker