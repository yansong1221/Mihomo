#include "MainWindow.h"
#include "core/APIClient.h"
#include "connections/ConnectionsModel.h"

namespace Mihomo {
MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
{
 
   auto model = new Ui::Connections::ConnectionsModel(this);

}

MainWindow::~MainWindow() {}

} // namespace mihomo