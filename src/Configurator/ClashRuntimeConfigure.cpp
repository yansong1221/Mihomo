#include "ClashRuntimeConfigure.h"
#include "Core/APIClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace Clash::Meta::Config {
ClashRuntimeConfigure::ClashRuntimeConfigure(QObject *parent /*= nullptr*/)
    : QObject(parent)
{
    __refreshConfigure();
}

ClashRuntimeConfigure::~ClashRuntimeConfigure() {}

QCoro::Task<> ClashRuntimeConfigure::__setMode(const QString &mode)
{
    if (mode_ == mode)
        co_return;

    Core::APIClient client;
    auto reply = co_await client.changeProxyMode(mode);
    if (reply->error() != QNetworkReply::NoError)
        co_return;

    __refreshConfigure();
}

QCoro::Task<> ClashRuntimeConfigure::__refreshConfigure()
{
    Core::APIClient client;
    auto reply = co_await client.configs();
    if (reply->error() != QNetworkReply::NoError)
        co_return;

    auto data = QJsonDocument::fromJson(reply->readAll()).object();

    auto mode = data["mode"].toString();
    if (mode_ != mode) {
        mode_ = mode;
        Q_EMIT modeChanged(mode_);
    }
}

ClashRuntimeConfigure &ClashRuntimeConfigure::instance()
{
    static ClashRuntimeConfigure s_instance;
    return s_instance;
}

} // namespace Clash::Meta::Config
