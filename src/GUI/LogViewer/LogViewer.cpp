#include "LogViewer.h"
#include "ui_LogViewer.h"

namespace Clash::Meta::GUI::Log {
LogViewer::LogViewer(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , ui(new Ui::LogViewer)
{
    ui->setupUi(this);
}

LogViewer::~LogViewer()
{
    delete ui;
}

} // namespace Clash::Meta::GUI::Log
