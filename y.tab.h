/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    STRING = 258,
    VAR = 259,
    IGUAL = 260,
    EOL = 261,
    ASPA = 262,
    VEZES = 263,
    DIVIDIDO = 264,
    ABRE_COLCHETE = 265,
    FECHA_COLCHETE = 266,
    EXIT = 267,
    VEZES_T = 268,
    DIVIDIDO_T = 269,
    VEZES_P = 270,
    DIVIDIDO_P = 271,
    VEZES_L = 272,
    DIVIDIDO_L = 273,
    VEZES_C = 274,
    DIVIDIDO_C = 275,
    NUMERO = 276,
    SOMA = 277
  };
#endif
/* Tokens.  */
#define STRING 258
#define VAR 259
#define IGUAL 260
#define EOL 261
#define ASPA 262
#define VEZES 263
#define DIVIDIDO 264
#define ABRE_COLCHETE 265
#define FECHA_COLCHETE 266
#define EXIT 267
#define VEZES_T 268
#define DIVIDIDO_T 269
#define VEZES_P 270
#define DIVIDIDO_P 271
#define VEZES_L 272
#define DIVIDIDO_L 273
#define VEZES_C 274
#define DIVIDIDO_C 275
#define NUMERO 276
#define SOMA 277

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 14 "./src/imageprocessing.y" /* yacc.c:1909  */

	char  strval[50];
	int	  ival;
	float float_value;

#line 104 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
