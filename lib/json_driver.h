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

#ifndef _JSON_DRIVER
#define _JSON_DRIVER

#include "qjson_export.h"

#include <QtCore/QIODevice>
#include <QtCore/QVariant>

class JSonDriverPrivate;

/**
* @brief Main class used for converting JSON data to QVariant objects
*/
class QJSON_EXPORT JSonDriver
{
  public:
    JSonDriver();
    virtual ~JSonDriver();

    /**
    * Read JSON string from the I/O Device and converts it to a QVariant object
    * @param io Input output device
    * @param ok if a conversion error occurs, *ok is set to false; otherwise *ok is set to true.
    * @returns a QVariant object generated from the JSON string
    */
    QVariant parse(QIODevice* io, bool* ok = 0);

    /**
    * This is a method provided for convenience.
    * @param jsonData data containing the JSON object representation
    * @param ok if a conversion error occurs, *ok is set to false; otherwise *ok is set to true.
    * @returns a QVariant object generated from the JSON string
    * /sa error
    */
    QVariant parse(const QByteArray& jsonData, bool* ok = 0);

    /**
    * This method returns the error message
    * @returns a QString object containing the error message of the last parse operation
    */
    QString error() const;

    /**
    * This method returns line number where the error occurred
    * @returns the line number where the error occurred
    */
    int errorLine() const;
    
  private:
    Q_DISABLE_COPY(JSonDriver)

    JSonDriverPrivate *d;
};

#endif
