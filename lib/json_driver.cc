/* This file is part of QJson
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

#include "json_driver.hh"
#include "json_parser.hh"

#include <QBuffer>

JSonDriver::JSonDriver()
{
  m_scanner = 0;
  m_negate = false;
  m_error = false;
}

JSonDriver::~JSonDriver()
{
  if (m_scanner)
    delete m_scanner;
}

QVariant JSonDriver::parse (QIODevice* io, bool* status)
{
  if (m_scanner)
    delete m_scanner;

  m_scanner = new JSonScanner (io);
  yy::json_parser parser (this);
  parser.parse ();

  delete m_scanner;
  m_scanner = 0;

  if (status != 0)
    *status = m_error;
  
  return m_result;
}

QVariant JSonDriver::parse(const QString& jsonString, bool* status) {
  QBuffer buffer;
  buffer.open(QBuffer::ReadWrite);
  buffer.write(jsonString.toAscii());
  buffer.seek(0);
  return parse (&buffer, status);
}

