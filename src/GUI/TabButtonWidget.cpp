#include "TabButtonWidget.h"
namespace Clash::Meta::GUI {

TabButtonWidget::TabButtonWidget(QWidget *parent /*= nullptr*/)
    : QListView(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFrameShape(QFrame::NoFrame);
}

TabButtonWidget::~TabButtonWidget() {}

void TabButtonWidget::paintEvent(QPaintEvent *e)
{
    QListView::paintEvent(e);
}

} // namespace Clash::Meta::GUI
