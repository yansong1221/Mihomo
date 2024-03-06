#include "Application.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    Mihomo::Application app(argc, argv);

    Mihomo::MainWindow w;
    w.show();

    return app.exec();
}