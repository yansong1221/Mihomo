#include "ProxiesGroupModel.h"
#include "Core/APIClient.h"
#include <QJsonObject>
#include <QTimer>

namespace Clash::Meta::Connections {
ProxiesGroupModel::ProxiesGroupModel(QObject *parent /*= nullptr*/)
    : QAbstractListModel(parent)
{}

ProxiesGroupModel::~ProxiesGroupModel() {}

QCoro::Task<> ProxiesGroupModel::reload()
{
    if (isLoadding_)
        co_return;

    isLoadding_ = true;
    auto obj = co_await Core::APIClient::instance().proxies();
    isLoadding_ = false;

    if (obj.isEmpty()) {
        QTimer::singleShot(1000, this, &ProxiesGroupModel::reload);
        co_return;
    }


}

} // namespace Clash::Meta::Connections
