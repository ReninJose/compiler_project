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

## Semantic Routines
- Instruction to run semantic routines code:
1. cd into the semanticRoutines folder
2. Run the script "./run.sh". It should compile and run all the test cases together.

To run test cases independently: ./compiler test/test<1-5>.cpp
Example: ./compiler test/test3.cpp

## Code Generator
- Instruction to run the code generator
1. cd into the CodeGenerator folder
2. Run the script "./run.sh". The code will read each file one by one and produces a arm file inside the output folder

## Code Optimizer
- I have included the following optimization techniques:
- 1. Constant Folding 
- 2. Strength Reduction
- 3. Dead Code Elimination
- 4. Code Hoisting

- Instruction to run the code optimizer
1. cd into the Optimization folder
2. Run the script "./run.sh". The code will generate the assembly files inside the output folder

- Note: I've used AI to help me print the AST tree on the terminal and to generate code optimizer's test cases. I've mentioned in code as comments to point out AI code.  