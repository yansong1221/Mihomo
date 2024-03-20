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

            QList<GroupItemModel::ProxyInfo> proxyInfos;
            for (const auto &item : value["all"].toArray()) {
                auto proxyName = item.toString();
                auto proxyObj = findProxyObject(proxyName);

                GroupItemModel::ProxyInfo info;
                info.proxyName = item.toString();
                info.type = proxyObj["type"].toString();
                info.isUdp = proxyObj["udp"].toBool();

                for (const auto &item : proxyObj["history"].toArray())
                    info.delay = item.toObject()["delay"].toInt();

                proxyInfos << info;
            }

            auto groupItem = new GroupItemModel(this);
            groupItem->init(groupName == "GLOBAL" ? GroupItemModel::GlobalGroup
                                                  : GroupItemModel::RuleGroup,
                            groupName,
                            proxyInfos,
                            now);

            items_ << groupItem;
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
    this->beginResetModel();
    for (const auto &item : items_)
        item->deleteLater();
    items_.clear();

    parse(obj);
    this->endResetModel();
}

QModelIndex GroupModel::index(int row,
                              int column /*= 0*/,
                              const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column, items_[row]);
}

GroupModelFilter::GroupModelFilter(QObject *parent /*= nullptr*/)
    : QSortFilterProxyModel(parent)
{
    model_ = new GroupModel(this);
    setSourceModel(model_);
}

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
    auto index = model_->index(source_row, 0, source_parent);
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

} // namespace Clash::Meta::Proxies
