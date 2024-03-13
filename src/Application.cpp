#include "Application.h"
#include <QtQml/QQmlEngine>
#include "Window.h"

namespace Clash::Meta {
Application::Application(int &argc, char *argv[])
    : SingleApplication(argc, argv)
{}

Application::~Application() {}

void Application::registerTypes(QQmlEngine *engine) 
{
    static constexpr const char kModuleUri[] = "Clash.Meta.GUI";
    Q_UNUSED(engine);

    static bool once = false;
    if (once) {
        return;
    }
    once = true;

    // @uri QWindowKit
    qmlRegisterType<GUI::Window>(kModuleUri, 1, 0, "ClashWindow");
    qmlRegisterModule(kModuleUri, 1, 0);
}

} // namespace mihomo