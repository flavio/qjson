
/* This file is part of QJson
 *
 * Copyright (C) 2009 Flavio Castelli <flavio.castelli@gmail.com>
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

#include <limits>

#include <QtCore/QVariant>
#include <QtCore/QVariantList>

#include <QtTest/QtTest>

#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>

#include "person.h"

class TestQObjectHelper: public QObject
{
  Q_OBJECT
  private slots:
    void testQObject2QVariant();
    void testQVariant2QObject();
    void testIgnorePropertiesDuringQVariantConversion();
    void testEnumToStringConversionDuringQObject2QVariantConversion();
    void testQVariant2QObjectWithEnumConvertedToString();
    void testQObject2QVariantStrictVariantConversionRule();
    void testQObject2QVariantAllowInvalidVariantsRule();
    void testQObject2QVariantAllowNullVariantsRule();
    void testQObject2QVariantAllowAllVariantsRule();
};

using namespace QJson;

void TestQObjectHelper::testQObject2QVariant()
{
  QString name = QLatin1String("Flavio Castelli");
  int phoneNumber = 123456;
  Person::Gender gender = Person::Male;
  QDate dob (1982, 7, 12);
  QVariantList nicknames;
  nicknames << QLatin1String("nickname1") << QLatin1String("nickname2");
  quint16 luckyNumber = 123;

  Person person;
  person.setName(name);
  person.setPhoneNumber(phoneNumber);
  person.setGender(gender);
  person.setDob(dob);
  person.setCustomField(nicknames);
  person.setLuckyNumber(luckyNumber);

  QVariantMap expected;
  expected[QLatin1String("name")] = QVariant(name);
  expected[QLatin1String("phoneNumber")] = QVariant(phoneNumber);
  expected[QLatin1String("gender")] = QVariant(gender);
  expected[QLatin1String("dob")] = QVariant(dob);
  expected[QLatin1String("customField")] = nicknames;
  expected[QLatin1String("luckyNumber")] = luckyNumber;

  QObjectHelper helper;
  QVariantMap result = helper.qobject2qvariant(&person);
  QCOMPARE(result, expected);
}

void TestQObjectHelper::testQVariant2QObject()
{
  QObjectHelper helper;
  bool ok;
  QString name = QLatin1String("Flavio Castelli");
  int phoneNumber = 123456;
  Person::Gender gender = Person::Male;
  QDate dob (1982, 7, 12);
  QVariantList nicknames;
  nicknames << QLatin1String("nickname1") << QLatin1String("nickname2");
  quint16 luckyNumber = 123;

  Person expected_person;
  expected_person.setName(name);
  expected_person.setPhoneNumber(phoneNumber);
  expected_person.setGender(gender);
  expected_person.setDob(dob);
  expected_person.setCustomField(nicknames);
  expected_person.setLuckyNumber(luckyNumber);

  QVariantMap variant = helper.qobject2qvariant(&expected_person);

  Serializer serializer;
  QByteArray json = serializer.serialize(variant, &ok);
  QVERIFY(ok);

  Parser parser;
  QVariant parsedVariant = parser.parse(json,&ok);
  QVERIFY(ok);
  QVERIFY(parsedVariant.canConvert(QVariant::Map));

  Person person;
  QCOMPARE(Person::Female, person.gender());
  helper.qvariant2qobject(parsedVariant.toMap(), &person);

  QCOMPARE(person.name(), name);
  QCOMPARE(person.phoneNumber(), phoneNumber);
  QCOMPARE(person.gender(), gender);
  QCOMPARE(person.dob(), dob);
  QCOMPARE(person.customField(), QVariant(nicknames));
  QCOMPARE(person.luckyNumber(), luckyNumber);
}

void TestQObjectHelper::testIgnorePropertiesDuringQVariantConversion()
{
  QString name = QLatin1String("Flavio Castelli");
  int phoneNumber = 123456;
  Person::Gender gender = Person::Male;
  QDate dob (1982, 7, 12);
  QVariantList nicknames;
  nicknames << QLatin1String("nickname1") << QLatin1String("nickname2");
  quint16 luckyNumber = 123;

  Person person;
  person.setName(name);
  person.setPhoneNumber(phoneNumber);
  person.setGender(gender);
  person.setDob(dob);
  person.setCustomField(nicknames);
  person.setLuckyNumber(luckyNumber);

  QVariantMap expected;
  expected[QLatin1String("name")] = QVariant(name);
  expected[QLatin1String("phoneNumber")] = QVariant(phoneNumber);
  expected[QLatin1String("gender")] = QVariant(gender);
  expected[QLatin1String("dob")] = QVariant(dob);
  expected[QLatin1String("customField")] = nicknames;

  QObjectHelper helper;
  helper.setIgnoredProperties(QStringList(QLatin1String("luckyNumber")));
  QVariantMap result = helper.qobject2qvariant(&person);
  QCOMPARE(result, expected);
}

void TestQObjectHelper::testEnumToStringConversionDuringQObject2QVariantConversion() {
  QString name = QLatin1String("Flavio Castelli");
  int phoneNumber = 123456;
  Person::Gender gender = Person::Male;
  QDate dob (1982, 7, 12);
  QVariantList nicknames;
  nicknames << QLatin1String("nickname1") << QLatin1String("nickname2");
  quint16 luckyNumber = 123;

  Person person;
  person.setName(name);
  person.setPhoneNumber(phoneNumber);
  person.setGender(gender);
  person.setDob(dob);
  person.setCustomField(nicknames);
  person.setLuckyNumber(luckyNumber);

  QVariantMap expected;
  expected[QLatin1String("name")] = QVariant(name);
  expected[QLatin1String("phoneNumber")] = QVariant(phoneNumber);
  expected[QLatin1String("gender")] = QVariant(QLatin1String("Male"));
  expected[QLatin1String("dob")] = QVariant(dob);
  expected[QLatin1String("customField")] = nicknames;
  expected[QLatin1String("luckyNumber")] = luckyNumber;

  QObjectHelper helper;
  helper.convertEnumToString(true);
  QVariantMap result = helper.qobject2qvariant(&person);
  QCOMPARE(result, expected);
}

void TestQObjectHelper::testQVariant2QObjectWithEnumConvertedToString()
{
  QObjectHelper helper;
  helper.convertEnumToString(true);

  bool ok;
  QString name = QLatin1String("Flavio Castelli");
  int phoneNumber = 123456;
  Person::Gender gender = Person::Male;
  QDate dob (1982, 7, 12);
  QVariantList nicknames;
  nicknames << QLatin1String("nickname1") << QLatin1String("nickname2");
  quint16 luckyNumber = 123;

  Person expected_person;
  expected_person.setName(name);
  expected_person.setPhoneNumber(phoneNumber);
  expected_person.setGender(gender);
  expected_person.setDob(dob);
  expected_person.setCustomField(nicknames);
  expected_person.setLuckyNumber(luckyNumber);

  QVariantMap variant = helper.qobject2qvariant(&expected_person);
  QCOMPARE(variant[QLatin1String("gender")].toString(), QLatin1String("Male"));

  Serializer serializer;
  QByteArray json = serializer.serialize(variant, &ok);
  QVERIFY(ok);

  Parser parser;
  QVariant parsedVariant = parser.parse(json,&ok);
  QVERIFY(ok);
  QVERIFY(parsedVariant.canConvert(QVariant::Map));

  Person person;
  QCOMPARE(Person::Female, person.gender());
  helper.qvariant2qobject(parsedVariant.toMap(), &person);

  QCOMPARE(person.name(), name);
  QCOMPARE(person.phoneNumber(), phoneNumber);
  QCOMPARE(person.gender(), gender);
  QCOMPARE(person.dob(), dob);
  QCOMPARE(person.customField(), QVariant(nicknames));
  QCOMPARE(person.luckyNumber(), luckyNumber);
}

void TestQObjectHelper::testQObject2QVariantStrictVariantConversionRule()
{
  QString name;             //NULL variant
  int phoneNumber = 123456;
  Person::Gender gender = Person::Male;
  QDate dob;                //NULL variant
  QVariant nicknames;       //INVALID variant
  quint16 luckyNumber = 123;

  Person person;
  person.setName(name);
  person.setPhoneNumber(phoneNumber);
  person.setGender(gender);
  person.setDob(dob);
  person.setCustomField(nicknames);
  person.setLuckyNumber(luckyNumber);

  QVariantMap expected;
  expected[QLatin1String("phoneNumber")] = QVariant(phoneNumber);
  expected[QLatin1String("gender")] = QVariant(gender);
  expected[QLatin1String("luckyNumber")] = luckyNumber;

  QObjectHelper helper;
  helper.setVariantConversionRules(QObjectHelper::Strict);
  QVariantMap result = helper.qobject2qvariant(&person);
  QCOMPARE(result, expected);
}

void TestQObjectHelper::testQObject2QVariantAllowInvalidVariantsRule()
{
  QString name;             //NULL variant
  int phoneNumber = 123456;
  Person::Gender gender = Person::Male;
  QDate dob;                //NULL variant
  QVariant nicknames;       //INVALID variant
  quint16 luckyNumber = 123;

  Person person;
  person.setName(name);
  person.setPhoneNumber(phoneNumber);
  person.setGender(gender);
  person.setDob(dob);
  person.setCustomField(nicknames);
  person.setLuckyNumber(luckyNumber);

  QVariantMap expected;
  expected[QLatin1String("phoneNumber")] = QVariant(phoneNumber);
  expected[QLatin1String("gender")] = QVariant(gender);
  expected[QLatin1String("customField")] = nicknames;
  expected[QLatin1String("luckyNumber")] = luckyNumber;

  QObjectHelper helper;
  helper.setVariantConversionRules(QObjectHelper::AcceptInvalidVariants);
  QVariantMap result = helper.qobject2qvariant(&person);
  qDebug() << "merda" << result;
  QCOMPARE(result, expected);
}

void TestQObjectHelper::testQObject2QVariantAllowNullVariantsRule()
{
  QString name;             //NULL variant
  int phoneNumber = 123456;
  Person::Gender gender = Person::Male;
  QDate dob;                //NULL variant
  QVariant nicknames;       //INVALID variant
  quint16 luckyNumber = 123;

  Person person;
  person.setName(name);
  person.setPhoneNumber(phoneNumber);
  person.setGender(gender);
  person.setDob(dob);
  person.setCustomField(nicknames);
  person.setLuckyNumber(luckyNumber);

  QVariantMap expected;
  expected[QLatin1String("name")] = QVariant(name);
  expected[QLatin1String("phoneNumber")] = QVariant(phoneNumber);
  expected[QLatin1String("gender")] = QVariant(gender);
  expected[QLatin1String("dob")] = QVariant(dob);
  expected[QLatin1String("luckyNumber")] = luckyNumber;

  QObjectHelper helper;
  helper.setVariantConversionRules(QObjectHelper::AcceptNullVariants);
  QVariantMap result = helper.qobject2qvariant(&person);
  QCOMPARE(result, expected);
}

void TestQObjectHelper::testQObject2QVariantAllowAllVariantsRule()
{
  QString name;             //NULL variant
  int phoneNumber = 123456;
  Person::Gender gender = Person::Male;
  QDate dob;                //NULL variant
  QVariant nicknames;       //INVALID variant
  quint16 luckyNumber = 123;

  Person person;
  person.setName(name);
  person.setPhoneNumber(phoneNumber);
  person.setGender(gender);
  person.setDob(dob);
  person.setCustomField(nicknames);
  person.setLuckyNumber(luckyNumber);

  QVariantMap expected;
  expected[QLatin1String("name")] = QVariant(name);
  expected[QLatin1String("phoneNumber")] = QVariant(phoneNumber);
  expected[QLatin1String("gender")] = QVariant(gender);
  expected[QLatin1String("dob")] = QVariant(dob);
  expected[QLatin1String("customField")] = nicknames;
  expected[QLatin1String("luckyNumber")] = luckyNumber;

  QObjectHelper helper;
  helper.setVariantConversionRules(QObjectHelper::AcceptAllVariants);
  QVariantMap result = helper.qobject2qvariant(&person);
  QCOMPARE(result, expected);
}

QTEST_MAIN(TestQObjectHelper)
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
// using Qt4 rather then Qt5
  #include "moc_testqobjecthelper.cxx"
#else
  #include "testqobjecthelper.moc"
#endif
