#include "TranslateHelper.h"
#include <QVariant>

namespace Clash::Meta::Components {
TranslateHelper::TranslateHelper(QObject *parent /*= nullptr*/)
    : QObject(parent)
{}

TranslateHelper::~TranslateHelper() {}

TranslateHelper &TranslateHelper::instance()
{
    static TranslateHelper s_instance;
    return s_instance;
}

void TranslateHelper::addObject(QObject *obj, TextCreator creator, const QString &propertyName)
{
    obj->setProperty(propertyName.toStdString().c_str(), creator());

    connect(obj, &QObject::destroyed, this, [this](QObject *obj) {
        auto iter = objs_.find(obj);
        if (iter == objs_.end())
            return;
        objs_.remove(obj);
    });
    objs_[obj] = {propertyName, creator};
}

void TranslateHelper::retranslateUi()
{
    for (auto iter = objs_.begin(); iter != objs_.end(); ++iter) {
        auto obj = iter.key();
        const auto &item = iter.value();
        obj->setProperty(item.propertyName.toStdString().c_str(), item.creator());
    }
}

} // namespace Clash::Meta::Components
