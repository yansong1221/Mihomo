#include "ClashConfigurator.h"
#include <yaml-cpp/yaml.h>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

namespace Clash::Meta::Config {

inline static YAML::Node convert(const QStringList &datas)
{
    YAML::Node arrayNode(YAML::NodeType::Sequence);
    for (const auto &item : datas)
        arrayNode.push_back(item.toStdString());

    return arrayNode;
}

inline static YAML::Node convert(const ClashConfigurator::Profile &data)
{
    YAML::Node node;
    node["store-selected"] = data.store_selected;
    node["store-fake-ip"] = data.store_fake_ip;
    return node;
}
inline static YAML::Node convert(const ClashConfigurator::TLS &data)
{
    YAML::Node node;
    node["certificate"] = data.certificate.toStdString();
    node["private-key"] = data.private_key.toStdString();
    return node;
}
inline static YAML::Node convert(const ClashConfigurator::GeoxUrl &data)
{
    YAML::Node node;
    node["geoip"] = data.geoip.toStdString();
    node["geosite"] = data.geosite.toStdString();
    node["mmdb"] = data.mmdb.toStdString();
    return node;
}
inline static std::string convert(const ClashConfigurator::Endpoint &data)
{
    return QString("%1:%2").arg(data.host).arg(data.port).toStdString();
}

inline static YAML::Node convert(const ClashConfigurator &config)
{
    YAML::Node data;
    data["allow-lan"] = config.allow_lan;

    if (!config.bind_address.isEmpty())
        data["bind-address"] = config.bind_address.toStdString();

    if (!config.lan_allowed_ips.isEmpty())
        data["lan-allowed-ips"] = convert(config.lan_allowed_ips);

    if (!config.lan_disallowed_ips.isEmpty())
        data["lan-disallowed-ips"] = convert(config.lan_disallowed_ips);

    if (!config.authentication.isEmpty())
        data["authentication"] = convert(config.authentication);

    if (!config.skip_auth_prefixes.isEmpty())
        data["skip-auth-prefixes"] = convert(config.skip_auth_prefixes);

    if (!config.mode.isEmpty())
        data["mode"] = config.mode.toStdString();

    if (!config.log_level.isEmpty())
        data["log-level"] = config.log_level.toStdString();

    data["ipv6"] = config.ipv6;

    if (config.keep_alive_interval)
        data["keep-alive-interval"] = config.keep_alive_interval.value();

    if (!config.find_process_mode.isEmpty())
        data["find-process-mode"] = config.find_process_mode.toStdString();

    data["external-controller"] = convert(config.controller.http);
    if (config.controller.https)
        data["external-controller-tls"] = convert(config.controller.https.value());
    if (config.controller.secret)
        data["secret"] = config.controller.secret.value().toStdString();

    if (!config.external_ui.isEmpty())
        data["external-ui"] = config.external_ui.toStdString();

    if (!config.external_ui_name.isEmpty())
        data["external-ui-name"] = config.external_ui_name.toStdString();

    if (!config.external_ui_url.isEmpty())
        data["external-ui-url"] = config.external_ui_url.toStdString();

    if (config.profile)
        data["profile"] = convert(config.profile.value());

    if (config.unified_delay)
        data["unified-delay"] = config.unified_delay.value();

    if (config.tcp_concurrent)
        data["tcp-concurrent"] = config.tcp_concurrent.value();

    if (!config.interface_name.isEmpty())
        data["interface-name"] = config.interface_name.toStdString();

    if (!config.routing_mark.isEmpty())
        data["routing-mark"] = config.routing_mark.toStdString();

    if (config.tls)
        data["tls"] = convert(config.tls.value());

    if (!config.global_client_fingerprint.isEmpty())
        data["global-client-fingerprint"] = config.global_client_fingerprint.toStdString();

    if (config.geodata_mode)
        data["geodata-mode"] = config.geodata_mode.value();

    if (!config.geodata_loader.isEmpty())
        data["geodata-loader"] = config.geodata_loader.toStdString();

    data["geo-auto-update"] = config.geo_auto_update;
    data["geo-update-interval"] = config.geo_update_interval;

    if (config.geox_url)
        data["geox-url"] = convert(config.geox_url.value());

    if (!config.global_ua.isEmpty())
        data["global-ua"] = config.global_ua.toStdString();

    return data;
}

ClashConfigurator::ClashConfigurator(QObject *parent /*= nullptr*/)
    : QObject(parent)
{}

ClashConfigurator &ClashConfigurator::instance()
{
    static ClashConfigurator s_instance;
    return s_instance;
}

QString ClashConfigurator::toYamlString() const
{
    auto data = convert(*this);
    return QString::fromStdString(YAML::Dump(data));
}

std::optional<QString> ClashConfigurator::toYamlFile(const QString &path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Unable to open file: " << path;
        return tr("Unable to open file: ") + path;
    }
    QTextStream stream(&file);
    stream << toYamlString();
    return std::nullopt;
}

std::optional<QString> ClashConfigurator::toYamlFile() const
{
    return toYamlFile(ClashConfigurator::fullConfigPath());
}

QString ClashConfigurator::configLocation()
{
    return resourceLocation();
}

QString ClashConfigurator::resourceLocation()
{
    auto dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/clash.meta/";
    if (!QDir().exists(dir))
        QDir().mkpath(dir);
    return dir;
}

QString ClashConfigurator::fullConfigPath()
{
    QDir dir(configLocation());
    return dir.filePath("config.yaml");
}

QString ClashConfigurator::executableFilePath()
{
    auto dir = QCoreApplication::applicationDirPath() + "/clash.meta/";
    return QDir(dir).filePath("mihomo.exe");
}

} // namespace Clash::Meta::Config