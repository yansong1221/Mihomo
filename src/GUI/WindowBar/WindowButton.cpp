#include "WindowButton.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyle>
namespace Clash::Meta::GUI {
WindowButton::WindowButton(QWidget *parent /*= nullptr*/)
    : QPushButton(parent)
{
    this->setFocusPolicy(Qt::NoFocus);
}

WindowButton::~WindowButton() {}

void WindowButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        Q_EMIT doubleClicked();
    }
}

} // namespace Clash::Meta::GUI