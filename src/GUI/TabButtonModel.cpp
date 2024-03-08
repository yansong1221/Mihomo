#include "TabButtonModel.h"
#include <QPainter>

namespace Clash::Meta::GUI {

void TabButtonItemDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    //QItemDelegate::paint(painter,option,index);
    //return;

    painter->save();

    //drawBackground(painter, option, index);
    //painter->fillRect(option.rect, Qt::transparent);

    auto text = index.data().toString();
    auto ico = index.data(Qt::DecorationRole).value<QIcon>();

    Qt::Alignment alignment = drawTextAlignment(option, index);

    QRect iconRect = this->drawIconRect(option, index);
    QRect textRect = this->drawTextRect(option, index);

    if (option.state.testFlag(QStyle::State_MouseOver)) {
        painter->fillRect(option.rect, QColor(233, 233, 233));
        painter->fillRect(option.rect, option.palette.color(QPalette::Highlight));
    }

    if (!ico.isNull()) {
        QSize iconSize(32, 32);
        // 获取绘制区域的中心点
        QPoint center = iconRect.center();

        // 计算图片的左上角坐标
        int x = center.x() - iconSize.width() / 2;
        int y = iconRect.bottom() - iconSize.height() - 2;

        if (option.state.testFlag(QStyle::State_Selected))
            painter->drawPixmap(x, y, ico.pixmap(iconSize, QIcon::Selected));
        else
            painter->drawPixmap(x, y, ico.pixmap(iconSize, QIcon::Normal));
    }

    if (option.state.testFlag(QStyle::State_Selected)) {
        //painter->fillRect(option.rect, option.palette.color(QPalette::Highlight));
        painter->setPen(option.palette.color(QPalette::HighlightedText));
        painter->setBrush(Qt::NoBrush);
    } else {
        painter->setPen(option.palette.color(QPalette::Dark));
        painter->setBrush(Qt::NoBrush);
    }
    painter->drawText(textRect, alignment, text);
    painter->restore();
}

QRect TabButtonItemDelegate::drawIconRect(const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    auto ico = index.data(Qt::DecorationRole).value<QIcon>();
    if (ico.isNull())
        return QRect();

    return QRect(QPoint(option.rect.topLeft()),
                 QSize(option.rect.width(), option.rect.height() / 2));
}

QRect TabButtonItemDelegate::drawTextRect(const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    auto _rect = drawIconRect(option, index);
    if (_rect.isNull())
        return option.rect;

    return QRect(QPoint(option.rect.x(), _rect.bottom()), option.rect.bottomRight());
}

Qt::Alignment TabButtonItemDelegate::drawTextAlignment(const QStyleOptionViewItem &option,
                                                       const QModelIndex &index) const
{
    return drawIconRect(option, index).isNull() ? Qt::AlignCenter : Qt::AlignHCenter | Qt::AlignTop;
}

QSize TabButtonItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    auto sz = QItemDelegate::sizeHint(option, index);
    return QSize(sz.width(), 60);
}

TabButtonModel::TabButtonModel(QObject *parent /*= nullptr*/)
    : QAbstractListModel(parent)
{
    ButtonItem item;
    item.text = "参数设置";
    item.ico.addPixmap(QPixmap(":/global/images/Meta.png"));
    item.ico.addPixmap(QPixmap(":/dark/images/normaled.png"),QIcon::Selected);
    items_ << item;
}

TabButtonModel::~TabButtonModel() {}

int TabButtonModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return items_.length();
}

QVariant TabButtonModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    const auto &item = items_[index.row()];
    switch (role) {
    case Qt::DisplayRole:
        return item.text;
    case Qt::DecorationRole:
        return item.ico;
        break;
    default:
        break;
    }
    return QVariant();
}

} // namespace Clash::Meta::GUI
