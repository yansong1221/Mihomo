#pragma once
#include <QAbstractListModel>
#include <qcoro/qcorotask.h>
namespace Clash::Meta::Connections {
class ProxiesGroupModel : public QAbstractListModel
{
    Q_OBJECT
public:
    struct GroupItem
    {
        QString name;
        QString now;
    };
    explicit ProxiesGroupModel(QObject *parent = nullptr);
    ~ProxiesGroupModel();

public slots:
    QCoro::Task<> reload();

private:
    void parse(const QJsonObject& obj);

private:
    bool isLoadding_ = false;
};
} // namespace Clash::Meta::Connections