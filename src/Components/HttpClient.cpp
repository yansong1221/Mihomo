#include "HttpClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <qcoro/network/qcoronetworkreply.h>

namespace Clash::Meta::Components {
HttpClient &HttpClient::instance()
{
    static HttpClient s_instance;
    return s_instance;
}

QCoro::Task<QNetworkReply *> HttpClient::awaitGet(const QUrl &url, const Headers &headers)
{
    QNetworkRequest request(url);
    std::for_each(headers.begin(), headers.end(), [&](const auto &iter) {
        request.setRawHeader(iter.first.toUtf8(), iter.second.toUtf8());
    });
    auto reply = QNetworkAccessManager::get(request);
    co_await reply;
    reply->deleteLater();
    co_return reply;
}

QCoro::Task<QNetworkReply *> HttpClient::awaitResquest(const QString &method,
                                                       const QUrl &url,
                                                       const QJsonObject &body,
                                                       const Headers &headers /*= {}*/)
{
    QNetworkRequest request(url);
    std::for_each(headers.begin(), headers.end(), [&](const auto &iter) {
        request.setRawHeader(iter.first.toUtf8(), iter.second.toUtf8());
    });
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    auto reply = QNetworkAccessManager::sendCustomRequest(request,
                                                          method.toUtf8(),
                                                          QJsonDocument(body).toJson());
    co_await reply;
    reply->deleteLater();
    co_return reply;
}

} // namespace Clash::Meta::Components
