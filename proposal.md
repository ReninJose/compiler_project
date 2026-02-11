# Compiler Project – Initial Definition

## 1. Student Information
- **Name:** Renin Jose
- **Program:** ECE 506C
- **Course:** Compiler Design
- **Semester / Year:** 2026

## 2. Project Overview
A compiler design project that translates a source language to a target language. This project is written in C++ to convert C code into Arm64.

## 3. Source Language
- **Name of the source code language:** C

## 4. Target / Output Language
- **Output of the compiler:** Arm64 
- **Reason for choosing this output language:** Interested in generating assembly code for smaller hardware to run efficiently. Such as smartphones, laptops, etc.

## 5. Compiler Features (Planned)
- Lexical analysis: RE/flex
- Syntax analysis (parser type, if known): Bison
- Semantic analysis: N/A
- Intermediate representation: LLVM IR
- Optimizations (if any): LLVM Pass Manager
- Code generation: LLVM AArch64 Backend

## 6. Tools and Libraries
List any tools, frameworks, or libraries you plan to use:
- Parser generators (e.g., ANTLR, Bison, Yacc): Bison
- Build tools: CMake
- Dependency Management: vcpkg
- Testing frameworks: LLVM lit
- flex

## 7. Repository Information
- **Repository hosting service:** Github
- **Repository URL:** https://github.com/ReninJose/compiler_project#

## 8. Project Status
- [x] Repository created
- [x] README file added
- [x] Initial project structure committed

## 9. Additional Notes
May change the planned libraries/tools while developing the project. 
