#pragma once
#include <QObject>
#include <QString>
#include <QStringList>

namespace Clash::Meta::Config {

class ClashConfigurator : public QObject
{
    Q_OBJECT
public:
    struct Endpoint
    {
        QString host;
        ushort port = 0;
    };

    /**
         * 允许其他设备经过 Clash 的代理端口访问互联网可选值 true/false
         */
    bool allow_lan = true;
    /**
         * 绑定地址,仅允许其他设备通过这个地址访问
         "*" 绑定所有 IP 地址
         192.168.31.31 绑定单个 IPV4 地址
         [aaaa::a8aa:ff:fe09:57d8] 绑定单个 IPV6 地址
         */
    QString bind_address = "*";

    /** 
        允许连接的IP地址段，仅作用于 allow-lan 为 true 默认值为 0.0.0.0/0和 ::/0 
        */
    QStringList lan_allowed_ips{"0.0.0.0/0", "::/0"};

    /**
         * 禁止连接的IP地址段,黑名单优先级高于白名单,默认值为空 192.168.0.3/32
         */
    QStringList lan_disallowed_ips;
    /**
         * http(s)/socks/mixed代理的用户验证 "user1:pass1" "user2:pass2"
         */
    QStringList authentication;

    /**
         * 设置允许跳过验证的IP段 127.0.0.1/8 ::1/128
         */
    QStringList skip_auth_prefixes;

    /**
         * rule 规则匹配
         * global 全局代理(需要在GLOBAL策略组选择代理/策略)
         * direct 全局直连
         * 此项拥有默认值，默认为规则模式
         */
    QString mode = "rule";

    /**
         * Clash 内核输出日志的等级，仅在控制台和控制页面输出
         * silent 静默，不输出
         * error 仅输出发生错误至无法使用的日志
         * warning 输出发生错误但不影响运行的日志，以及 error 级别内容
         * info 输出一般运行的内容，以及 error 和 warning 级别的日志
         * debug 尽可能的输出运行中所有的信息
         */
    QString log_level = "info";

    /**
         * 是否允许内核接受 IPv6 流量
         * 可选值 true/false,默认为 true
         */
    bool ipv6 = true;

    /**
         * 控制 Clash 发出 TCP Keep Alive 包的间隔，减少移动设备耗电问题的临时措施。
         * 此时 Clash 发现并关闭一个无效 TCP 连接用时： 1*keep-alive-interval + 9*keep-alive-interval
         */
    std::optional<int> keep_alive_interval;

    /**
         * 控制是否让 Clash 去匹配进程
         * always 开启，强制匹配所有进程
         * strict 默认，由 Clash 判断是否开启
         * off 不匹配进程，推荐在路由器上使用此模式
         */
    QString find_process_mode = "strict";

    /**
     * https://wiki.metacubex.one/config/general/#api
     */
    struct ExternalController
    { /**
         * 外部控制器，可以使用 RESTful API 来控制你的 Clash 内核
         * API 监听地址，你可以将 127.0.0.1 修改为 0.0.0.0 来监听所有 IP
         */
        Endpoint http = {"127.0.0.1", 9097};

        /*
        * HTTPS-API 监听地址，需要配置 tls 部分证书和其私钥配置，使用 TLS 也必须填写 external-controller
        * 127.0.0.1:9443
        */
        std::optional<Endpoint> https;

        /**
         * API 的访问密钥
         */
        std::optional<QString> secret;
    };
    ExternalController controller;

    /**
         * 可以将静态网页资源（比如 Clash-dashboard) 运行在 Clash API, 路径为 API 地址/ui
         */
    QString external_ui;

    /**
         * 非必须，更新时会更新到指定文件夹，不配置则直接更新到 external-ui 目录
         * #  合并为 external-ui/xd
         */
    QString external_ui_name;
    /**
         * 自定义外部用户界面下载地址
         */
    QString external_ui_url;

    /**
         * 在 Clash 官方中，profile 应为扩展配置，但在 Clash.meta, 仅作为缓存项使用
         */
    struct Profile
    {
        /**
             * # 储存 API 对策略组的选择，以供下次启动时使用
             */
        bool store_selected = true;
        /**
             *  # 储存 fakeip 映射表，域名再次发生连接时，使用原有映射地址
             */
        bool store_fake_ip = true;
    };
    std::optional<Profile> profile;

    /**
         * 统一延迟
         * 更换延迟计算方式,去除握手等额外延迟
         * 可选值 true/false
         */
    std::optional<bool> unified_delay;

    /**
         * TCP并发 可选值 true/false
         */
    std::optional<bool> tcp_concurrent;

    /**
         * Clash 的流量出站接口
         */
    QString interface_name;

    /**
         * 为 Linux 下的出站连接提供默认流量标记
         */
    QString routing_mark;

    /**
         * 目前仅用于 API 的 https
         * https://wiki.metacubex.one/config/general/#tls
         */
    struct TLS
    {
        /**
             * # 证书 PEM 格式，或者 证书的路径
             */
        QString certificate;
        /**
             * # 证书对应的私钥 PEM 格式，或者私钥路径
             */
        QString private_key;
    };
    std::optional<TLS> tls;

    /**
         * 全局 TLS 指纹，优先低于 proxy 内的 client-fingerprint。
         * 目前支持开启 TLS 传输的 TCP/grpc/WS/HTTP , 支持协议有 VLESS,Vmess 和 trojan.
         * https://wiki.metacubex.one/config/general/#_13
         */
    QString global_client_fingerprint;

    /**
         * 更改geoip使用文件,mmdb或者dat,可选 true/false,true为dat,此项有默认值 false
         * https://wiki.metacubex.one/config/general/#geo
         */
    std::optional<bool> geodata_mode;

    /**
         * GEO文件加载模式
         * 可选的加载模式如下
         * standard：标准加载器
         * memconservative：专为内存受限(小内存)设备优化的加载器(默认值)
         * https://wiki.metacubex.one/config/general/#geo_1
         */
    QString geodata_loader = "standard";

    /**
         * 自动更新 GEO
         * https://wiki.metacubex.one/config/general/#geo_2
         */
    bool geo_auto_update = false;

    /**
         * 更新间隔,单位小时
         */
    int geo_update_interval = 24;

    /**
         * 自定 GEO 下载地址
         * https://wiki.metacubex.one/config/general/#geo_3
         */
    struct GeoxUrl
    {
        QString geoip;
        QString geosite;
        QString mmdb;
    };
    std::optional<GeoxUrl> geox_url;

    /**
         * 自定全局UA
         * 自定义外部资源下载时使用的的UA,默认为 clash.meta
         * https://wiki.metacubex.one/config/general/#ua
         */
    QString global_ua = "clash.meta";

public:
    QString toYamlString() const;
    std::optional<QString> toYamlFile(const QString &path) const;
    std::optional<QString> toYamlFile() const;

    static ClashConfigurator &instance();

    static QString resourceLocation();
    static QString configLocation();

    static QString fullConfigPath();
    static QString executableFilePath();

public:
    explicit ClashConfigurator(QObject *parent = nullptr);
};

} // namespace Clash::Meta::Config