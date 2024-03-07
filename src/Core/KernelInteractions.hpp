#pragma once
#include <optional>
#include <QObject>

class QProcess;

namespace Mihomo::Core {

class KernelInstance : public QObject
{
    Q_OBJECT
public:
    explicit KernelInstance(QObject *parent = nullptr);
    ~KernelInstance() override;

    static KernelInstance &getInstance();
    //
    std::optional<QString> StartConnection(const QString &vCorePath, const QString &configPath);
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
} // namespace Mihomo::Core
