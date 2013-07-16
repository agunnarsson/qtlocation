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
** This file is part of the Nokia services plugin for the Maps and
** Navigation API.  The use of these services, whether by use of the
** plugin or by other means, is governed by the terms and conditions
** described by the file NOKIA_TERMS_AND_CONDITIONS.txt in
** this package, located in the directory containing the Nokia services
** plugin source code.
**
****************************************************************************/

#include "qgeocodingmanagerengine_nokia.h"
#include "qgeocodereply_nokia.h"
#include "marclanguagecodes.h"
#include "qgeonetworkaccessmanager.h"
#include "qgeouriprovider.h"
#include "uri_constants.h"

#include <qgeoaddress.h>
#include <qgeocoordinate.h>
#include <QUrl>
#include <QMap>
#include <QStringList>

QT_BEGIN_NAMESPACE

QGeocodingManagerEngineNokia::QGeocodingManagerEngineNokia(
        QGeoNetworkAccessManager *networkManager,
        const QMap<QString, QVariant> &parameters,
        QGeoServiceProvider::Error *error,
        QString *errorString)
        : QGeocodingManagerEngine(parameters)
        , m_networkManager(networkManager)
        , m_uriProvider(new QGeoUriProvider(this, parameters, "geocoding.host", GEOCODING_HOST, GEOCODING_HOST_CN))
{
    Q_ASSERT(networkManager);
    m_networkManager->setParent(this);

    if (parameters.contains("token")) {
        m_token = parameters.value("token").toString();
    }

    if (parameters.contains("app_id")) {
        m_applicationId = parameters.value("app_id").toString();
    }

    if (error)
        *error = QGeoServiceProvider::NoError;

    if (errorString)
        *errorString = "";
}

QGeocodingManagerEngineNokia::~QGeocodingManagerEngineNokia() {}

QString QGeocodingManagerEngineNokia::getAuthenticationString() const
{
    QString authenticationString;

    if (!m_token.isEmpty() && !m_applicationId.isEmpty()) {
        authenticationString += "?token=";
        authenticationString += m_token;

        authenticationString += "&app_id=";
        authenticationString += m_applicationId;
    }

    return authenticationString;
}


QGeocodeReply *QGeocodingManagerEngineNokia::geocode(const QGeoAddress &address,
        const QGeoShape &bounds)
{
    QString requestString = "http://";
    requestString += m_uriProvider->getCurrentHost();
    requestString += "/geocoder/gc/2.0";

    requestString += getAuthenticationString();

    requestString += "&lg=";
    requestString += languageToMarc(locale().language());

    if (address.country().isEmpty()) {
        QStringList parts;

        if (!address.state().isEmpty())
            parts << address.state();

        if (!address.city().isEmpty())
            parts << address.city();

        if (!address.postalCode().isEmpty())
            parts << address.postalCode();

        if (!address.street().isEmpty())
            parts << address.street();

        requestString += "&obloc=";
        requestString += parts.join(" ");
    } else {
        requestString += "&country=";
        requestString += address.country();

        if (!address.state().isEmpty()) {
            requestString += "&state=";
            requestString += address.state();
        }

        if (!address.city().isEmpty()) {
            requestString += "&city=";
            requestString += address.city();
        }

        if (!address.postalCode().isEmpty()) {
            requestString += "&zip=";
            requestString += address.postalCode();
        }

        if (!address.street().isEmpty()) {
            requestString += "&street=";
            requestString += address.street();
        }
    }


    // TODO?
    // street number has been removed from QGeoAddress
    // do we need to try to split it out from QGeoAddress::street
    // in order to geocode properly

    // Old code:
//    if (!address.streetNumber().isEmpty()) {
//        requestString += "&number=";
//        requestString += address.streetNumber();
//    }

    return geocode(requestString, bounds);
}

QGeocodeReply *QGeocodingManagerEngineNokia::reverseGeocode(const QGeoCoordinate &coordinate,
        const QGeoShape &bounds)
{
    QString requestString = "http://";
    requestString += m_uriProvider->getCurrentHost();
    requestString += "/geocoder/rgc/2.0";

    requestString += getAuthenticationString();

    requestString += "&long=";
    requestString += trimDouble(coordinate.longitude());
    requestString += "&lat=";
    requestString += trimDouble(coordinate.latitude());

    requestString += "&lg=";
    requestString += languageToMarc(locale().language());

    return geocode(requestString, bounds);
}

QGeocodeReply *QGeocodingManagerEngineNokia::geocode(const QString &address,
        int limit,
        int offset,
        const QGeoShape &bounds)
{
    QString requestString = "http://";
    requestString += m_uriProvider->getCurrentHost();
    requestString += "/geocoder/gc/2.0";

    requestString += getAuthenticationString();

    requestString += "&lg=";
    requestString += languageToMarc(locale().language());

    requestString += "&obloc=";
    requestString += address;

    if (limit > 0) {
        requestString += "&total=";
        requestString += QString::number(limit);
    }

    if (offset > 0) {
        requestString += "&offset=";
        requestString += QString::number(offset);
    }

    return geocode(requestString, bounds, limit, offset);
}

QGeocodeReply *QGeocodingManagerEngineNokia::geocode(QString requestString,
        const QGeoShape &bounds,
        int limit,
        int offset)
{
    QNetworkReply *networkReply = m_networkManager->get(QNetworkRequest(QUrl(requestString)));
    QGeocodeReplyNokia *reply = new QGeocodeReplyNokia(networkReply, limit, offset, bounds, this);

    connect(reply,
            SIGNAL(finished()),
            this,
            SLOT(placesFinished()));

    connect(reply,
            SIGNAL(error(QGeocodeReply::Error, QString)),
            this,
            SLOT(placesError(QGeocodeReply::Error, QString)));

    return reply;
}

QString QGeocodingManagerEngineNokia::trimDouble(double degree, int decimalDigits)
{
    QString sDegree = QString::number(degree, 'g', decimalDigits);

    int index = sDegree.indexOf('.');

    if (index == -1)
        return sDegree;
    else
        return QString::number(degree, 'g', decimalDigits + index);
}

void QGeocodingManagerEngineNokia::placesFinished()
{
    QGeocodeReply *reply = qobject_cast<QGeocodeReply *>(sender());

    if (!reply)
        return;

    if (receivers(SIGNAL(finished(QGeocodeReply*))) == 0) {
        reply->deleteLater();
        return;
    }

    emit finished(reply);
}

void QGeocodingManagerEngineNokia::placesError(QGeocodeReply::Error error, const QString &errorString)
{
    QGeocodeReply *reply = qobject_cast<QGeocodeReply *>(sender());

    if (!reply)
        return;

    if (receivers(SIGNAL(error(QGeocodeReply *, QGeocodeReply::Error, QString))) == 0) {
        reply->deleteLater();
        return;
    }

    emit this->error(reply, error, errorString);
}

QString QGeocodingManagerEngineNokia::languageToMarc(QLocale::Language language)
{
    uint offset = 3 * (uint(language));
    if (language == QLocale::C || offset + 3 > sizeof(marc_language_code_list))
        return QLatin1String("eng");

    const unsigned char *c = marc_language_code_list + offset;
    if (c[0] == 0)
        return QLatin1String("eng");

    QString code(3, Qt::Uninitialized);
    code[0] = ushort(c[0]);
    code[1] = ushort(c[1]);
    code[2] = ushort(c[2]);

    return code;
}

QT_END_NAMESPACE
