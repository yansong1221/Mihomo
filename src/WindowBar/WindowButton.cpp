#include "WindowButton.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
namespace Mihomo::GUI {
WindowButton::WindowButton(QWidget *parent /*= nullptr*/)
    : QPushButton(parent)
{}

WindowButton::~WindowButton() {}

void WindowButton::setHoverColor(QColor color)
{
    if (hoverColor_ == color)
        return;

    hoverColor_ = color;
    update();
}

void WindowButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (isHovered_)
        painter.fillRect(rect(), hoverColor_);

    auto Image = this->icon().pixmap(this->iconSize());
    if (Image.isNull())
        return;

    auto imageRect = this->contentsRect();

    // 获取绘制区域的中心点
    QPoint center = imageRect.center();

    // 计算图片的左上角坐标
    int x = center.x() - Image.width() / 2;
    int y = center.y() - Image.height() / 2;

    // 绘制图片
    painter.drawPixmap(x, y, Image);
    return;
}

void WindowButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        Q_EMIT doubleClicked();
    }
}

void WindowButton::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    isHovered_ = true;
    update();
}

void WindowButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    isHovered_ = false;
    update();
}

void MaximizeWindowButton::setMaximizeIcon(const QIcon &icon)
{
    maximizeIcon_ = icon;
    updateIcon();
}

void MaximizeWindowButton::setNormaledIcon(const QIcon &icon)
{
    normaledIcon_ = icon;
    updateIcon();
}

void MaximizeWindowButton::updateIcon()
{
    if (!watchWidget()) {
        this->setIcon(normaledIcon_);
        return;
    }

    if (watchWidget()->isMaximized()) {
        this->setIcon(normaledIcon_);
    } else {
        this->setIcon(maximizeIcon_);
    }
}

void MaximizeWindowButton::onWatchWidgetEvent(QWidget *widget, QEvent *event)
{
    if (event->type() != QEvent::WindowStateChange)
        return;

    updateIcon();
}

void AlwaysTopWindowButton::onWatchWidgetEvent(QWidget *widget, QEvent *event)
{
    if (event->type() != QEvent::ModifiedChange)
        return;
    updateIcon();
}

void AlwaysTopWindowButton::updateIcon()
{
    if (!watchWidget()) {
        this->setIcon(uncheckedIcon_);
        return;
    }

    if (watchWidget()->windowFlags().testFlag(Qt::WindowStaysOnTopHint)) {
        this->setIcon(checkedIcon_);
    } else {
        this->setIcon(uncheckedIcon_);
    }
}

void AlwaysTopWindowButton::setCheckedIcon(const QIcon &icon)
{
    checkedIcon_ = icon;
    updateIcon();
}

void AlwaysTopWindowButton::setUnCheckedIcon(const QIcon &icon)
{
    uncheckedIcon_ = icon;
    updateIcon();
}

void WatchStateButton::setWatchWidget(QWidget *widget)
{
    if (watchWidget_ == widget)
        return;

    if (watchWidget_)
        watchWidget_->removeEventFilter(this);

    watchWidget_ = widget;
    if (watchWidget_)
        watchWidget_->installEventFilter(this);

    updateIcon();
}

QWidget *WatchStateButton::watchWidget() const
{
    return watchWidget_;
}

bool WatchStateButton::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == watchWidget_)
        this->onWatchWidgetEvent(watchWidget_, event);

    return QObject::eventFilter(watched, event);
}

} // namespace Mihomo::GUI