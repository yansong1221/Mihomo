#pragma once
#include <QObject>
#include <QString>
//
namespace Mihomo::Core {

class SysProxyConfigurator
{
public:
    static void ClearSystemProxy();
    static void SetSystemProxy(const QString &address, int http_port, int socks_port);
};

} // namespace Mihomo::Core
