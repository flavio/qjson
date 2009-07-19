/* This file is part of qjson
  *
  * Copyright (C) 2009 Till Adam <adam@kde.org>
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

#include "serializer.h"

#include <QDataStream>
#include <QStringList>
#include <QVariant>

using namespace QJson;

class Serializer::SerializerPrivate {

};

Serializer::Serializer() : d( new SerializerPrivate ) {
}

Serializer::~Serializer() {
  delete d;
}

void Serializer::serialize( const QVariant& v, QIODevice* io, bool* ok )
{
  Q_ASSERT( io );
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

  const QByteArray str = serialize( v );
  if ( !str.isNull() ) {
    QDataStream stream( io );
    stream << str;
  } else {
    if ( ok )
      *ok = false;
  }
}

static QString sanitizeString( const QString& s )
{
  const QString str = QLatin1String("\"") + s + QLatin1String("\"");
  return str;
}

static QByteArray join( const QList<QByteArray>& list, const QByteArray& sep ) {
  QByteArray res;
  Q_FOREACH( const QByteArray& i, list ) {
    if ( !res.isEmpty() )
      res += sep;
    res += i;
  }
  return res;
}

QByteArray Serializer::serialize( const QVariant &v )
{
  if ( !v.isValid() )
    return ""; // no parse error
  QByteArray str;
  bool error = false;

  // two major cases, either it's an array or ... not
  if ( v.canConvert<QVariantList>() ) {
    const QVariantList list = v.toList();
    QList<QByteArray> values;
    Q_FOREACH( const QVariant& v, list )
      values << serialize( v );
    str = "[ " + join( values, ", " ) + " ]";
  } else {
    // not a list, so it must be an object
    if ( !v.canConvert<QVariantMap>() ) {
      // not a map, so it must be a value
      if ( v.type() == QVariant::String )
        str = sanitizeString( v.toString() ).toUtf8();
      else
        str = v.toByteArray();
    } else {
      const QVariantMap vmap = v.toMap();
      QMapIterator<QString, QVariant> it( vmap );
      str = "{ ";
      QList<QByteArray> pairs;
      while ( it.hasNext() ) {
        it.next();
        pairs << sanitizeString(it.key()).toUtf8() + " : " + serialize(it.value() );
      }
      str += join( pairs, ", " );
      str += " }";
    }
  }
  if ( !error )
    return str;
  else
    return QByteArray();
}

