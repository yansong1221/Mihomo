#include "ProxiesGroupModel.h"
#include "Core/APIClient.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QTimer>

namespace Clash::Meta::Proxies {

class AbstractNode
{
public:
    virtual ~AbstractNode() = default;

    bool alive = false;
    bool tfo = false;
    bool udp = false;
    bool xudp = false;
    QString name;

    virtual QString type() const = 0;
    virtual bool isGroup() const = 0;

    virtual int delay() const { return -1; }

public:
    bool parse(const QJsonObject &obj)
    {
        name = obj["name"].toString();
        alive = obj["alive"].toBool();
        tfo = obj["tfo"].toBool();
        udp = obj["udp"].toBool();
        xudp = obj["xudp"].toBool();

        return onParseData(obj);
    }

protected:
    virtual bool onParseData(const QJsonObject &obj) { return true; }
};
class AbstractGroupNode : public AbstractNode
{
public:
    QStringList all;
    QString now;

    enum class GroupType { GLOBAL, RULE };
    GroupType groupType() const
    {
        return this->name != "GLOBAL" ? GroupType::RULE : GroupType::GLOBAL;
    }

    virtual bool selectable() const = 0;

protected:
    virtual bool onParseData(const QJsonObject &obj) override
    {
        all.clear();
        now.clear();

        auto __all = obj["all"].toArray();
        for (const auto &item : __all.toVariantList())
            all << item.toString();

        now = obj["now"].toString();
        return true;
    }

private:
    virtual bool isGroup() const final { return true; }
};

class AbstractSingleNode : public AbstractNode
{
private:
    virtual bool isGroup() const { return false; };
};

class Compatible : public AbstractSingleNode
{
public:
    inline const static QString Type = "Compatible";
    virtual QString type() const override { return Type; }
};

class Direct : public AbstractSingleNode
{
public:
    inline const static QString Type = "Direct";
    virtual QString type() const override { return Type; }
};
class Reject : public AbstractSingleNode
{
public:
    inline const static QString Type = "Reject";
    virtual QString type() const override { return Type; }
};
class Pass : public AbstractSingleNode
{
public:
    inline const static QString Type = "Pass";
    virtual QString type() const override { return Type; }
};
class RejectDrop : public AbstractSingleNode
{
public:
    inline const static QString Type = "RejectDrop";
    virtual QString type() const override { return Type; }
};

class Selector : public AbstractGroupNode
{
public:
    inline const static QString Type = "Selector";
    virtual QString type() const override { return Type; }
    virtual bool selectable() const override { return true; }
};
class URLTest : public AbstractGroupNode
{
public:
    inline const static QString Type = "URLTest";
    virtual QString type() const override { return Type; }
    virtual bool selectable() const override { return false; }
};

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
    for (auto iter = proxies.constBegin(); iter != proxies.constEnd(); ++iter) {
        if (iter.key() == Selector::Type) {
        } else if (iter.key() == URLTest::Type) {
        } else if (iter.key() == Compatible::Type) {
        } else if (iter.key() == Direct::Type) {
        } else if (iter.key() == Reject::Type) {
        }
    }
}

int GroupModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return 0;
}

QVariant GroupModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
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
    parse(obj);
}

} // namespace Clash::Meta::Proxies
