#pragma once
#include <QAbstractListModel>
#include <QIcon>
#include <QItemDelegate>
#include <QStyledItemDelegate>
namespace Clash::Meta::GUI {

class TabButtonItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    using QItemDelegate::QItemDelegate;

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QRect drawIconRect(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QRect drawTextRect(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    Qt::Alignment drawTextAlignment(const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const;
};

class TabButtonModel : public QAbstractListModel
{
    Q_OBJECT
public:
    struct ButtonItem
    {
        QIcon ico;
        QString text;
        QWidget *widget = nullptr;
    };

    explicit TabButtonModel(QObject *parent = nullptr);
    ~TabButtonModel();

public:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<ButtonItem> items_;
};
} // namespace Clash::Meta::GUI