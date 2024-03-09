#include "GeneralWidget.h"
#include "ui_GeneralWidget.h"
namespace Clash::Meta::GUI {
GeneralWidget::GeneralWidget(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , ui(new Ui::GeneralWidget)
{
    ui->setupUi(this);
}

GeneralWidget::~GeneralWidget()
{
    delete ui;
}

} // namespace Clash::Meta::GUI
