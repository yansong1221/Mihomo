#include "YamlBuilder.h"
#include "Components/HttpClient.h"
#include <QFile>
#include <QNetworkReply>
#include <QTextStream>
#include <QDebug>

namespace Clash::Meta::Config {
QCoro::Task<QString> YamlBuilder::awaitYamlFromUrl(const QUrl &url)
{
    auto reply = co_await Components::HttpClient::instance().awaitGet(url);
    if (reply->error() != QNetworkReply::NoError)
        co_return QString();

    auto data = reply->readAll();
    co_return QString::fromUtf8(data);
}

QString YamlBuilder::yamlFromUrl(const QUrl &url)
{
    try {
        return QCoro::waitFor(awaitYamlFromUrl(url));
    } catch (const std::exception & e) {
        qDebug() << e.what();
    }
    return QString();
}

bool YamlBuilder::writeFile(const QString &data, const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QTextStream stream(&file);
    stream << data;
    return true;
}

bool YamlBuilder::writeFile(const YAML::Node &data, const QString &path)
{
    auto str = QString::fromStdString(YAML::Dump(data));
    return writeFile(str, path);
}

} // namespace Clash::Meta::Config
