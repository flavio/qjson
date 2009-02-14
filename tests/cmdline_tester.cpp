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
 
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QVariant>

#include <json_driver.hh>


int main(int argc, char *argv[]) {
  QCoreApplication app (argc, argv);
  
  if (app.arguments().size() != 2) {
    qFatal("You have to specify the file containing the json code");
    exit (1);
  }
  
  QString filename = app.arguments()[1];
  if (!QFile::exists ( filename )) {
    qFatal ("The file you specified doesn't exist!");
    exit (1);
  }
  
  JSonDriver driver;
  bool status;

  QVariant data = driver.parse (new QFile(filename), &status);
  if (status) {
    qFatal("An error occured during parsing");
    exit (1);
  }
  else {
    qDebug() << "json object successfully converted to:";
    qDebug() << data;
  }
  
  return 0;
}