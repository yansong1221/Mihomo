#pragma once
#include <optional>
#include <QObject>
#include <QUrl>
#include <QUrlQuery>

#include <qcoro/qcorotask.h>

class QNetworkAccessManager;
class QNetworkReply;

namespace Clash::Meta::Core {
class APIClient
{
    using Params = std::map<QString, QString>;

public:
    QCoro::Task<QJsonObject> version() const;
    QCoro::Task<QJsonObject> connections() const;
    QCoro::Task<QJsonObject> proxies() const;

    QCoro::Task<QNetworkReply *> delay(const QString &proxy,
                                       const QString &testUrl,
                                       int timeout) const;
    QCoro::Task<QNetworkReply *> slectProxies(const QString &proxy, const QString &name);
    QCoro::Task<QNetworkReply *> changeProxyMode(const QString &mode);
    QCoro::Task<QNetworkReply *> configs();

private:
    QCoro::Task<QJsonObject> getOKJson(const QString &path) const;

    QCoro::Task<QNetworkReply *> get(const QString &path,
                                     const QUrlQuery &query = QUrlQuery()) const;

    QCoro::Task<QNetworkReply *> sendJsonData(const QString &method,
                                              const QString &path,
                                              const QJsonObject &body) const;

private:
};
} // namespace Clash::Meta::Core