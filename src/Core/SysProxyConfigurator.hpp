#pragma once
#include <QObject>
#include <QString>
//
namespace Clash::Meta::Core {

class SysProxyConfigurator
{
public:
    static void ClearSystemProxy();
    static void SetSystemProxy(const QString &address, int http_port, int socks_port);
};

} // namespace Mihomo::Core
