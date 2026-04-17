/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_INT_TYPE = 258,
     TOKEN_CHAR_TYPE = 259,
     TOKEN_FLOAT_TYPE = 260,
     TOKEN_VOID_TYPE = 261,
     TOKEN_BOOL_TYPE = 262,
     TOKEN_IF = 263,
     TOKEN_ELSE = 264,
     TOKEN_INT = 265,
     TOKEN_CHAR = 266,
     TOKEN_PLUS = 267,
     TOKEN_MINUS = 268,
     TOKEN_MUL = 269,
     TOKEN_DIV = 270,
     TOKEN_BOOL = 271,
     TOKEN_TYPE = 272,
     TOKEN_IDENT = 273,
     TOKEN_ASSIGN = 274,
     TOKEN_SEMI = 275,
     TOKEN_ERROR = 276,
     TOKEN_EQEQ = 277,
     TOKEN_NEQ = 278,
     TOKEN_LTE = 279,
     TOKEN_GTE = 280,
     TOKEN_LT = 281,
     TOKEN_GT = 282,
     TOKEN_AND = 283,
     TOKEN_OR = 284,
     TOKEN_NOT = 285,
     TOKEN_TRUE = 286,
     TOKEN_FALSE = 287,
     TOKEN_LPAREN = 288,
     TOKEN_RPAREN = 289,
     TOKEN_LCURLY = 290,
     TOKEN_RCURLY = 291,
     TOKEN_RETURN = 292,
     TOKEN_LIB = 293,
     stmt = 294
   };
#endif
/* Tokens.  */
#define TOKEN_INT_TYPE 258
#define TOKEN_CHAR_TYPE 259
#define TOKEN_FLOAT_TYPE 260
#define TOKEN_VOID_TYPE 261
#define TOKEN_BOOL_TYPE 262
#define TOKEN_IF 263
#define TOKEN_ELSE 264
#define TOKEN_INT 265
#define TOKEN_CHAR 266
#define TOKEN_PLUS 267
#define TOKEN_MINUS 268
#define TOKEN_MUL 269
#define TOKEN_DIV 270
#define TOKEN_BOOL 271
#define TOKEN_TYPE 272
#define TOKEN_IDENT 273
#define TOKEN_ASSIGN 274
#define TOKEN_SEMI 275
#define TOKEN_ERROR 276
#define TOKEN_EQEQ 277
#define TOKEN_NEQ 278
#define TOKEN_LTE 279
#define TOKEN_GTE 280
#define TOKEN_LT 281
#define TOKEN_GT 282
#define TOKEN_AND 283
#define TOKEN_OR 284
#define TOKEN_NOT 285
#define TOKEN_TRUE 286
#define TOKEN_FALSE 287
#define TOKEN_LPAREN 288
#define TOKEN_RPAREN 289
#define TOKEN_LCURLY 290
#define TOKEN_RCURLY 291
#define TOKEN_RETURN 292
#define TOKEN_LIB 293
#define stmt 294




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 16 "parser.bison"
{
    int val;
    struct stmt* stmt;
    struct type* type;
    struct decl* decl; 
    struct expr* expr;
}
/* Line 1529 of yacc.c.  */
#line 135 "token.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

