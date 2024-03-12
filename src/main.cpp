#include "Application.h"
#include "Configurator/ClashConfigurator.h"
#include "Core/KernelInteractions.hpp"

#include <QFile>
#include <QString>
#include <QStringDecoder>

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>

#include <QWKQuick/qwkquickglobal.h>
#include <QskQml.h>

QString StringFromFile(QFile &source)
{
    bool wasOpened = source.isOpen();
    if (!wasOpened)
        source.open(QFile::ReadOnly);
    auto byteArray = source.readAll();
    if (!wasOpened)
        source.close();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    for (const auto &encoding :
         {QStringDecoder::Utf8, QStringDecoder::Utf16, QStringDecoder::System}) {
        auto converter = QStringDecoder(encoding, QStringConverter::Flag::ConvertInvalidToNull);
        const auto data = converter(byteArray).data;
        if (!data.contains('\0'))
            return data;
    }
    Q_ASSERT_X(false, Q_FUNC_INFO, "Unsupported File Encoding");
    return "";
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::ConverterState state;
    const QString text = codec->toUnicode(byteArray.constData(), byteArray.size(), &state);
    if (state.invalidChars > 0) {
        LOG("Not a valid UTF-8 sequence: " + source.fileName());
    }
    return state.invalidChars > 0 ? byteArray : text;
#endif
}
QString StringFromFile(const QString &filePath)
{
    QFile f(filePath);
    return StringFromFile(f);
}
int main(int argc, char *argv[])
{
    using namespace Clash::Meta;

    QCoreApplication::setOrganizationName("MetaCubeX");
    QCoreApplication::setApplicationName("ClashMeta-GUI");

    Application app(argc, argv);

    QGuiApplication::setWindowIcon(QIcon(":/images/Meta.png"));

    QskQml::registerTypes();

    QQuickWindow::setDefaultAlphaBuffer(true);
    QQmlApplicationEngine engine;
    QWK::registerTypes(&engine);
    //engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));
    engine.load(QUrl::fromLocalFile("D:\\Mihomo\\assets\\qml\\main.qml"));
    return app.exec();
}