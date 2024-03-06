#include "APIClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <qcoro/network/qcoronetworkreply.h>
#include <qcoro/qcorotask.h>

namespace Mihomo::Core {
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

void APIClient::init(const QUrl &apiUrl, const QString &authorization)
{
    apiUrl_ = apiUrl;
    authorization_ = authorization;
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
    QNetworkRequest request(apiUrl_.url() + path);
    if (!authorization_.isEmpty())
        request.setRawHeader("Authorization", QString("Bearer %1").arg(authorization_).toUtf8());

    auto reply = manager_->get(request);
    co_await reply;

    reply->deleteLater();

    if (reply->error() == QNetworkReply::NoError) {
        auto doc = QJsonDocument::fromJson(reply->readAll()).object();
        co_return doc;
    }
    co_return QJsonObject();
}

} // namespace mihomo::core