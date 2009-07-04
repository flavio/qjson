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
#include <QStringList>
#include <QTextStream>
#include <QDebug>

JSonDriver::JSonDriver() :
    m_scanner(0)
  , m_negate(false)
  , m_error(false)
  , m_errorMsg()
{
}

JSonDriver::~JSonDriver()
{
  delete m_scanner;
}

void JSonDriver::setError(QString errorMsg, int errorLine) {
  m_error = true;
  m_errorMsg = errorMsg;
  m_errorLine = errorLine;
}

QVariant JSonDriver::parse (QIODevice* io, bool* status)
{
  m_errorMsg.clear();
  delete m_scanner;

  if (!io->isOpen()) {
    if (!io->open(QIODevice::ReadOnly)) {
      if (status != 0)
        *status = false;
      qFatal ("Error opening device");
      return QVariant();
    }
  }

  if (!io->isReadable()) {
    if (status != 0)
      *status = false;
    qFatal ("Device is not readable");
    io->close();
    return QVariant();
  }

  m_scanner = new JSonScanner (io);
  yy::json_parser parser (this);
  parser.parse ();

  delete m_scanner;
  m_scanner = 0;

  if (status != 0)
    *status = m_error;

  io->close();
  return m_result;
}

QVariant JSonDriver::parse(const QString& jsonString, bool* status) {
  QBuffer buffer;
  buffer.open(QBuffer::ReadWrite);
  buffer.write(jsonString.toAscii());
  buffer.seek(0);
  return parse (&buffer, status);
}

void JSonDriver::serialize( const QVariant& v, QIODevice* io, bool* b )
{
  if (!io->isOpen()) {
    if (!io->open(QIODevice::WriteOnly)) {
      if ( b != 0 )
        *b = false;
      qFatal ("Error opening device");
      return;
    }
  }

  if (!io->isWritable()) {
    if (b != 0)
      *b = false;
    qFatal ("Device is not readable");
    io->close();
    return;
  }

  bool error = false;
  const QString str = serialize( v );
  if ( !str.isNull() ) {
    QTextStream stream( io );
    stream << str;
  } else {
    if ( b )
      *b = error;
  }
}

static QString sanitizeString( const QString& s )
{
  return QLatin1String("\"") + s + QLatin1String("\"");
}

const QString JSonDriver::serialize( const QVariant &v )
{
  if ( !v.isValid() )
    return QString(""); // no parse error
  QString str;
  bool error = false;

  // two major cases, either it's an array or ... not
  if ( v.canConvert<QVariantList>() ) {
    const QVariantList list = v.toList();
    QStringList values;
    Q_FOREACH( const QVariant& v, list ) {
      values << serialize( v );
    }
    str = QLatin1String("[ ") + values.join(", ") + QLatin1String(" ]");
  } else {
    // not a list, so it must be an object
    if ( !v.canConvert<QVariantMap>() ) {
      // not a map, so it must be a value
      if ( v.type() == QVariant::String )
        str = sanitizeString( v.toString() );
      else
        str = v.toString();
    } else {
      const QVariantMap vmap = v.toMap();
      QStringList values;
      QMapIterator<QString, QVariant> it( vmap );
      while ( it.hasNext() ) {
        it.next();
        QString value = it.value().toString();
        if ( it.value().type() == QVariant::String )
          value = sanitizeString( value );
      }
      str = QLatin1String("{ ") + values.join(", ") + QLatin1String(" }");
    }
  }
  if ( !error )
    return str;
  else
    return QString();
}
