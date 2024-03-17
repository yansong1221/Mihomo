#pragma once
#include <QAbstractListModel>
#include <QJSEngine>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <qcoro/qcorotask.h>
namespace Clash::Meta::Proxies {
class GroupModel : public QAbstractListModel
{
    Q_OBJECT
public:
    struct GroupItem
    {
        QString key;

        QString name;
        QString now;
        QString type;
    };
    explicit GroupModel(QObject *parent = nullptr);
    ~GroupModel();

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

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
};
} // namespace Clash::Meta::Proxies