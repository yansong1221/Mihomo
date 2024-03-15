#include "ClashMetaProcess.h"
#include "Configurator/ClashConfigurator.h"
#include <QTimer>
#include "Configurator/YamlBuilder.h"

namespace Clash::Meta {
ClashMetaProcess::ClashMetaProcess(QObject *parent /*= nullptr*/)
    : QObject(parent)
{
    kernel_ = new Core::KernelInstance(this);
    connect(kernel_, &Core::KernelInstance::OnProcessErrored, this, [this](const QString &errorStr) {
        QTimer::singleShot(1000, this, &ClashMetaProcess::restart);
    });
    connect(kernel_->process(), &QProcess::stateChanged, this, [this]() {
        Q_EMIT runningChanged(isRunning());
    });
    return;
    auto data = Config::YamlBuilder::yamlFromUrl(
        QUrl("https://api.subcloud.xyz/"
             "sub?target=clash&url=https%3A%2F%2Ffeiniaoyun01.life%2Fapi%2Fv1%2Fclient%2Fsubscribe%"
             "3Ftoken%3Df1a2350571c467d6412cd9279f8fc160&insert=false&config=https%3A%2F%2Fraw."
             "githubusercontent.com%2FACL4SSR%2FACL4SSR%2Fmaster%2FClash%2Fconfig%2FACL4SSR_Online."
             "ini&emoji=true&list=false&tfo=false&scv=true&fdn=false&sort=false&new_name=true"));

    Config::YamlBuilder::writeFile(data, Config::ClashConfigurator::fullConfigPath());
}

ClashMetaProcess::~ClashMetaProcess() {}

ClashMetaProcess &ClashMetaProcess::instance()
{
    static ClashMetaProcess s_instance;
    return s_instance;
}

void ClashMetaProcess::restart()
{
    kernel_->StopConnection();
    kernel_->StartConnection(Config::ClashConfigurator::executableFilePath(),
                             Config::ClashConfigurator::fullConfigPath());
}

bool ClashMetaProcess::isRunning() const
{
    return kernel_->process()->state() == QProcess::Running;
}

} // namespace Clash::Meta
