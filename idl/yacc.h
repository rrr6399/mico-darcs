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

#ifndef YY_YY_YACC_H_INCLUDED
# define YY_YY_YACC_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_AMPERSAND = 258,
    T_ANY = 259,
    T_ASTERISK = 260,
    T_ATTRIBUTE = 261,
    T_BOOLEAN = 262,
    T_CASE = 263,
    T_CHAR = 264,
    T_CHARACTER_LITERAL = 265,
    T_WCHARACTER_LITERAL = 266,
    T_CIRCUMFLEX = 267,
    T_COLON = 268,
    T_COMMA = 269,
    T_CONST = 270,
    T_CONTEXT = 271,
    T_DEFAULT = 272,
    T_DOUBLE = 273,
    T_ENUM = 274,
    T_EQUAL = 275,
    T_EXCEPTION = 276,
    T_FALSE = 277,
    T_FIXED = 278,
    T_FIXED_PT_LITERAL = 279,
    T_FLOAT = 280,
    T_FLOATING_PT_LITERAL = 281,
    T_GREATER_THAN_SIGN = 282,
    T_IDENTIFIER = 283,
    T_IN = 284,
    T_INOUT = 285,
    T_INTEGER_LITERAL = 286,
    T_INTERFACE = 287,
    T_LEFT_CURLY_BRACKET = 288,
    T_LEFT_PARANTHESIS = 289,
    T_LEFT_SQUARE_BRACKET = 290,
    T_LESS_THAN_SIGN = 291,
    T_LONG = 292,
    T_MINUS_SIGN = 293,
    T_MODULE = 294,
    T_OCTET = 295,
    T_ONEWAY = 296,
    T_OUT = 297,
    T_PERCENT_SIGN = 298,
    T_PLUS_SIGN = 299,
    T_RAISES = 300,
    T_READONLY = 301,
    T_RIGHT_CURLY_BRACKET = 302,
    T_RIGHT_PARANTHESIS = 303,
    T_RIGHT_SQUARE_BRACKET = 304,
    T_SCOPE = 305,
    T_SEMICOLON = 306,
    T_SEQUENCE = 307,
    T_SHIFTLEFT = 308,
    T_SHIFTRIGHT = 309,
    T_SHORT = 310,
    T_SOLIDUS = 311,
    T_STRING = 312,
    T_STRING_LITERAL = 313,
    T_WSTRING_LITERAL = 314,
    T_PRAGMA = 315,
    T_INCLUDE = 316,
    T_STRUCT = 317,
    T_SWITCH = 318,
    T_TILDE = 319,
    T_TRUE = 320,
    T_OBJECT = 321,
    T_TYPEDEF = 322,
    T_UNION = 323,
    T_UNSIGNED = 324,
    T_VERTICAL_LINE = 325,
    T_VOID = 326,
    T_WCHAR = 327,
    T_WSTRING = 328,
    T_UNKNOWN = 329,
    T_ABSTRACT = 330,
    T_VALUETYPE = 331,
    T_TRUNCATABLE = 332,
    T_SUPPORTS = 333,
    T_CUSTOM = 334,
    T_PUBLIC = 335,
    T_PRIVATE = 336,
    T_FACTORY = 337,
    T_NATIVE = 338,
    T_VALUEBASE = 339,
    T_IMPORT = 340,
    T_TYPEID = 341,
    T_TYPEPREFIX = 342,
    T_GETRAISES = 343,
    T_SETRAISES = 344,
    T_LOCAL = 345,
    T_COMPONENT = 346,
    T_PROVIDES = 347,
    T_USES = 348,
    T_MULTIPLE = 349,
    T_EMITS = 350,
    T_PUBLISHES = 351,
    T_CONSUMES = 352,
    T_HOME = 353,
    T_MANAGES = 354,
    T_PRIMARYKEY = 355,
    T_FINDER = 356,
    T_EVENTTYPE = 357
  };
