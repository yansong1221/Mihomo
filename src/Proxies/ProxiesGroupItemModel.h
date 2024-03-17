#pragma once
#include <QAbstractListModel>
#include <qcoro/qcorotask.h>

namespace Clash::Meta::Proxies {
class GroupItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    class ProxyItem;

    explicit GroupItemModel(QObject *parent = nullptr);
    ~GroupItemModel();

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
public:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    //virtual QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int currentIndex() const { return currentIndex_; }
    void setCurrentIndex(int index);

signals:
    void currentIndexChanged(int index);

private:
    QList<ProxyItem *> items_;
    int currentIndex_ = -1;
};

class GroupItemModel::ProxyItem : public QObject
{
    Q_OBJECT
public:
    explicit ProxyItem(const QString &proxyName,
                       const QString &type,
                       bool isUdp,
                       QObject *parent = nullptr);
    ~ProxyItem();

    Q_PROPERTY(bool delayDetecting READ isDelayDetecting NOTIFY delayDetectingChanged)
    Q_PROPERTY(int delay READ delay NOTIFY delayChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(bool udp READ isUdp)

public:
    int delay() const { return delay_; }
    void setDelay(int val);
    QString name() const { return name_; }
    QString type() const { return type_; }
    bool isUdp() const { return isUdp_; }

    bool isDelayDetecting() const { return delayDetecting_; }
signals:
    void nameChanged(const QString &name);
    void delayChanged(int val);
    void delayDetectingChanged(bool detecting);

public slots:
    void delayDetect();

private:
    QCoro::Task<> __delayDetect();

private:
    bool delayDetecting_ = false;

    QString name_;
    QString type_;
    bool isUdp_ = false;
    int delay_ = -2; //-2 未检测过 -1超时 大于等于0 具体延迟ms
};

} // namespace Clash::Meta::Proxies