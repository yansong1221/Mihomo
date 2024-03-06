#pragma once
#include <QAbstractListModel>

#include <qcoro/qcorotask.h>

class QTimer;

namespace Mihomo::Ui::Connections {

class ConnectionsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    struct Item
    {
        struct MetaData
        {
            QString network;
            QString type;
            QString sourceIP;
            QString destinationIP;
            QString sourcePort;
            QString destinationPort;
            QString host;
            QString dnsMode;
            QString processPath;
            QString specialProxy;
        };

        QString id;
        MetaData metadata;
        quint64 upload = 0;
        quint64 download = 0;
        QString start;
        QStringList chains;
        QString rule;
        QString rulePayload;
    };

    explicit ConnectionsModel(QObject *parent = nullptr);
    ~ConnectionsModel();

public:
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    inline quint64 downloadTotal() const { return downloadTotal_; }
    inline quint64 uploadTotal() const { return uploadTotal_; }

signals:
    void downloadTotalChanged(quint64 value);
    void uploadTotalChanged(quint64 value);
private slots:
    QCoro::Task<> __update();

private:
    void __update(const QList<Item> &newItems);

private:
    QTimer *updateTimer_;
    QList<Item> items_;

    quint64 downloadTotal_ = 0;
    quint64 uploadTotal_ = 0;
};

} // namespace Mihomo::Ui::Connections