#endif
/* Tokens.  */
#define T_AMPERSAND 258
#define T_ANY 259
#define T_ASTERISK 260
#define T_ATTRIBUTE 261
#define T_BOOLEAN 262
#define T_CASE 263
#define T_CHAR 264
#define T_CHARACTER_LITERAL 265
#define T_WCHARACTER_LITERAL 266
#define T_CIRCUMFLEX 267
#define T_COLON 268
#define T_COMMA 269
#define T_CONST 270
#define T_CONTEXT 271
#define T_DEFAULT 272
#define T_DOUBLE 273
#define T_ENUM 274
#define T_EQUAL 275
#define T_EXCEPTION 276
#define T_FALSE 277
#define T_FIXED 278
#define T_FIXED_PT_LITERAL 279
#define T_FLOAT 280
#define T_FLOATING_PT_LITERAL 281
#define T_GREATER_THAN_SIGN 282
#define T_IDENTIFIER 283
#define T_IN 284
#define T_INOUT 285
#define T_INTEGER_LITERAL 286
#define T_INTERFACE 287
#define T_LEFT_CURLY_BRACKET 288
#define T_LEFT_PARANTHESIS 289
#define T_LEFT_SQUARE_BRACKET 290
#define T_LESS_THAN_SIGN 291
#define T_LONG 292
#define T_MINUS_SIGN 293
#define T_MODULE 294
#define T_OCTET 295
#define T_ONEWAY 296
#define T_OUT 297
#define T_PERCENT_SIGN 298
#define T_PLUS_SIGN 299
#define T_RAISES 300
#define T_READONLY 301
#define T_RIGHT_CURLY_BRACKET 302
#define T_RIGHT_PARANTHESIS 303
#define T_RIGHT_SQUARE_BRACKET 304
#define T_SCOPE 305
#define T_SEMICOLON 306
#define T_SEQUENCE 307
#define T_SHIFTLEFT 308
#define T_SHIFTRIGHT 309
#define T_SHORT 310
#define T_SOLIDUS 311
#define T_STRING 312
#define T_STRING_LITERAL 313
#define T_WSTRING_LITERAL 314
#define T_PRAGMA 315
#define T_INCLUDE 316
#define T_STRUCT 317
#define T_SWITCH 318
#define T_TILDE 319
#define T_TRUE 320
#define T_OBJECT 321
#define T_TYPEDEF 322
#define T_UNION 323
#define T_UNSIGNED 324
#define T_VERTICAL_LINE 325
#define T_VOID 326
#define T_WCHAR 327
#define T_WSTRING 328
#define T_UNKNOWN 329
#define T_ABSTRACT 330
#define T_VALUETYPE 331
#define T_TRUNCATABLE 332
#define T_SUPPORTS 333
#define T_CUSTOM 334
#define T_PUBLIC 335
#define T_PRIVATE 336
#define T_FACTORY 337
#define T_NATIVE 338
#define T_VALUEBASE 339
#define T_IMPORT 340
#define T_TYPEID 341
#define T_TYPEPREFIX 342
#define T_GETRAISES 343
#define T_SETRAISES 344
#define T_LOCAL 345
#define T_COMPONENT 346
#define T_PROVIDES 347
#define T_USES 348
#define T_MULTIPLE 349
#define T_EMITS 350
#define T_PUBLISHES 351
#define T_CONSUMES 352
#define T_HOME 353
#define T_MANAGES 354
#define T_PRIMARYKEY 355
#define T_FINDER 356
#define T_EVENTTYPE 357

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 56 "yacc.yy" /* yacc.c:1909  */

  char              *ident;
  ParseNode         *node;
  CORBA::ULongLong  _int;
  char             *_str;
  CORBA::WChar     *_wstr;
  CORBA::Char       _char;
  CORBA::WChar      _wchar;
  CORBA::LongDouble _float;
  CORBA::LongDouble _fixed;

#line 270 "yacc.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_YACC_H_INCLUDED  */
