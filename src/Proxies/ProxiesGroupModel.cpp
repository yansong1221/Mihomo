#include "ProxiesGroupModel.h"
#include "Core/APIClient.h"
#include "ProxiesGroupItemModel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QTimer>

namespace Clash::Meta::Proxies {

GroupModel::GroupModel(QObject *parent /*= nullptr*/)
    : QAbstractListModel(parent)
{}

GroupModel::~GroupModel() {}

void GroupModel::reload()
{
    __reload();
}
void GroupModel::parse(const QJsonObject &obj)
{
    auto proxies = obj["proxies"].toObject();
    if (proxies.isEmpty())
        return;

    auto findProxyObject = [&](const QString &key) { return proxies[key].toObject(); };

    for (auto iter = proxies.constBegin(); iter != proxies.constEnd(); ++iter) {
        auto key = iter.key();
        auto value = iter.value().toObject();

        auto type = value["type"].toString();

        if (type == "Selector" || type == "URLTest" || type == "Fallback") {
            auto udp = value["udp"].toBool();
            auto now = value["now"].toString();
            auto groupName = value["name"].toString();

            auto groupItem = new GroupItemModel(groupName == "GLOBAL" ? GroupItemModel::GlobalGroup
                                                                      : GroupItemModel::RuleGroup,
                                                groupName,
                                                this);

            for (const auto &item : value["all"].toArray()) {
                auto proxyName = item.toString();
                auto proxyObj = findProxyObject(proxyName);

                int delay = -2;
                for (const auto &item : proxyObj["history"].toArray())
                    delay = item.toObject()["delay"].toInt();

                groupItem->append(proxyName,
                                  proxyObj["type"].toString(),
                                  proxyObj["udp"].toBool(),
                                  delay);
            }
            groupItem->setCurrentProxyName(now);

            auto row = items_.length();
            this->beginInsertRows(QModelIndex(), row, row);
            items_ << groupItem;
            this->endInsertRows();
        }
    }
}

int GroupModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return items_.length();
}

QVariant GroupModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(items_[index.row()]);
    }
    return QVariant();
}

QCoro::Task<> GroupModel::__reload()
{
    if (isLoadding_)
        co_return;
    isLoadding_ = true;
    Core::APIClient client;
    auto obj = co_await client.proxies();
    isLoadding_ = false;

    if (obj.isEmpty()) {
        QTimer::singleShot(1000, this, &GroupModel::reload);
        co_return;
    }
    this->clear();
    parse(obj);

    Q_EMIT reloadSuccessed();
}

QModelIndex GroupModel::index(int row,
                              int column /*= 0*/,
                              const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column, items_[row]);
}

bool GroupModel::removeRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/)
{
    if (count < 1)
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    for (const auto &item : items_)
        item->deleteLater();
    items_.remove(row, count);
    endRemoveRows();
    return true;
}

void GroupModel::clear()
{
    removeRows(0, rowCount());
}

GroupModelFilter::GroupModelFilter(QObject *parent /*= nullptr*/)
    : QSortFilterProxyModel(parent)
{}

void GroupModelFilter::setFilerMode(const QString &mode)
{
    if (filerMode_ == mode)
        return;

    filerMode_ = mode;
    Q_EMIT filerModeChanged(filerMode_);
    invalidateRowsFilter();
}

bool GroupModelFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    auto index = sourceModel()->index(source_row, 0, source_parent);
    auto item = static_cast<GroupItemModel *>(index.internalPointer());

    if (filerMode_.compare("global", Qt::CaseInsensitive) == 0) {
        if (item->groupType() == GroupItemModel::GlobalGroup)
            return true;
    } else if (filerMode_.compare("rule", Qt::CaseInsensitive) == 0) {
        if (item->groupType() != GroupItemModel::GlobalGroup)
            return true;
    }
    return false;
}

//void GroupModelFilter::setSourceModel(QAbstractItemModel *sourceModel)
//{
//    QSortFilterProxyModel::setSourceModel(sourceModel);
//}

} // namespace Clash::Meta::Proxies
