#pragma once
#include <optional>
#include <QObject>
#include <QUrl>

#include <qcoro/qcorotask.h>

class QNetworkAccessManager;
class QNetworkReply;

namespace Clash::Meta::Core {
class APIClient : public QObject
{
    Q_OBJECT
public:
    explicit APIClient(QObject *parent = nullptr);

public:

    QCoro::Task<QJsonObject> version() const;
    QCoro::Task<QJsonObject> connections() const;
    QCoro::Task<QJsonObject> proxies() const;

    static APIClient &instance();

private:
    QCoro::Task<QJsonObject> get(const QString &path) const;

private:
};
} // namespace Mihomo::Core