#include "ProxiesGroupItemModel.h"
#include "Core/APIClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace Clash::Meta::Proxies {
GroupItemModel::GroupItemModel(GroupType groupType,
                               const QString &groupName,
                               QObject *parent /*= nullptr*/)
    : QAbstractListModel(parent)
    , groupType_(groupType)
    , groupName_(groupName)
{}

GroupItemModel::~GroupItemModel() {}

QModelIndex GroupItemModel::index(int row,
                                  int column,
                                  const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column, &items_[row]);
}

int GroupItemModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return items_.length();
}
QVariant GroupItemModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    const auto &item = items_[index.row()];
    switch (role) {
    case ProxyDelayRole:
        return item.delay;
    case ProxyNameRole:
        return item.proxyName;
    case ProxyTypeRole:
        return item.type;
    case ProxyUdpRole:
        return item.isUdp;
    case ProxyDelayDetectingRole:
        return item.delayDetecting;
    default:
        break;
    }
    return QVariant();
}

void GroupItemModel::setCurrentIndex(int row)
{
    __setCurrentIndex(row);
}

QCoro::Task<> GroupItemModel::__setCurrentIndex(int row)
{
    if (currentIndex_ == row)
        co_return;

    auto &item = items_[row];

    Core::APIClient client;
    auto reply = co_await client.slectProxies(groupName_, item.proxyName);
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        co_return;
    }
    currentIndex_ = row;

    Q_EMIT currentIndexChanged(currentIndex_);
    Q_EMIT currentNameChanged(currentName());
}

QString GroupItemModel::currentName() const
{
    if (currentIndex_ == -1)
        return QString();

    return items_[currentIndex_].proxyName;
}

bool GroupItemModel::removeRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/)
{
    if (count < 1)
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    items_.remove(row, count);
    endRemoveRows();
    return true;
}

void GroupItemModel::clear()
{
    removeRows(0, rowCount());
}

QHash<int, QByteArray> GroupItemModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ProxyDelayRole] = "delayRole";
    roles[ProxyNameRole] = "nameRole";
    roles[ProxyTypeRole] = "typeRole";
    roles[ProxyUdpRole] = "udpRole";
    roles[ProxyDelayDetectingRole] = "delayDetectingRole";
    return roles;
}

void GroupItemModel::delayDetect(int row)
{
    __delayDetect(row);
}

QCoro::Task<> GroupItemModel::__delayDetect(int row)
{
    auto &item = items_[row];
    if (item.delayDetecting)
        co_return;

    item.delayDetecting = true;
    Q_EMIT dataChanged(index(row), index(row), {ProxyDelayDetectingRole});

    Core::APIClient client;
    auto reply = co_await client.delay(item.proxyName, "https://www.google.com", 5000);

    item.delayDetecting = false;
    Q_EMIT dataChanged(index(row), index(row), {ProxyDelayDetectingRole});

    if (reply->error() == QNetworkReply::NoError) {
        auto obj = QJsonDocument::fromJson(reply->readAll()).object();
        auto delay = obj["delay"].toInt();
        __setDelay(row, delay);
    } else
        __setDelay(row, -1);
}

void GroupItemModel::__setDelay(int row, int val)
{
    auto &item = items_[row];
    if (item.delay == val)
        return;

    item.delay = val;
    Q_EMIT dataChanged(index(row), index(row), {ProxyDelayRole});
}

void GroupItemModel::append(const QString &proxyName, const QString &type, bool isUdp, int delay)
{
    auto row = items_.length();
    this->beginInsertRows(QModelIndex(), row, row);
    ProxyInfo info;
    info.proxyName = proxyName;
    info.type = type;
    info.isUdp = isUdp;
    info.delay = delay;
    items_ << info;
    this->endInsertRows();
}

void GroupItemModel::setCurrentProxyName(const QString &currentProxyName)
{
    for (int i = 0; i < items_.length(); ++i) {
        const auto &info = items_[i];
        if (info.proxyName == currentProxyName) {
            currentIndex_ = i;
            Q_EMIT currentIndexChanged(currentIndex_);
            Q_EMIT currentNameChanged(currentName());
        }
    }
}

} // namespace Clash::Meta::Proxies
