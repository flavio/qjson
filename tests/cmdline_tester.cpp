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
 
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include "parser.h"

using namespace QJson;

int main(int argc, char *argv[]) {
  QCoreApplication app (argc, argv);
  
  if (app.arguments().size() != 2) {
    qCritical("You have to specify the file containing the json code");
    exit (1);
  }
  
  QString filename = app.arguments()[1];
  if (!QFile::exists ( filename )) {
    qCritical ("The file you specified doesn't exist!");
    exit (1);
  }
  
  Parser driver;
  bool ok;

  QFile file (filename);
  QVariant data = driver.parse (&file, &ok);
  if (!ok) {
    qCritical("%s:%i - Error: %s", filename.toLatin1().data(), driver.errorLine(), qPrintable(driver.errorString()));
    exit (1);
  }
  else {
    qDebug() << "json object successfully converted to:";
    qDebug() << data;
  }

  qDebug() << "JOB DONE, BYE";
  return 0;
}

