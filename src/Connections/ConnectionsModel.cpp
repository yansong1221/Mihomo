#include "ConnectionsModel.h"
#include "core/APIClient.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>

namespace Mihomo::Ui::Connections {

inline static bool operator==(const ConnectionsModel::Item &left,
                              const ConnectionsModel::Item &right)
{
    return left.id == right.id;
}
inline static bool operator==(const ConnectionsModel::Item &item, const QString &id)
{
    return item.id == id;
}

ConnectionsModel::ConnectionsModel(QObject *parent /*= nullptr*/)
    : QAbstractListModel(parent)
{
    Core::APIClient::instance().init(QString("http://127.0.0.1:1172"),
                                     "93eb58af-38e1-427e-b6a8-1a8863dd1f25");

    updateTimer_ = new QTimer(this);
    connect(updateTimer_, SIGNAL(timeout()), this, SLOT(__update()));
    updateTimer_->setInterval(1000);
    updateTimer_->start();
}

ConnectionsModel::~ConnectionsModel() {}

QCoro::Task<> ConnectionsModel::__update()
{
    updateTimer_->stop();
    auto data = co_await Core::APIClient::instance().connections();
    if (!data.isEmpty()) {
        auto _downloadTotal = data["downloadTotal"].toInteger();
        if (_downloadTotal != downloadTotal_) {
            downloadTotal_ = _downloadTotal;
            Q_EMIT downloadTotalChanged(downloadTotal_);
        }
        auto _uploadTotal = data["uploadTotal"].toInteger();
        if (_uploadTotal != uploadTotal_) {
            uploadTotal_ = _uploadTotal;
            Q_EMIT uploadTotalChanged(uploadTotal_);
        }

        auto connections = data["connections"].toArray();

        QList<Item> newItems;
        for (const auto &conn : connections) {
            auto obj = conn.toObject();

            Item item;

            item.id = obj["id"].toString();
            auto metadata = obj["metadata"].toObject();
            {
                item.metadata.network = metadata["network"].toString();
                item.metadata.type = metadata["type"].toString();
                item.metadata.sourceIP = metadata["sourceIP"].toString();
                item.metadata.destinationIP = metadata["destinationIP"].toString();
                item.metadata.sourcePort = metadata["sourcePort"].toString();
                item.metadata.destinationPort = metadata["destinationPort"].toString();
                item.metadata.host = metadata["host"].toString();
                item.metadata.dnsMode = metadata["dnsMode"].toString();
                item.metadata.processPath = metadata["processPath"].toString();
                item.metadata.specialProxy = metadata["specialProxy"].toString();
            }
            item.upload = obj["upload"].toInteger();
            item.download = obj["download"].toInteger();
            item.start = obj["start"].toString();
            auto chains = obj["chains"].toArray();

            for (const auto &chain : chains)
                item.chains << chain.toString();

            item.rule = obj["rule"].toString();
            item.rulePayload = obj["rulePayload"].toString();

            newItems << item;
        }
        __update(newItems);
    }
    updateTimer_->start();
}

void ConnectionsModel::__update(const QList<Item> &newItems)
{
    //先删除新的中不存在的
    int row = 0;
    while (row < items_.length()) {
        if (!newItems.contains(items_[row])) {
            this->beginRemoveRows(QModelIndex(), row, row);
            items_.removeAt(row);
            this->endRemoveRows();
            continue;
        }
        row++;
    }
    for (const auto &newItem : newItems) {
        auto row = items_.indexOf(newItem);
        //之前就存在
        if (row != -1) {
            auto oldItem = items_[row];
            items_[row] = newItem;

            if (oldItem.download != newItem.download || oldItem.upload != newItem.upload) {
                auto _index = this->index(row);
                Q_EMIT dataChanged(_index, _index);
            }
        }
        //原来没有
        else {
            auto row = items_.length();
            this->beginInsertRows(QModelIndex(), row, row);
            items_ << newItem;
            this->endInsertRows();
        }
    }
}

QVariant ConnectionsModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    return QVariant();
}

int ConnectionsModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return items_.length();
}

} // namespace Mihomo::Ui::Connections
