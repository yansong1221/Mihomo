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
APIClient::APIClient(QObject *parent)
    : QObject(parent)
{}
APIClient &APIClient::instance()
{
    static APIClient s_instance;
    return s_instance;
}

QCoro::Task<QJsonObject> APIClient::version() const
{
    return this->get("/version");
}
QCoro::Task<QJsonObject> APIClient::connections() const
{
    return this->get("/connections");
}
QCoro::Task<QJsonObject> APIClient::get(const QString &path) const
{
    const auto &controller = Config::ClashConfigurator::instance().controller;

    QUrl url;
    url.setHost(controller.http.host);
    url.setPort(controller.http.port);
    url.setScheme("http");
    url.setPath(path);

    Components::HttpClient::Headers header;
    if (controller.secret)
        header["Authorization"] = QString("Bearer %1").arg(*controller.secret);

    auto reply = co_await Components::HttpClient::instance().awaitGet(url, header);

    if (reply->error() == QNetworkReply::NoError) {
        auto doc = QJsonDocument::fromJson(reply->readAll()).object();
        co_return doc;
    }
    co_return QJsonObject();
}

QCoro::Task<QJsonObject> APIClient::proxies() const 
{
    return this->get("/proxies");
}

} // namespace Clash::Meta::Core