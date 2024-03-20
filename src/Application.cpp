#include "Application.h"
#include "ClashMetaProcess.h"
#include "Configurator/ClashRuntimeConfigure.h"
#include "Proxies/ProxiesGroupItemModel.h"
#include "Proxies/ProxiesGroupModel.h"
#include "Window.h"
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>

namespace Clash::Meta {
Application::Application(int &argc, char *argv[])
    : SingleApplication(argc, argv)
{}

Application::~Application() {}

void Application::registerTypes(QQmlEngine *engine)
{
    static constexpr const char kModuleUri[] = "Clash.Meta";
    Q_UNUSED(engine);

    static bool once = false;
    if (once) {
        return;
    }
    once = true;

    // @uri QWindowKit
    qmlRegisterType<GUI::Window>(kModuleUri, 1, 0, "ClashWindow");
    qmlRegisterType<Proxies::GroupModel>(kModuleUri, 1, 0, "ProxiesGroupModel");
    qmlRegisterType<Proxies::GroupModelFilter>(kModuleUri, 1, 0, "ProxiesGroupModelFilter");
    //qmlRegisterType<Proxies::GroupItemModel>(kModuleUri, 1, 0, "ProxiesGroupItemModel");
    //qmlRegisterType<Proxies::GroupItemModel::ProxyItem>(kModuleUri, 1, 0, "ProxiesGroupItemProxyItem");
    qmlRegisterModule(kModuleUri, 1, 0);

    engine->rootContext()->setContextProperty("ClashMetaProcess", &ClashMetaProcess::instance());
    engine->rootContext()->setContextProperty("ClashRuntimeConfigure",
                                              &Config::ClashRuntimeConfigure::instance());
}

} // namespace Clash::Meta