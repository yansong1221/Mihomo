#include "ProxiesGroupItemModel.h"
#include "Core/APIClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace Clash::Meta::Proxies {
GroupItemModel::GroupItemModel(QObject *parent /*= nullptr*/)
    : QAbstractListModel(parent)
{
    for (int i = 0; i < 30; ++i) {
        items_ << new ProxyItem(QString::number(i), "Test", this);
    }
}

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
    if (currentIndex_ == index)
        return;

    currentIndex_ = index;
    Q_EMIT currentIndexChanged(index);
}

//QHash<int, QByteArray> GroupItemModel::roleNames() const
//{
//    QHash<int, QByteArray> names;
//    names[Qt::DisplayRole] = "object";
//    return names;
//}

GroupItemModel::ProxyItem::ProxyItem(const QString &name,
                                     const QString &type,
                                     bool isUdp,
                                     QObject *parent /*= nullptr*/)
    : QObject(parent)
    , name_(name)
    , type_(type)
    , isUdp_(isUdp)
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
    auto reply = co_await client.delay(name_, "https://www.google.com", 5000);

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
    if (delay_ == val)
        return;

    delay_ = val;
    Q_EMIT delayChanged(delay_);
}

} // namespace Clash::Meta::Proxies
