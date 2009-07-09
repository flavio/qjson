#include "serializer.h"

#include <QStringList>
#include <QTextStream>
#include <QVariant>

using namespace QJSon;

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

  const QString str = serialize( v );
  if ( !str.isNull() ) {
    QTextStream stream( io );
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

QString Serializer::serialize( const QVariant &v )
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
      QMapIterator<QString, QVariant> it( vmap );
      str = QLatin1String("{ ");
      while ( it.hasNext() ) {
        it.next();
        str += sanitizeString(it.key()) + QLatin1String(" : ") + serialize(it.value());
      }
      str += QLatin1String(" }");
    }
  }
  if ( !error )
    return str;
  else
    return QString();
}

