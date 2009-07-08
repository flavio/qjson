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

#include "json_driver.h"
#include "json_driver_p.h"
#include "json_parser.hh"

#include <QtCore/QBuffer>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

JSonDriverPrivate::JSonDriverPrivate() :
    m_scanner(0)
  , m_negate(false)
  , m_error(false)
{
}

JSonDriverPrivate::~JSonDriverPrivate()
{
  delete m_scanner;
}

void JSonDriverPrivate::setError(QString errorMsg, int errorLine) {
  m_error = true;
  m_errorMsg = errorMsg;
  m_errorLine = errorLine;
}

JSonDriver::JSonDriver() :
    d(new JSonDriverPrivate)
{
}

JSonDriver::~JSonDriver()
{
  delete d;
}

QVariant JSonDriver::parse (QIODevice* io, bool* ok)
{
  d->m_errorMsg.clear();
  delete d->m_scanner;
  d->m_scanner = 0;

  if (!io->isOpen()) {
    if (!io->open(QIODevice::ReadOnly)) {
      if (ok != 0)
        *ok = false;
      qCritical ("Error opening device");
      return QVariant();
    }
  }

  if (!io->isReadable()) {
    if (ok != 0)
      *ok = false;
    qCritical ("Device is not readable");
    io->close();
    return QVariant();
  }

  d->m_scanner = new JSonScanner (io);
  yy::json_parser parser (d);
  parser.parse ();

  delete d->m_scanner;
  d->m_scanner = 0;

  if (ok != 0)
    *ok = !d->m_error;

  io->close();
  return d->m_result;
}

QVariant JSonDriver::parse(const QString& jsonString, bool* ok) {
  QBuffer buffer;
  buffer.open(QBuffer::ReadWrite);
  buffer.write(jsonString.toAscii());
  buffer.seek(0);
  return parse (&buffer, ok);
}

void JSonDriver::serialize( const QVariant& v, QIODevice* io, bool* ok )
{
  if (!io->isOpen()) {
    if (!io->open(QIODevice::WriteOnly)) {
      if ( ok != 0 )
        *ok = false;
      qCritical ("Error opening device");
      return;
    }
  }

  if (!io->isWritable()) {
    if (ok != 0)
      *ok = false;
    qCritical ("Device is not readable");
    io->close();
    return;
  }

  bool error = false;
  const QString str = serialize( v );
  if ( !str.isNull() ) {
    QTextStream stream( io );
    stream << str;
  } else {
    if ( ok )
      *ok = error;
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

QString JSonDriver::error() const
{
  return d->m_errorMsg;
}

int JSonDriver::errorLine() const
{
  return d->m_errorLine;
}
