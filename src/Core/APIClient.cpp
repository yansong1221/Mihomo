#include "APIClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "Configurator/ClashConfigurator.h"
#include <qcoro/network/qcoronetworkreply.h>
#include <qcoro/qcorotask.h>

namespace Clash::Meta::Core {
APIClient::APIClient(QObject *parent)
    : QObject(parent)
{
    manager_ = new QNetworkAccessManager(this);
}
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

    QNetworkRequest request(url);
    if (controller.secret)
        request.setRawHeader("Authorization", QString("Bearer %1").arg(*controller.secret).toUtf8());

    auto reply = manager_->get(request);
    co_await reply;

    reply->deleteLater();

    if (reply->error() == QNetworkReply::NoError) {
        auto doc = QJsonDocument::fromJson(reply->readAll()).object();
        co_return doc;
    }
    co_return QJsonObject();
}

} // namespace Clash::Meta::Core