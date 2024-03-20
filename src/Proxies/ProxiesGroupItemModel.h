#pragma once
#include <QAbstractListModel>
#include <qcoro/qcorotask.h>

namespace Clash::Meta::Proxies {
class GroupItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ProxyRoleType {
        ProxyDelayRole = Qt::UserRole + 1,
        ProxyNameRole,
        ProxyTypeRole,
        ProxyUdpRole,
        ProxyDelayDetectingRole
    };

    enum GroupType { GlobalGroup, RuleGroup };
    Q_ENUM(GroupType)

    explicit GroupItemModel(GroupType groupType,
                            const QString &groupName,
                            QObject *parent = nullptr);
    ~GroupItemModel();

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString currentName READ currentName NOTIFY currentNameChanged)
    Q_PROPERTY(QString groupName READ groupName)
public:
    QModelIndex index(int row,
                      int column = 0,
                      const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual QHash<int, QByteArray> roleNames() const override;

public:
    void append(const QString &proxyName, const QString &type, bool isUdp, int delay);
    void setCurrentProxyName(const QString &currentProxyName);

    void clear();
    GroupType groupType() const { return groupType_; }

public:
    int currentIndex() const { return currentIndex_; }
    void setCurrentIndex(int index);
    QString currentName() const;
    QString groupName() const { return groupName_; }

    Q_INVOKABLE void delayDetect(int row);

private:
    QCoro::Task<> __setCurrentIndex(int row);
    QCoro::Task<> __delayDetect(int row);
    void __setDelay(int row, int val);

signals:
    void currentIndexChanged(int index);
    void currentNameChanged(const QString &name);

private:
    struct ProxyInfo
    {
        QString proxyName;
        QString type;
        bool isUdp = false;
        bool delayDetecting = false;
        int delay = -2; //-2 未检测过 -1超时 大于等于0 具体延迟ms
    };
    QList<ProxyInfo> items_;

    int currentIndex_ = -1;
    QString groupName_;
    GroupType groupType_;
};

} // namespace Clash::Meta::Proxies