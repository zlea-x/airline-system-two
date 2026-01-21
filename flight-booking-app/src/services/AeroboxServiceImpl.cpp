#include "services/AeroboxServiceImpl.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

AeroboxServiceImpl::AeroboxServiceImpl()
    : m_apiKey("api_key") // real api key is here
{
}

std::vector<Flight> AeroboxServiceImpl::fetchFlights(const QString& flightNumber,
                                                    const QString& date)
{
    std::vector<Flight> flights;

    // Correct URL pattern: number + date
    QUrl url(QString("https://aerodatabox.p.rapidapi.com/flights/number/%1/%2")
                 .arg(flightNumber, date));

    QUrlQuery query;
    query.addQueryItem("withAircraftImage", "false");
    query.addQueryItem("withLocation", "false");
    query.addQueryItem("withFlightPlan", "false");
    url.setQuery(query);

    qDebug() << "REQUEST URL:" << url.toString();

    QNetworkRequest request(url);
    request.setRawHeader("x-rapidapi-host", "aerodatabox.p.rapidapi.com");
    request.setRawHeader("x-rapidapi-key", m_apiKey.toUtf8());
    request.setRawHeader("Accept", "application/json");

    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply* reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Network error:" << reply->errorString();
        reply->deleteLater();
        return flights;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();

    if (!doc.isArray()) {
        qDebug() << "Unexpected JSON format (not array)";
        return flights;
    }

    QJsonArray arr = doc.array();
    int idCounter = 1;

    for (const QJsonValue& v : arr) {
        QJsonObject obj = v.toObject();

        QJsonObject dep = obj["departure"].toObject();
        QJsonObject arrv = obj["arrival"].toObject();

        QString depIcao = dep["airport"].toObject()["icao"].toString();
        QString arrIcao = arrv["airport"].toObject()["icao"].toString();

        QString depTime = dep["scheduledTime"].toObject()["local"].toString();
        QString arrTime = arrv["scheduledTime"].toObject()["local"].toString();

        flights.emplace_back(
            idCounter++,
            depIcao.toStdString(),
            arrIcao.toStdString(),
            depTime.toStdString(),
            arrTime.toStdString()
        );
    }

    return flights;
}

bool AeroboxServiceImpl::cancelRemoteBooking(int /*bookingId*/)
{
    // Not implemented; return true as a stub
    return true;
}
