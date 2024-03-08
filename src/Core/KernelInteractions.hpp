#pragma once
#include <optional>
#include <QObject>

class QProcess;

namespace Clash::Meta::Core {

class KernelInstance : public QObject
{
    Q_OBJECT
public:
    explicit KernelInstance(QObject *parent = nullptr);
    ~KernelInstance() override;

    static KernelInstance &instance();
    //
    std::optional<QString> StartConnection(const QString &vCorePath,
                                           const QString &configPath,
                                           const QString &workingDirectory = QString());
    void StopConnection();
    bool KernelStarted = false;
    //
    static bool ValidateConfig(const QString &vCorePath, const QString &path);
    static bool ValidateKernel(const QString &vCorePath, QString *message);
    static std::pair<bool, std::optional<QString>> CheckAndSetCoreExecutableState(
        const QString &vCorePath);

signals:
    void OnProcessErrored(const QString &errMessage);
    void OnProcessOutputReadyRead(const QString &output);

private:
    QProcess *vProcess;
};
} // namespace Clash::Meta::Core
