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

#ifndef QGEOCODEREPLY_H
#define QGEOCODEREPLY_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtLocation/QGeoLocation>

QT_BEGIN_NAMESPACE

class QGeoShape;
class QGeocodeReplyPrivate;

class Q_LOCATION_EXPORT QGeocodeReply : public QObject
{
    Q_OBJECT

public:
    enum Error {
        NoError,
        EngineNotSetError,
        CommunicationError,
        ParseError,
        UnsupportedOptionError,
        CombinationError,
        UnknownError
    };

    QGeocodeReply(Error error, const QString &errorString, QObject *parent = 0);
    virtual ~QGeocodeReply();

    bool isFinished() const;
    Error error() const;
    QString errorString() const;

    QGeoShape viewport() const;
    QList<QGeoLocation> locations() const;

    int limit() const;
    int offset() const;

    virtual void abort();

Q_SIGNALS:
    void finished();
    void error(QGeocodeReply::Error error, const QString &errorString = QString());

protected:
    QGeocodeReply(QObject *parent = 0);

    void setError(Error error, const QString &errorString);
    void setFinished(bool finished);

    void setViewport(const QGeoShape &viewport);
    void addLocation(const QGeoLocation &location);
    void setLocations(const QList<QGeoLocation> &locations);

    void setLimit(int limit);
    void setOffset(int offset);

private:
    QGeocodeReplyPrivate *d_ptr;
    Q_DISABLE_COPY(QGeocodeReply)
};

QT_END_NAMESPACE

#endif