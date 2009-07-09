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

#include <QtTest/QtTest>

#include "json_driver.h"
#include "serializer.h"

#include <QtCore/QVariant>

using namespace QJSon;

class TestJSonDriver: public QObject
{
  Q_OBJECT
  private slots:
    void parseNonAsciiString();
    void parseSimpleObject();
    void parseEmptyObject();
    void parseUrl();
    void parseMultipleObject();

    void parseSimpleArray();
    void parseInvalidObject();
    void parseMultipleArray();

    void testTrueFalseNullValues();
    void testEscapeChars();
    void testNumbers();

    void testReadWriteEmptyDocument();
    void testSerialize();
    void testSerialize_data();
    void testReadWrite();
    void testReadWrite_data();
};

Q_DECLARE_METATYPE(QVariant)

void TestJSonDriver::parseSimpleObject() {
  QString json = "{\"foo\":\"bar\"}";
  QVariantMap map;
  map.insert ("foo", "bar");
  QVariant expected(map);

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestJSonDriver::parseEmptyObject() {
  QString json = "{}";
  QVariantMap map;
  QVariant expected (map);

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestJSonDriver::parseInvalidObject() {
  QString json = "{\"foo\":\"bar\"";

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (!ok);
}

void TestJSonDriver::parseNonAsciiString() {
  QString json = "{\"artist\":\"Queensr\\u00ffche\"}";
  QVariantMap map;

  QChar unicode_char (0x00ff);
  QString unicode_string;
  unicode_string.setUnicode(&unicode_char, 1);
  unicode_string = "Queensr" + unicode_string + "che";
  
  map.insert ("artist", unicode_string);
  QVariant expected (map);

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestJSonDriver::parseMultipleObject() {
  //put also some extra spaces inside the json string
  QString json = "{ \"foo\":\"bar\",\n\"number\" : 51.3 , \"array\":[\"item1\", 123]}";
  QVariantMap map;
  map.insert ("foo", "bar");
  map.insert ("number", 51.3);
  QVariantList list;
  list.append (QString("item1"));
  list.append (QString("123"));
  map.insert ("array", list);
  QVariant expected (map);

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
  QVERIFY (result.toMap()["number"].canConvert<float>());
  QVERIFY (result.toMap()["array"].canConvert<QVariantList>());
}

void TestJSonDriver::parseUrl(){
  //"http:\/\/www.last.fm\/venue\/8926427"
  QString json = "[\"http:\\/\\/www.last.fm\\/venue\\/8926427\"]";
  QVariantList list;
  list.append (QVariant(QString("http://www.last.fm/venue/8926427")));
  QVariant expected (list);

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

 void TestJSonDriver::parseSimpleArray() {
  QString json = "[\"foo\",\"bar\"]";
  QVariantList list;
  list.append (QVariant(QString("foo")));
  list.append (QVariant(QString("bar")));
  QVariant expected (list);

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestJSonDriver::parseMultipleArray() {
  //put also some extra spaces inside the json string
  QString json = "[ {\"foo\":\"bar\"},\n\"number\",51.3 , [\"item1\", 123]]";
  QVariantMap map;
  map.insert ("foo", "bar");

  QVariantList array;
  array.append (QString("item1"));
  array.append (123);
  
  QVariantList list;
  list.append (map);
  list.append (QString("number"));
  list.append (QString("51.3"));
  list.append ((QVariant) array);

  QVariant expected (list);

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestJSonDriver::testTrueFalseNullValues() {
  QString json = "[true,false, null, {\"foo\" : true}]";
  QVariantList list;
  list.append (QVariant(true));
  list.append (QVariant(false));
  list.append (QVariant());
  QVariantMap map;
  map.insert ("foo", true);
  list.append (map);
  QVariant expected (list);

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
  QCOMPARE (result.toList().at(0).toBool(), true);
  QCOMPARE (result.toList().at(1).toBool(), false);
  QVERIFY (result.toList().at(2).isNull());
}

void TestJSonDriver::testEscapeChars() {
  QString json = "[\"\\b \\f \\n \\r \\t \", \" \\\\ \\/ \\\" \", \"http://foo.com\"]";

  QVariantList list;
  list.append (QVariant("\b \f \n \r \t "));
  list.append (QVariant(" \\\\ / \\\" "));
  list.append (QVariant("http://foo.com"));

  QVariant expected (list);

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestJSonDriver::testNumbers() {
  QString json = "[1,2.4, -100, -3.4, -5e+, 2e,3e+,4.3E,5.4E-]";
  QVariantList list;
  list.append (QVariant(1));
  list.append (QVariant(2.4));
  list.append (QVariant(-100));
  list.append (QVariant(-3.4));
  list.append (QVariant("-5e+"));
  list.append (QVariant("2e"));
  list.append (QVariant("3e+"));
  list.append (QVariant("4.3E"));
  list.append (QVariant("5.4E-"));
  QVariant expected (list);

  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);

  QVariantList numbers = result.toList();
  QCOMPARE( numbers[0].type(),QVariant::Int );
  QCOMPARE( numbers[1].type(), QVariant::Double );
  QCOMPARE( numbers[2].type(), QVariant::Int );
  QCOMPARE( numbers[3].type(), QVariant::Double );
}

void TestJSonDriver::testReadWriteEmptyDocument()
{
  QString json = QString("");
  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse( json, &ok );
  QVERIFY(ok);
  Serializer serializer;
  const QString serialized = serializer.serialize( result );
  QVERIFY( !serialized.isNull() );
  QVERIFY( serialized.isEmpty() );
}

void TestJSonDriver::testSerialize()
{
  QFETCH( QVariant, qvariant );
  QFETCH( QString, expected );
  Serializer serializer;
  bool ok;
  QString result = serializer.serialize( qvariant);
  result = result.replace(" ", "");
  expected = expected.replace(" ", "");
  QCOMPARE(result, expected);
}

void TestJSonDriver::testSerialize_data()
{
    QTest::addColumn<QVariant>( "qvariant" );
    QTest::addColumn<QString>( "expected" );
    
    // array tests
    QVariantList array;
    array.clear();
    QTest::newRow( "empty array") << (QVariant) array << "[]";

    array << QVariant("foo") << QVariant("bar");
    QTest::newRow( "basic array") << (QVariant)  array << "[\"foo\",\"bar\"]";

    array.clear();
    array << QVariant(6);
    QTest::newRow( "int array") << (QVariant)  array << "[6]";

    // document tests
    QVariantMap map;
    map.clear();
    QTest::newRow( "empty object") << (QVariant)  map << "{}";

    map["foo"] = QVariant("bar");
    QTest::newRow( "basic document") << (QVariant) map << "{ \"foo\":\"bar\" }";

    map["foo"] = QVariant(6);
    QTest::newRow( "object with ints" ) << (QVariant) map << "{ \"foo\":6 }";
}


void TestJSonDriver::testReadWrite()
{
  QFETCH( QString, json );
  JSonDriver driver;
  bool ok;
  QVariant result = driver.parse( json, &ok );
  QVERIFY(ok);
  Serializer serializer;
  const QString serialized = serializer.serialize( result );
//  qWarning() << serialized;
  QVariant writtenThenRead = driver.parse( serialized, &ok );
  QCOMPARE( result, writtenThenRead );
}

void TestJSonDriver::testReadWrite_data()
{
    QTest::addColumn<QString>( "json" );

    // array tests
    QTest::newRow( "empty array" ) << "[]";
    QTest::newRow( "basic array" ) << "[\"foo\",\"bar\"]";
    QTest::newRow( "single int array" ) << "[6]";
    QTest::newRow( "int array" ) << "[6,5,6,7]";
    const QString json = "[1,2.4, -100, -3.4, -5e+, 2e,3e+,4.3E,5.4E-]";
    QTest::newRow( "array of various numbers" ) << json;

    // document tests
    QTest::newRow( "empty object" ) << "{}";
    QTest::newRow( "basic document" ) << "{\"foo\":\"bar\"}";
    QTest::newRow( "object with ints" ) << "{\"foo\":6}";
    QString json2 = "{ \"foo\":\"bar\",\n\"number\" : 51.3 , \"array\":[\"item1\", 123]}";
    QTest::newRow( "complicated document" ) << json2;

    // more complex cases
    const QString json3 = "[ {\"foo\":\"bar\"},\n\"number\",51.3 , [\"item1\", 123]]";
    QTest::newRow( "complicated array" ) << json3;
}

QTEST_MAIN(TestJSonDriver)
#include "moc_testjsondriver.cxx"
