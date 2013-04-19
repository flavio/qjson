/* This file is part of qjson
  *
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

#ifndef QOBJECTHELPER_H
#define QOBJECTHELPER_H

#include "qjson_export.h"

#include <QtCore/QLatin1String>
#include <QtCore/QStringList>
#include <QtCore/QVariantMap>

QT_BEGIN_NAMESPACE
class QObject;
QT_END_NAMESPACE

namespace QJson {
  /**
  * @brief Class used to convert QObject into QVariant and vivce-versa.
  * During these operations only the class attributes defined as properties will
  * be considered.
  *
  * Properties marked as 'non-stored' will be ignored.
  *
  * By default all the QVariants are converted (either invalid and null ones).
  *
  * Suppose the declaration of the Person class looks like this:
  * \code
  * class Person : public QObject
    {
      Q_OBJECT

      Q_PROPERTY(QString name READ name WRITE setName)
      Q_PROPERTY(int phoneNumber READ phoneNumber WRITE setPhoneNumber)
      Q_PROPERTY(Gender gender READ gender WRITE setGender)
      Q_PROPERTY(QDate dob READ dob WRITE setDob)
      Q_ENUMS(Gender)

     public:
        Person(QObject* parent = 0);
        ~Person();

        QString name() const;
        void setName(const QString& name);

        int phoneNumber() const;
        void setPhoneNumber(const int  phoneNumber);

        enum Gender {Male, Female};
        void setGender(Gender gender);
        Gender gender() const;

        QDate dob() const;
        void setDob(const QDate& dob);

      private:
        QString m_name;
        int m_phoneNumber;
        Gender m_gender;
        QDate m_dob;
    };
    \endcode

    The following code will serialize an instance of Person to JSON :

    \code
    Person person;
    person.setName("Flavio");
    person.setPhoneNumber(123456);
    person.setGender(Person::Male);
    person.setDob(QDate(1982, 7, 12));

    QObjectHelper helper;
    QVariantMap variant = helper.qobject2qvariant(&person);
    Serializer serializer;
    qDebug() << serializer.serialize( variant);
    \endcode

    The generated output will be:
    \code
    { "dob" : "1982-07-12", "gender" : 0, "name" : "Flavio", "phoneNumber" : 123456 }
    \endcode

    It's also possible to initialize a QObject using the values stored inside of
    a QVariantMap.

    Suppose you have the following JSON data stored into a QString:
    \code
    { "dob" : "1982-07-12", "gender" : 0, "name" : "Flavio", "phoneNumber" : 123456 }
    \endcode

    The following code will initialize an already allocated instance of Person
    using the JSON values:
    \code
    Parser parser;
    QVariant variant = parser.parse(json);

    Person person;
    QObjectHelper helper;
    helper.qvariant2qobject(variant.toMap(), &person);
    \endcode

    \sa Parser
    \sa Serializer
  */
  class QJSON_EXPORT QObjectHelper {
    public:
      QObjectHelper();
      ~QObjectHelper();

      /**
       * Flag controlling qobject2qvariant() behavior.
       */
      enum VariantConversionRule {
        /**
         * Do not convert either null or invalid QVariants
         */
        Strict,

        /**
         * Generate entries for NULL fields (QVariant::isNull() == true).
         */
        AcceptNullVariants,

        /**
         * Generate entries for INVALID fields (QVariant::isValid() == false).
         */
        AcceptInvalidVariants,

        /**
         * Generate entries for all fields. This is the default flag used.
         */
        AcceptAllVariants = AcceptNullVariants | AcceptInvalidVariants
      };
      Q_DECLARE_FLAGS(VariantConversionRules, VariantConversionRule)

      /**
       * Set QVariant conversion rules affecting the serialization process.
       * @param VariantConversionRules rules The rules to use.
       */
      void setVariantConversionRules(VariantConversionRules rules);

      /**
       * Retrieve the current QVariant conversion rules used for the serialization.
       * @return
       */
      VariantConversionRules variantConversionRules() const;

      /**
       * Set a list of properties to ignore during the serialization process.
       * Note well: the `objectName` property is always going to be ignored.
       * @param QStringList properties: list of properties to ignore.
       */
      void setIgnoredProperties(const QStringList& properties);

      /**
       * Retrieves the list of properties that are going to be ignored during the serialization process.
       * Note well: the `objectName` property is always going to be ignored, but
       * is not going to be part of this QStringList
       */
      QStringList ignoredProperties() const;

      /**
       * Convert Enum to strings, do not use the int value.
       * The default behaviour is to use int notation.
       * @param toggle: convert Enum to strings if toggle is true, use int values otherwise.
       */
      void convertEnumToString(bool toggle);

      bool enumConvertedToString() const;

      /**
      * This method converts a QObject instance into a QVariantMap.
      *
      * @param object The QObject instance to be converted.
      */
      QVariantMap qobject2qvariant(const QObject* object);

      /**
      * This method converts a QVariantMap instance into a QObject
      *
      * @param variant Attributes to assign to the object.
      * @param object The QObject instance to update.
      */
      void qvariant2qobject(const QVariantMap& variant, QObject* object);

    private:
      Q_DISABLE_COPY(QObjectHelper)
      class QObjectHelperPrivate;
      QObjectHelperPrivate* const d;
  };
}

#endif // QOBJECTHELPER_H
