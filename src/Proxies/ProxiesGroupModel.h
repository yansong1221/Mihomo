#pragma once
#include <QAbstractListModel>
#include <QJSEngine>
#include <QSortFilterProxyModel>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <qcoro/qcorotask.h>
namespace Clash::Meta::Proxies {

class GroupItemModel;
class GroupModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit GroupModel(QObject *parent = nullptr);
    ~GroupModel();

public:
    QModelIndex index(int row,
                      int column = 0,
                      const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //QHash<int, QByteArray> roleNames() const override;

    static QObject *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)

        return new GroupModel(qmlEngine->rootContext()->parent());
    }
public slots:
    void reload();

private:
    void parse(const QJsonObject &obj);

    QCoro::Task<> __reload();

private:
    bool isLoadding_ = false;
    QList<GroupItemModel *> items_;
};

class GroupModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    Q_PROPERTY(QString filerMode READ filerMode WRITE setFilerMode NOTIFY filerModeChanged)

    explicit GroupModelFilter(QObject *parent = nullptr);

public:
    QString filerMode() const { return filerMode_; }
    void setFilerMode(const QString &mode);

signals:
    void filerModeChanged(const QString &mode);

public slots:
    void reload() { model_->reload(); }

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString filerMode_;
    GroupModel *model_;
};
} // namespace Clash::Meta::Proxies