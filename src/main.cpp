#include "Application.h"
#include "Configurator/ClashConfigurator.h"
#include "Core/KernelInteractions.hpp"

#include <QFile>
#include <QString>
#include <QStringDecoder>

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/qqmlextensionplugin.h>
#include <QtQuick/QQuickWindow>

#include <QWKQuick/qwkquickglobal.h>

#ifdef FLUENTUI_BUILD_STATIC_LIB
#if (QT_VERSION > QT_VERSION_CHECK(6, 2, 0))
Q_IMPORT_QML_PLUGIN(FluentUIPlugin)
#endif
#include <FluentUI.h>
#endif

int main(int argc, char *argv[])
{
    using namespace Clash::Meta;

#ifdef WIN32
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
#else
    qputenv("QT_QUICK_CONTROLS_STYLE", "Default");
#endif
#ifdef Q_OS_LINUX
    //fix bug UOSv20 does not print logs
    qputenv("QT_LOGGING_RULES", "");
    //fix bug UOSv20 v-sync does not work
    qputenv("QSG_RENDER_LOOP", "basic");
#endif

    QCoreApplication::setOrganizationName("MetaCubeX");
    QCoreApplication::setApplicationName("ClashMeta-GUI");

    Application app(argc, argv);

    QGuiApplication::setWindowIcon(QIcon(":/assets/meta.png"));

    QQuickWindow::setDefaultAlphaBuffer(true);
    QQmlApplicationEngine engine;

    QWK::registerTypes(&engine);
    Application::registerTypes(&engine);

#ifdef FLUENTUI_BUILD_STATIC_LIB
    FluentUI::getInstance()->registerTypes(&engine);
#endif

    QUrl url(QStringLiteral("qrc:///Clash/Meta/GUI/qml/main.qml"));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}