#pragma once
#include <QAbstractListModel>
#include <qcoro/qcorotask.h>

namespace Clash::Meta::Proxies {
class GroupItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    class ProxyItem;

    enum GroupType { GlobalGroup, RuleGroup };
    Q_ENUM(GroupType)

    explicit GroupItemModel(QObject *parent = nullptr);
    ~GroupItemModel();

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString currentName READ currentName NOTIFY currentNameChanged)
    Q_PROPERTY(QString groupName READ groupName)
public:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    struct ProxyInfo
    {
        QString proxyName;
        QString type;
        bool isUdp = false;
        int delay = -2; //-2 未检测过 -1超时 大于等于0 具体延迟ms
    };
    void init(GroupType groupType,
              const QString &groupName,
              const QList<ProxyInfo> &proxyInfos,
              const QString &currentProxyName);

    void clear();

public:
    int currentIndex() const { return currentIndex_; }
    void setCurrentIndex(int index);
    QString currentName() const;
    QString groupName() const { return groupName_; }

private:
    QCoro::Task<> __setCurrentIndex(int index);

signals:
    void currentIndexChanged(int index);
    void currentNameChanged(const QString &name);

private:
    QList<ProxyItem *> items_;
    int currentIndex_ = -1;
    QString groupName_;
    GroupType groupType_;
};

class GroupItemModel::ProxyItem : public QObject
{
    Q_OBJECT
public:
    explicit ProxyItem(const ProxyInfo &info, QObject *parent = nullptr);
    ~ProxyItem();

    Q_PROPERTY(bool delayDetecting READ isDelayDetecting NOTIFY delayDetectingChanged)
    Q_PROPERTY(int delay READ delay NOTIFY delayChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(bool udp READ isUdp)

public:
    int delay() const { return info_.delay; }
    void setDelay(int val);
    QString name() const { return info_.proxyName; }
    QString type() const { return info_.type; }
    bool isUdp() const { return info_.isUdp; }

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
    ProxyInfo info_;
};

} // namespace Clash::Meta::Proxies