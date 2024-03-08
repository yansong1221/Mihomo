#include "Application.h"
#include "Configurator/ClashConfigurator.h"
#include "Core/KernelInteractions.hpp"
#include "GUI/MainWindow.h"

int main(int argc, char *argv[])
{
    using namespace Clash::Meta;

    QCoreApplication::setOrganizationName("MetaCubeX");
    QCoreApplication::setApplicationName("ClashMeta-GUI");

    Application app(argc, argv);

    QApplication::setWindowIcon(QIcon(":/global/images/Meta.png"));

    Config::ClashConfigurator::instance().toYamlFile();
    Core::KernelInstance::instance().StartConnection(Config::ClashConfigurator::executableFilePath(),
                                                     Config::ClashConfigurator::fullConfigPath());

    GUI::MainWindow w;
    w.show();

    return app.exec();
}