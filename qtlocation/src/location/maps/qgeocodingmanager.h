/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGEOCODINGMANAGER_H
#define QGEOCODINGMANAGER_H

#include <QtLocation/QGeocodeReply>
#include <QtLocation/QGeoRectangle>

#include <QObject>
#include <QList>
#include <QMap>

QT_BEGIN_NAMESPACE

class QLocale;

class QGeocodingManagerEngine;
class QGeocodingManagerPrivate;

class Q_LOCATION_EXPORT QGeocodingManager : public QObject
{
    Q_OBJECT
public:
    ~QGeocodingManager();

    QString managerName() const;
    int managerVersion() const;

    QGeocodeReply *geocode(const QGeoAddress &address,
                           const QGeoShape &bounds = QGeoShape());
    QGeocodeReply *geocode(const QString &searchString,
                            int limit = -1,
                            int offset = 0,
                           const QGeoShape &bounds = QGeoShape());

    QGeocodeReply *reverseGeocode(const QGeoCoordinate &coordinate,
                                  const QGeoShape &bounds = QGeoShape());

    void setLocale(const QLocale &locale);
    QLocale locale() const;

Q_SIGNALS:
    void finished(QGeocodeReply *reply);
    void error(QGeocodeReply *reply, QGeocodeReply::Error error, QString errorString = QString());

private:
    QGeocodingManager(QGeocodingManagerEngine *engine, QObject *parent = 0);

    QGeocodingManagerPrivate *d_ptr;
    Q_DISABLE_COPY(QGeocodingManager)

    friend class QGeoServiceProvider;
    friend class QGeoServiceProviderPrivate;
};

QT_END_NAMESPACE

#endif