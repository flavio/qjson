/* This file is part of QJson
 *
 * Copyright (C) 2009 Flavio Castelli <flavio.castelli@gmail.com>
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

class TestSerializer: public QObject
{
  Q_OBJECT
  private slots:
    void testReadWriteEmptyDocument();
    void testReadWrite();
    void testReadWrite_data();
};

Q_DECLARE_METATYPE(QVariant)

using namespace QJson;

void TestSerializer::testReadWriteEmptyDocument()
{
  QByteArray json = "";
  Parser parser;
  bool ok;
  QVariant result = parser.parse( json, &ok );
  QVERIFY(ok);
  Serializer serializer;
  const QByteArray serialized = serializer.serialize( result );
  QVERIFY( !serialized.isNull() );
  QVERIFY( serialized.isEmpty() );
}

void TestSerializer::testReadWrite()
{
  QFETCH( QByteArray, json );
  Parser parser;
  bool ok;
  QVariant result = parser.parse( json, &ok );
  QVERIFY(ok);
  Serializer serializer;
  const QByteArray serialized = serializer.serialize( result );
//  qWarning() << serialized;
  QVariant writtenThenRead = parser.parse( serialized, &ok );
  QVERIFY(ok);
  QCOMPARE( result, writtenThenRead );
}

void TestSerializer::testReadWrite_data()
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

QTEST_MAIN(TestSerializer)
#include "moc_testserializer.cxx"
