# compiler_project

## Scanner - version1
- Instructions to run the scanner:
1. flex -o Scanner/lex_out.yy.cc Scanner/lexer.l
2. clang++ Scanner/main.cpp Scanner/lex_out.yy.cc -o Scanner/bin/linker
3. ./Scanner/bin/linker