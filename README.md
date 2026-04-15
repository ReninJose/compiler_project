# compiler_project

## Scanner - version1
- Instructions to run the scanner:
1. mkdir Scanner/bin
2. flex -o Scanner/lex_out.yy.cc Scanner/lexer.l
3. clang++ Scanner/main.cpp Scanner/lex_out.yy.cc -o Scanner/bin/linker
4. ./Scanner/bin/linker

## Parser - version1
- Instruction to run the parser:
1. cd into the Parser folder
2. Run the following command ./run_script.sh
3. Run the code: ./compiler

- note: The parser will read the contents in the file line by line. If the parser outputs nothing, then the parser parsed the line properly but I havn't added a functionality to return a value.