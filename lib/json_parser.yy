/* This file is part of QJSon
  *
  * Copyright (C) 2008 Flavio Castelli <flavio.castelli@gmail.com>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public License
  * along with this library; see the file COPYING.LIB.  If not, write to
  * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  * Boston, MA 02110-1301, USA.
  */

%skeleton "lalr1.cc"
%defines
%define "parser_class_name" "json_parser"

%{
  #include <QDebug>
  #include <QMap>
  #include <QString>
  #include <QVariant>

  #include "json_driver.hh"
  #include "json_scanner.h"

  class JSonDriver;
  class JSonScanner;


  #define YYERROR_VERBOSE 1
%}

%parse-param { JSonDriver* driver }
%lex-param { JSonDriver* driver }

%locations

%debug
%error-verbose

%token END 0 "end of file"

%token CURLY_BRACKET_OPEN 1 "{"
%token CURLY_BRACKET_CLOSE 2 "}"
%token SQUARE_BRACKET_OPEN 3 "["
%token SQUARE_BRACKET_CLOSE 4 "]"

%token COLON 5 ":"
%token COMMA 6 ","
%token MINUS 7 "-"
%token DOT 8 "."
%token DIGIT 9 "digit"
%token E 10 "exponential"
%token TRUE_VAL 11 "true"
%token FALSE_VAL 12 "false"
%token NULL_VAL 13 "null"
%token QUOTMARKOPEN 14 "open quotation mark"
%token QUOTMARKCLOSE 15 "close quotation mark"

%token WORD 16 "character"

// define the initial token
%start start

%%

// grammar rules

start: data {
              driver->m_result = $1;
              qDebug() << "json_parser - parsing finished";
            };

data: object {$$ = $1; }
      | array {$$ = $1; }
      | error
          {
            qDebug()<< "json_parser - syntax error found, "
                    << "forcing exit";
            YYABORT;
          }
      | END;

object: CURLY_BRACKET_OPEN members CURLY_BRACKET_CLOSE { $$ = $2; };

members: /* empty */ { $$ = QVariant (QVariantMap()); }
        | pair r_members {
            QVariantMap members = $2.toMap();
            $$ = QVariant(members.unite ($1.toMap()));
          };

r_members: /* empty */ { $$ = QVariant (QVariantMap()); }
        | COMMA pair r_members {
          QVariantMap members = $3.toMap();
          
          $$ = QVariant(members.unite ($2.toMap()));
          };

pair:   string COLON value {
            QVariantMap pair;
            pair.insert ($1.toString(), QVariant($3));
            $$ = QVariant (pair);
          };

array: SQUARE_BRACKET_OPEN values SQUARE_BRACKET_CLOSE { $$ = $2; };

values: /* empty */ { $$ = QVariant (QVariantList()); }
        | value r_values {
          QVariantList members = $2.toList();
          members.prepend ($1);
          $$ = QVariant(members);
        };

r_values: /* empty */ { $$ = QVariant (QVariantList()); }
          | COMMA value r_values {
            QVariantList members = $3.toList();
            members.prepend ($2);
            $$ = QVariant(members);
          };

value: string { $$ = $1; }
        | number { $$ = $1; }
        | object { $$ = $1; }
        | array { $$ = $1; }
        | TRUE_VAL { $$ = QVariant (true); }
        | FALSE_VAL { $$ = QVariant (false); }
        | NULL_VAL {
          QVariant null_variant;
          $$ = null_variant;
        };

number: int { $$ = QVariant ($1); }
        | int fract {  $$ = QVariant ($1.toString().append($2.toString())); }
        | int exp {  $$ = QVariant ($1.toString().append($2.toString())); }
        | int fract exp {
          QString value = $1.toString();
          value += $2.toString();
          value += $3.toString();
          $$ = QVariant (value);
        };

int:  DIGIT digits { $$ = QVariant ($1.toString().append($2.toString())); }
      | MINUS DIGIT digits { $$ = QVariant ($3.toString().prepend($2.toString().prepend("-"))); };

digits: /* empty */ { $$ = QVariant (""); }
        | DIGIT digits {
          QString digits = $2.toString();
          digits.prepend ($1.toString());
          $$ = QVariant(digits);
        };

fract: DOT digits {
          QString digits = $2.toString();
          digits.prepend (".");
          $$ = QVariant(digits);
        };

exp: E digits { $$ = QVariant($1.toString().append($2.toString())); };

string: QUOTMARKOPEN string_arg QUOTMARKCLOSE { $$ = $2 };

string_arg: /*empty */ { $$ = QVariant (""); }
            | string_arg WORD {
                QString string = $1.toString();
                string += $2.toString();
                $$ = QVariant(string);
              };

%%

int yy::yylex(YYSTYPE *yylval, yy::location *yylloc, JSonDriver* driver)
{
  JSonScanner* scanner = driver->scanner();
  yylval->clear();
  int ret = scanner->yylex(yylval, yylloc);

  char buff [50];
  snprintf (buff, 50 * sizeof (char), "%i", ret);

  qDebug() << "json_parser::yylex - calling scanner yylval==|" 
           << yylval->toString() << "|, ret==|" << buff << "|";
  
  return ret;
}

void yy::json_parser::error (const yy::location& yyloc,
                                 const std::string& error)
{
  /*qDebug() << yyloc.begin.line;
  qDebug() << yyloc.begin.column;
  qDebug() << yyloc.end.line;
  qDebug() << yyloc.end.column;*/
  qDebug("json_parser::error [line %i] - %s", yyloc.end.line, error.c_str()) ;
  driver->setError(error.c_str(), yyloc.end.line);  
}
