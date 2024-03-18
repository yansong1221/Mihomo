#pragma once
#include <QNetworkAccessManager>
#include <qcoro/qcorotask.h>

namespace Clash::Meta::Components {
class HttpClient : public QNetworkAccessManager
{
    Q_OBJECT
public:
    using QNetworkAccessManager::QNetworkAccessManager;

    static HttpClient &instance();

    using Headers = std::map<QString, QString>;

public:
    QCoro::Task<QNetworkReply *> awaitGet(const QUrl &url, const Headers &headers = {});
    QCoro::Task<QNetworkReply *> awaitResquest(const QString &method,
                                               const QUrl &url,
                                               const QJsonObject &body,
                                               const Headers &headers = {});
};
} // namespace Clash::Meta::Components