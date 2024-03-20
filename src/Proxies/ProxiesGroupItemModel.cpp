#include "ProxiesGroupItemModel.h"
#include "Core/APIClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace Clash::Meta::Proxies {
GroupItemModel::GroupItemModel(QObject *parent /*= nullptr*/)
    : QAbstractListModel(parent)
{}

GroupItemModel::~GroupItemModel() {}

QModelIndex GroupItemModel::index(int row,
                                  int column,
                                  const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column, items_[row]);
}

int GroupItemModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return items_.length();
}
QVariant GroupItemModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(items_[index.row()]);
    }
    return QVariant();
}

void GroupItemModel::setCurrentIndex(int index)
{
    __setCurrentIndex(index);
}

QCoro::Task<> GroupItemModel::__setCurrentIndex(int index)
{
    if (currentIndex_ == index)
        co_return;

    auto modelIndex = this->index(index, 0);
    auto item = static_cast<ProxyItem *>(modelIndex.internalPointer());

    Core::APIClient client;
    auto reply = co_await client.slectProxies(groupName_, item->name());
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        co_return;
    }
    qDebug() << reply->readAll();

    currentIndex_ = index;
    Q_EMIT currentIndexChanged(index);
    Q_EMIT currentNameChanged(currentName());
}

QString GroupItemModel::currentName() const
{
    if (currentIndex_ == -1)
        return QString();

    auto modelIndex = this->index(currentIndex_, 0);
    auto item = static_cast<ProxyItem *>(modelIndex.internalPointer());
    return item->name();
}

void GroupItemModel::init(GroupType groupType,
                          const QString &groupName,
                          const QList<ProxyInfo> &proxyInfos,
                          const QString &currentProxyName)
{
    this->clear();

    groupType_ = groupType;
    groupName_ = groupName;
    for (int i = 0; i < proxyInfos.length(); ++i) {
        const auto &info = proxyInfos[i];

        this->beginInsertRows(QModelIndex(), i, i);
        items_ << new ProxyItem(info, this);
        this->endInsertRows();

        if (info.proxyName == currentProxyName) {
            currentIndex_ = i;
            Q_EMIT currentIndexChanged(currentIndex_);
            Q_EMIT currentNameChanged(currentName());
        }
    }
}

bool GroupItemModel::removeRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/)
{
    if (count < 1)
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = row; i < row + count; ++i)
        items_[i]->deleteLater();
    items_.remove(row, count);
    endRemoveRows();
    return true;
}

void GroupItemModel::clear()
{
    removeRows(0, rowCount());
}

void GroupItemModel::setExpand(bool flag)
{
    if (isExpand_ == flag)
        return;

    isExpand_ = flag;
    Q_EMIT expandChanged(isExpand_);
}

GroupItemModel::ProxyItem::ProxyItem(const ProxyInfo &info, QObject *parent /*= nullptr*/)
    : QObject(parent)
    , info_(info)
{}

GroupItemModel::ProxyItem::~ProxyItem() {}

void GroupItemModel::ProxyItem::delayDetect()
{
    __delayDetect();
}

QCoro::Task<> GroupItemModel::ProxyItem::__delayDetect()
{
    if (delayDetecting_)
        co_return;

    delayDetecting_ = true;
    Q_EMIT delayDetectingChanged(true);

    Core::APIClient client;
    auto reply = co_await client.delay(info_.proxyName, "https://www.google.com", 5000);

    delayDetecting_ = false;
    Q_EMIT delayDetectingChanged(false);

    if (reply->error() == QNetworkReply::NoError) {
        auto obj = QJsonDocument::fromJson(reply->readAll()).object();
        auto delay = obj["delay"].toInt();
        setDelay(delay);
    } else
        setDelay(-1);
}

void GroupItemModel::ProxyItem::setDelay(int val)
{
    if (info_.delay == val)
        return;

    info_.delay = val;
    Q_EMIT delayChanged(val);
}

} // namespace Clash::Meta::Proxies
