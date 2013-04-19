/* This file is part of qjson
  *
  * Copyright (C) 2009 Till Adam <adam@kde.org>
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
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


#include "qobjecthelper.h"

#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QObject>

using namespace QJson;

class QObjectHelper::QObjectHelperPrivate {
  public:
    QObjectHelperPrivate() {
      convertEnumToString = false;
      variantConversionRules = AcceptAllVariants;
    }

    QVariantMap qobject2qvariant(const QObject* object);
    void qvariant2qobject(const QVariantMap& variant, QObject* object);


    QStringList ignoredProperties;
    bool convertEnumToString;
    QObjectHelper::VariantConversionRules variantConversionRules;
};

QVariantMap QObjectHelper::QObjectHelperPrivate::qobject2qvariant(const QObject* object)
{
  QVariantMap result;
  QString objectNameString = QLatin1String("objectName");

  const QMetaObject *metaobject = object->metaObject();
  int count = metaobject->propertyCount();
  for (int i=0; i<count; ++i) {
    QMetaProperty metaproperty = metaobject->property(i);
    const char *name        = metaproperty.name();
    QLatin1String latinName = QLatin1String(name);

    if ((objectNameString == latinName) || (ignoredProperties.contains(latinName)) || (!metaproperty.isReadable()))
      continue;

    QVariant value;

    if (metaproperty.isEnumType() && convertEnumToString) {
      QMetaEnum metaEnum = metaproperty.enumerator();
      value = QLatin1String(metaEnum.valueToKey(object->property(name).toInt()));
    } else {
      value = object->property(name);
    }

    if (value.isValid() && value.isNull() && !variantConversionRules.testFlag(AcceptNullVariants))
        continue;
    if (!value.isValid() && !variantConversionRules.testFlag(AcceptInvalidVariants))
        continue;
    result[latinName] = value;
 }
  return result;

}

void QObjectHelper::QObjectHelperPrivate::qvariant2qobject(const QVariantMap& variant, QObject* object)
{
  const QMetaObject *metaobject = object->metaObject();

  QVariantMap::const_iterator iter;
  for (iter = variant.constBegin(); iter != variant.constEnd(); ++iter) {
    int pIdx = metaobject->indexOfProperty( iter.key().toLatin1() );

    if ( pIdx < 0 ) {
      continue;
    }

    QMetaProperty metaproperty = metaobject->property( pIdx );
    QVariant::Type type = metaproperty.type();
    QVariant v( iter.value() );

    if ((metaproperty.isEnumType()) && type == QVariant::String)
    {
      QMetaEnum metaEnum = metaproperty.enumerator();
      type = QVariant::Int;
      v = metaEnum.keyToValue(v.toByteArray().data());
    }

    if ( v.canConvert( type ) ) {
      v.convert( type );
      metaproperty.write( object, v );
    } else if (QString(QLatin1String("QVariant")).compare(QLatin1String(metaproperty.typeName())) == 0) {
     metaproperty.write( object, v );
    }
  }
}


QObjectHelper::QObjectHelper()
  : d (new QObjectHelperPrivate)
{
}

QObjectHelper::~QObjectHelper()
{
  delete d;
}

QVariantMap QObjectHelper::qobject2qvariant(const QObject* object)
{
  return d->qobject2qvariant(object);
}

void QObjectHelper::qvariant2qobject(const QVariantMap& variant, QObject* object)
{
  d->qvariant2qobject(variant, object);
}

void QObjectHelper::setIgnoredProperties(const QStringList& properties)
{
  d->ignoredProperties = properties;
}

QStringList QObjectHelper::ignoredProperties() const
{
  return d->ignoredProperties;
}

void QObjectHelper::convertEnumToString(bool toggle)
{
  d->convertEnumToString = toggle;
}

bool QObjectHelper::enumConvertedToString() const
{
  return d->convertEnumToString;
}

void QObjectHelper::setVariantConversionRules(QObjectHelper::VariantConversionRules rules)
{
  d->variantConversionRules = rules;
}

QObjectHelper::VariantConversionRules QObjectHelper::variantConversionRules() const
{
  return d->variantConversionRules;
}

