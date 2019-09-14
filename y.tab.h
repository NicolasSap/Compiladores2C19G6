
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
     DIGITO = 258,
     LETRA = 259,
     CHAR_ESP = 260,
     COMMENT = 261,
     ID = 262,
     CTE_STRING = 263,
     CTE_ENT = 264,
     CTE_REAL = 265,
     P_A = 266,
     P_C = 267,
     C_A = 268,
     C_C = 269,
     L_A = 270,
     L_C = 271,
     PYC = 272,
     PTO = 273,
     DPTO = 274,
     COMA = 275,
     OP_MAX = 276,
     OP_MAXEQ = 277,
     OP_MIN = 278,
     OP_MINEQ = 279,
     OP_NEQ = 280,
     OP_EQ = 281,
     EQ = 282,
     IF = 283,
     ELSE = 284,
     ENDIF = 285,
     VAR = 286,
     ENDVAR = 287,
     REPEAT = 288,
     UNTIL = 289,
     PRINT = 290,
     READ = 291,
     CONST = 292,
     INT = 293,
     FLOAT = 294,
     STRING = 295,
     MENOS_UNARIO = 296,
     OP_ASIG = 297,
     OP_SUMA = 298,
     OP_RESTA = 299,
     OP_MULT = 300,
     OP_DIV = 301,
     AND = 302,
     OR = 303,
     NOT = 304
   };
#endif
/* Tokens.  */
#define DIGITO 258
#define LETRA 259
#define CHAR_ESP 260
#define COMMENT 261
#define ID 262
#define CTE_STRING 263
#define CTE_ENT 264
#define CTE_REAL 265
#define P_A 266
#define P_C 267
#define C_A 268
#define C_C 269
#define L_A 270
#define L_C 271
#define PYC 272
#define PTO 273
#define DPTO 274
#define COMA 275
#define OP_MAX 276
#define OP_MAXEQ 277
#define OP_MIN 278
#define OP_MINEQ 279
#define OP_NEQ 280
#define OP_EQ 281
#define EQ 282
#define IF 283
#define ELSE 284
#define ENDIF 285
#define VAR 286
#define ENDVAR 287
#define REPEAT 288
#define UNTIL 289
#define PRINT 290
#define READ 291
#define CONST 292
#define INT 293
#define FLOAT 294
#define STRING 295
#define MENOS_UNARIO 296
#define OP_ASIG 297
#define OP_SUMA 298
#define OP_RESTA 299
#define OP_MULT 300
#define OP_DIV 301
#define AND 302
#define OR 303
#define NOT 304




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


