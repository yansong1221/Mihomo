#include "Application.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MetaCubeX");
    QCoreApplication::setApplicationName("Mihomo");

    Mihomo::Application app(argc, argv);

    QApplication::setWindowIcon(QIcon(":/global/images/Meta.png"));

    Mihomo::MainWindow w;
    w.show();

    return app.exec();
}