#include "GeneralWidget.h"
#include "ui_GeneralWidget.h"
#include <QtMWidgets/TextLabel>

namespace Clash::Meta::GUI {
GeneralWidget::GeneralWidget(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , ui(new Ui::GeneralWidget)
{
    ui->setupUi(this);
    //auto section = new QtMWidgets::TableViewSection(this);

    //{
    //    auto cell = new QtMWidgets::TableViewCell(this);
    //    cell->setHighlightOnClick(true);
    //    cell->textLabel()->setText(tr("Port"));

    //    section->addCell(cell);
    //}
    //ui->view->addSection(section);
}

GeneralWidget::~GeneralWidget()
{
    delete ui;
}

} // namespace Clash::Meta::GUI
