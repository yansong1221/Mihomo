#pragma once
#include <QString>
#include <QUrl>
#include <qcoro/qcorotask.h>
#include <yaml-cpp/yaml.h>

namespace Clash::Meta::Config {

class YamlBuilder
{
public:
    static QCoro::Task<QString> awaitYamlFromUrl(const QUrl &url);
    static QString yamlFromUrl(const QUrl &url);

    static bool writeFile(const QString &data, const QString &path);
    static bool writeFile(const YAML::Node &data, const QString &path);
};
} // namespace Clash::Meta::Config