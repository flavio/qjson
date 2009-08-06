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

#include "parser.h"
#include "serializer.h"

#include <QtCore/QVariant>

class TestParser: public QObject
{
  Q_OBJECT
  private slots:
    void parseNonAsciiString();
    void parseSimpleObject();
    void parseEmptyObject();
    void parseEmptyValue();
    void parseUrl();
    void parseMultipleObject();

    void parseSimpleArray();
    void parseInvalidObject();
    void parseMultipleArray();

    void testTrueFalseNullValues();
    void testEscapeChars();
    void testNumbers();

    void testReadWriteEmptyDocument();
    void testReadWrite();
    void testReadWrite_data();
};

Q_DECLARE_METATYPE(QVariant)

using namespace QJson;

void TestParser::parseSimpleObject() {
  QByteArray json = "{\"foo\":\"bar\"}";
  QVariantMap map;
  map.insert (QLatin1String("foo"), QLatin1String("bar"));
  QVariant expected(map);

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestParser::parseEmptyObject() {
  QByteArray json = "{}";
  QVariantMap map;
  QVariant expected (map);

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestParser::parseEmptyValue() {
  QByteArray json = "{\"value\": \"\"}";

  QVariantMap map;
  map.insert (QLatin1String("value"), QLatin1String(""));
  QVariant expected (map);

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
  QVERIFY (result.toMap().value(QLatin1String("value")).canConvert<QString>());

  QString value = result.toMap().value(QLatin1String("value")).toString();
  QVERIFY (value.isEmpty());
}

void TestParser::parseInvalidObject() {
  QByteArray json = "{\"foo\":\"bar\"";

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (!ok);
}

void TestParser::parseNonAsciiString() {
  QByteArray json = "{\"artist\":\"Queensr\\u00ffche\"}";
  QVariantMap map;

  QChar unicode_char (0x00ff);
  QString unicode_string;
  unicode_string.setUnicode(&unicode_char, 1);
  unicode_string = QLatin1String("Queensr") + unicode_string + QLatin1String("che");
  
  map.insert (QLatin1String("artist"), unicode_string);
  QVariant expected (map);

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestParser::parseMultipleObject() {
  //put also some extra spaces inside the json string
  QByteArray json = "{ \"foo\":\"bar\",\n\"number\" : 51.3 , \"array\":[\"item1\", 123]}";
  QVariantMap map;
  map.insert (QLatin1String("foo"), QLatin1String("bar"));
  map.insert (QLatin1String("number"), 51.3);
  QVariantList list;
  list.append (QLatin1String("item1"));
  list.append (QLatin1String("123"));
  map.insert (QLatin1String("array"), list);
  QVariant expected (map);

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
  QVERIFY (result.toMap().value(QLatin1String("number")).canConvert<float>());
  QVERIFY (result.toMap().value(QLatin1String("array")).canConvert<QVariantList>());
}

void TestParser::parseUrl(){
  //"http:\/\/www.last.fm\/venue\/8926427"
  QByteArray json = "[\"http:\\/\\/www.last.fm\\/venue\\/8926427\"]";
  QVariantList list;
  list.append (QVariant(QLatin1String("http://www.last.fm/venue/8926427")));
  QVariant expected (list);

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

 void TestParser::parseSimpleArray() {
  QByteArray json = "[\"foo\",\"bar\"]";
  QVariantList list;
  list.append (QLatin1String("foo"));
  list.append (QLatin1String("bar"));
  QVariant expected (list);

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestParser::parseMultipleArray() {
  //put also some extra spaces inside the json string
  QByteArray json = "[ {\"foo\":\"bar\"},\n\"number\",51.3 , [\"item1\", 123]]";
  QVariantMap map;
  map.insert (QLatin1String("foo"), QLatin1String("bar"));

  QVariantList array;
  array.append (QLatin1String("item1"));
  array.append (123);
  
  QVariantList list;
  list.append (map);
  list.append (QLatin1String("number"));
  list.append (QLatin1String("51.3"));
  list.append ((QVariant) array);

  QVariant expected (list);

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
}

void TestParser::testTrueFalseNullValues() {
  QByteArray json = "[true,false, null, {\"foo\" : true}]";
  QVariantList list;
  list.append (QVariant(true));
  list.append (QVariant(false));
  list.append (QVariant());
  QVariantMap map;
  map.insert (QLatin1String("foo"), true);
  list.append (map);
  QVariant expected (list);

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result, expected);
  QCOMPARE (result.toList().at(0).toBool(), true);
  QCOMPARE (result.toList().at(1).toBool(), false);
  QVERIFY (result.toList().at(2).isNull());
}

void TestParser::testEscapeChars() {
  QByteArray json = "[\"\\b \\f \\n \\r \\t \", \" \\\\ \\/ \\\" \", \"http://foo.com\"]";

  QVariantList list;
  list.append (QLatin1String("\b \f \n \r \t "));
  list.append (QLatin1String(" \\ / \" "));
  list.append (QLatin1String("http://foo.com"));

  QVariant expected (list);

  Parser driver;
  bool ok;
  QVariant result = driver.parse (json, &ok);
  QVERIFY (ok);
  QCOMPARE(result.toList().size(), expected.toList().size() );
  QCOMPARE(result, expected);
}

void TestParser::testNumbers() {
  QByteArray json = "[1,2.4, -100, -3.4, 1.00004, 01.01, -5e+, 2e,3e+,4.3E,5.4E-]";
  QVariantList list;
  list.append (QVariant(1));
  list.append (QVariant(2.4));
  list.append (QVariant(-100));
  list.append (QVariant(-3.4));
  list.append (QVariant(1.00004));
  list.append (QVariant(1.01));
  list.append (QLatin1String("-5e+"));
  list.append (QLatin1String("2e"));
  list.append (QLatin1String("3e+"));
  list.append (QLatin1String("4.3E"));
  list.append (QLatin1String("5.4E-"));
  QVariant expected (list);

  Parser driver;
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

void TestParser::testReadWriteEmptyDocument()
{
  QByteArray json = "";
  Parser driver;
  bool ok;
  QVariant result = driver.parse( json, &ok );
  QVERIFY(ok);
  Serializer serializer;
  const QByteArray serialized = serializer.serialize( result );
  QVERIFY( !serialized.isNull() );
  QVERIFY( serialized.isEmpty() );
}

void TestParser::testReadWrite()
{
  QFETCH( QByteArray, json );
  Parser driver;
  bool ok;
  QVariant result = driver.parse( json, &ok );
  QVERIFY(ok);
  Serializer serializer;
  const QByteArray serialized = serializer.serialize( result );
//  qWarning() << serialized;
  QVariant writtenThenRead = driver.parse( serialized, &ok );
  QVERIFY(ok);
  QCOMPARE( result, writtenThenRead );
}

void TestParser::testReadWrite_data()
{
    QTest::addColumn<QByteArray>( "json" );

    // array tests
    QTest::newRow( "empty array" ) << QByteArray("[]");
    QTest::newRow( "basic array" ) << QByteArray("[\"foo\",\"bar\"]");
    QTest::newRow( "single int array" ) << QByteArray("[6]");
    QTest::newRow( "int array" ) << QByteArray("[6,5,6,7]");
    const QByteArray json = "[1,2.4, -100, -3.4, -5e+, 2e,3e+,4.3E,5.4E-]";
    QTest::newRow( QByteArray("array of various numbers") ) << json;

    // document tests
    QTest::newRow( "empty object" ) << QByteArray("{}");
    QTest::newRow( "basic document" ) << QByteArray("{\"foo\":\"bar\"}");
    QTest::newRow( "object with ints" ) << QByteArray("{\"foo\":6}");
    const QByteArray json2 = "{ \"foo\":\"bar\",\n\"number\" : 51.3 , \"array\":[\"item1\", 123]}";
    QTest::newRow( "complicated document" ) << json2;

    // more complex cases
    const QByteArray json3 = "[ {\"foo\":\"bar\"},\n\"number\",51.3 , [\"item1\", 123]]";
    QTest::newRow( "complicated array" ) << json3;
}

QTEST_MAIN(TestParser)
#include "moc_testjsondriver.cxx"
