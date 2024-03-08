#pragma once
#include <type_traits>
#include <QCoreApplication>
#include <QEvent>
#include <QObject>
#include <QWidget>
namespace Clash::Meta::Components {

// 使用 SFINAE (Substitution Failure Is Not An Error) 技术来判断派生关系
template<typename Base, typename Derived>
struct is_derived_from
{
    static constexpr bool value = std::is_base_of<Base, Derived>::value;
};

// 特化模板，如果是 Derived 继承自 Base，则设置 value 为 true
template<typename Base, typename Derived>
struct is_derived_from<Base, Derived &>
{
    static constexpr bool value = true;
};

// 使用别名模板，更方便使用
template<typename Base, typename Derived>
constexpr bool is_derived_from_v = is_derived_from<Base, Derived>::value;

template<typename BaseClass>
class ObjectEvent : public BaseClass
{
    static_assert(is_derived_from_v<QObject, BaseClass>, "template MUST be derived from QObject!");

public:
    template<typename T = BaseClass>
    explicit ObjectEvent(
        std::conditional_t<is_derived_from_v<QWidget, T>, QWidget, QObject> *parent = nullptr)
        : BaseClass(parent)
    {
        QCoreApplication::instance()->installEventFilter(this);
    }

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    virtual void retranslateUi() {}
};

template<typename BaseClass>
inline bool ObjectEvent<BaseClass>::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == QCoreApplication::instance()) {
        switch (event->type()) {
        case QEvent::LanguageChange:
            this->retranslateUi();
            break;
        default:
            break;
        }
    }
    return BaseClass::eventFilter(watched, event);
}

} // namespace Mihomo::Components