#include "WindowButton.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

WindowButton::WindowButton(QWidget *parent /*= nullptr*/)
    : QPushButton(parent)
{}

WindowButton::~WindowButton() {}

void WindowButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (isHovered_) {
        // 绘制悬停时的样式
        painter.fillRect(rect(), Qt::blue);
    } else {
        // 绘制非悬停时的样式
        painter.fillRect(rect(), Qt::gray);
    }
}

void WindowButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        Q_EMIT doubleClicked();
    }
}

void WindowButton::enterEvent(QEnterEvent *event)
{
    isHovered_ = true;
    update();
}

void WindowButton::leaveEvent(QEvent *event)
{
    isHovered_ = false;
    update();
}
