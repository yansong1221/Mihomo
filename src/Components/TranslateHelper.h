#pragma once
#include <memory>
#include <QMap>
#include <QObject>
#include <QString>

#define TR_CREATOR(VAL) [this]() { return tr(VAL); }

namespace Clash::Meta::Components {

class TranslateHelper : public QObject
{
    Q_OBJECT
public:
    using TextCreator = std::function<QString()>;

public:
    explicit TranslateHelper(QObject *parent = nullptr);
    ~TranslateHelper();

public:
    static TranslateHelper &instance();
    void addObject(QObject *obj, TextCreator creator, const QString &propertyName = "text");
    void retranslateUi();

private:
    struct Item
    {
        QString propertyName;
        TextCreator creator;
    };
    QMap<QObject *, Item> objs_;
};

} // namespace Clash::Meta::Components