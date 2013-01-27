/* This file is part of QJson
 *
 * Copyright (C) 2013 Silvio Moioli <silvio@moioli.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software Foundation.
 * 
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

%option 8bit c++ full warn
%option noyywrap nounistd
%option noinput nounput noyy_push_state noyy_pop_state noyy_top_state noyy_scan_buffer noyy_scan_bytes noyy_scan_string noyyget_extra noyyset_extra noyyget_leng noyyget_text noyyget_lineno noyyset_lineno noyyget_in noyyset_in noyyget_out noyyset_out noyyget_lval noyyset_lval noyyget_lloc noyyset_lloc noyyget_debug noyyset_debug
%option yyclass="JSonScanner"
%option outfile="json_scanner.cc"

%{
  #include "json_scanner.h"
  #include "json_parser.hh"
  
  #define YY_USER_INIT if(m_allowSpecialNumbers) { \
    BEGIN(ALLOW_SPECIAL_NUMBERS); \
  }
%}

%s ALLOW_SPECIAL_NUMBERS
%x QUOTMARK_OPEN HEX_OPEN

%%

[\v\f\t ]+    {
                m_yylloc->columns(yyleng);
              }

[\r\n]+       {
                m_yylloc->lines(yyleng);
              }

true          {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::TRUE_VAL;
              }
                
false         {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::FALSE_VAL;
              }

null          {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::NULL_VAL;
              }

[Ee][+\-]?    {
                m_yylloc->columns(yyleng);
                *m_yylval = QVariant(QLatin1String(yytext));
                return yy::json_parser::token::E;
              }

[1-9]+0?      |
[1-9]*0       {
                m_yylloc->columns(yyleng);
                *m_yylval = QVariant(QString::fromAscii(yytext).toULongLong());
                return yy::json_parser::token::DIGIT;
              }

[A-Za-z]      {
                m_yylloc->columns(yyleng);
                *m_yylval = QVariant(QLatin1String(yytext));
                return yy::json_parser::token::STRING;
              }
              
:             {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::COLON;
              }

,             {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::COMMA;
              }

\.            {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::DOT;
              }

-             {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::MINUS;
              }

\[            {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::SQUARE_BRACKET_OPEN;
              }

\]            {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::SQUARE_BRACKET_CLOSE;
              }

\{            {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::CURLY_BRACKET_OPEN;
              }

\}            {
                m_yylloc->columns(yyleng);
                return yy::json_parser::token::CURLY_BRACKET_CLOSE;
              }

              
\"            {
                m_yylloc->columns(yyleng);
                BEGIN(QUOTMARK_OPEN);
                return yy::json_parser::token::QUOTMARKOPEN;
              }
              
<QUOTMARK_OPEN>{
  \\\"          {
                  m_stringPieces << QLatin1String("\"");
                }
  \\\\          {
                  m_stringPieces << QLatin1String("\\");
                }
  \\\/          {
                  m_stringPieces << QLatin1String("/");
                }
  \\b           {
                   m_stringPieces << QLatin1String("\b");
                }
  \\f           {
                  m_stringPieces << QLatin1String("\f");
                }
  \\n           {
                  m_stringPieces << QLatin1String("\n");
                }
  \\r           {
                  m_stringPieces << QLatin1String("\r");
                }
  \\t           {
                  m_stringPieces << QLatin1String("\t");
                }
  \\u           {
                  BEGIN(HEX_OPEN);
                }
  [^\"\\]+      {
                  m_stringPieces << QString::fromUtf8(yytext);
                }              
  \\            {
                  // ignore
                }
  \"            {
                  m_yylloc->columns(yyleng);
                  if (m_stringPieces.count() > 0) {
                    *m_yylval = QVariant(m_stringPieces.join(QLatin1String("")));
                    m_stringPieces.clear();
                    unput('\"');
                    return yy::json_parser::token::STRING;
                  }
                  else {
                    BEGIN(INITIAL);
                    return yy::json_parser::token::QUOTMARKCLOSE;
                  }
                }
}

<HEX_OPEN>{
  [0-9A-Fa-f]{4} {
                    QString hexDigits = QString::fromUtf8(yytext, yyleng);
                    bool ok;
                    ushort hexDigit1 = hexDigits.left(2).toShort(&ok, 16);
                    ushort hexDigit2 = hexDigits.right(2).toShort(&ok, 16);    
                    m_stringPieces << QChar(hexDigit2, hexDigit1);
                    BEGIN(QUOTMARK_OPEN);
                 }
  .|\n           {
                    qCritical() << "Invalid hex string";
                    m_yylloc->columns(yyleng);
                    *m_yylval = QVariant(QLatin1String(""));
                    BEGIN(QUOTMARK_OPEN);
                    return -1;
                 }
}

<ALLOW_SPECIAL_NUMBERS>{
  (?i:nan)      {
                  m_yylloc->columns(yyleng);
                  return yy::json_parser::token::NAN_VAL;
                }

  [Ii]nfinity   {
                    m_yylloc->columns(yyleng);
                    return yy::json_parser::token::INFINITY_VAL;
                }
}

.             {
                m_yylloc->columns(yyleng);
                qCritical() << "JSonScanner::yylex - unknown char '" << yytext[0] <<"', returning -1";
                return -1;
              }

<<EOF>>       return yy::json_parser::token::END;
