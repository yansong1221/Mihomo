#include "APIClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "Components/HttpClient.h"
#include "Configurator/ClashConfigurator.h"
#include <qcoro/network/qcoronetworkreply.h>
#include <qcoro/qcorotask.h>

namespace Clash::Meta::Core {

QCoro::Task<QJsonObject> APIClient::version() const
{
    return this->getOKJson("/version");
}
QCoro::Task<QJsonObject> APIClient::connections() const
{
    return this->getOKJson("/connections");
}
QCoro::Task<QJsonObject> APIClient::getOKJson(const QString &path) const
{
    auto reply = co_await get(path);
    if (reply->error() == QNetworkReply::NoError) {
        auto doc = QJsonDocument::fromJson(reply->readAll()).object();
        co_return doc;
    }
    co_return QJsonObject();
}

QCoro::Task<QJsonObject> APIClient::proxies() const
{
    return this->getOKJson("/proxies");
}

QCoro::Task<QNetworkReply *> APIClient::delay(const QString &proxy,
                                              const QString &testUrl,
                                              int timeout) const
{
    QUrlQuery query;
    query.addQueryItem("timeout", QString::number(timeout));
    query.addQueryItem("url", testUrl);
    return get(QString("/proxies/%1/delay").arg(QUrl::toPercentEncoding(proxy)), query);
}

QCoro::Task<QNetworkReply *> APIClient::get(const QString &path, const QUrlQuery &query) const
{
    const auto &controller = Config::ClashConfigurator::instance().controller;

    QUrl url;
    url.setHost(controller.http.host);
    url.setPort(controller.http.port);
    url.setScheme("http");
    url.setPath(path);
    url.setQuery(query);

    Components::HttpClient::Headers header;
    if (controller.secret)
        header["Authorization"] = QString("Bearer %1").arg(*controller.secret);

    return Components::HttpClient::instance().awaitGet(url, header);
}

} // namespace Clash::Meta::Core