/* This file is part of qjson
  *
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
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

#ifndef JSONCONVERTERTHREAD_H
#define JSONCONVERTERTHREAD_H

#include <QtCore/QThread>
#include <QtCore/QVariant>

class JSonConverterThreadPrivate;

/**
* @brief Convenience class for converting JSON data to QVariant objects using a dedicated thread
*/
class JSonConverterThread  : public QThread
{
  Q_OBJECT
  public:
    /**
    * This signal is emitted when the conversion process has been completed
    * @param data contains the JSON data that has to be converted
    * @param parent thread's parent
    **/
    explicit JSonConverterThread(QString& data, QObject* parent = 0);

    void run();

  Q_SIGNALS:
    /**
    * This signal is emitted when the conversion process has been completed
    * @param json contains the result of the conversion
    * @param status if a conversion error occurs status is set to false, otherwise is set to true.
    * @param error_msg contains a string explaining the failure reason
    **/
    void conversionFinished(const QVariant& json, bool status, const QString& error_msg);

  private:
    Q_DISABLE_COPY(JSonConverterThread)

    JSonConverterThreadPrivate *d;
};

#endif // JSONCONVERTERTHREAD_H
