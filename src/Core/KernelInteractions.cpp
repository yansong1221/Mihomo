#include "KernelInteractions.hpp"
#include "KernelABIChecker.hpp"
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProcess>
#include <QSaveFile>

#define NEWLINE "\r\n"

#ifdef Q_OS_MACOS
#define ACCESS_OPTIONAL_VALUE(obj) (*obj)
#else
#define ACCESS_OPTIONAL_VALUE(obj) (obj.value())
#endif

#define QSTRN(num) QString::number(num)

static QStringList SplitLines(const QString &_string)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return _string.split(QRegularExpression("[\r\n]"), Qt::SkipEmptyParts);
#elif QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    return _string.split(QRegExp("[\r\n]"), Qt::SkipEmptyParts);
#else
    return _string.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
#endif
}
static QStringList GetFileList(const QDir &dir)
{
    return dir.entryList(QStringList{"*", "*.*"}, QDir::Hidden | QDir::Files);
}
static bool FileExistsIn(const QDir &dir, const QString &fileName)
{
    return GetFileList(dir).contains(fileName);
}

static QString JsonToString(const QJsonObject &json,
                            QJsonDocument::JsonFormat format = QJsonDocument::JsonFormat::Indented)
{
    QJsonDocument doc;
    doc.setObject(json);
    return doc.toJson(format);
}

static bool StringToFile(const QString &text, const QString &targetpath)
{
    bool override = false;
    {
        QFileInfo info(targetpath);
        override = info.exists();
        if (!override && !info.dir().exists())
            info.dir().mkpath(info.dir().path());
    }
    QSaveFile f{targetpath};
    f.open(QIODevice::WriteOnly);
    f.write(text.toUtf8());
    f.commit();
    return override;
}

namespace Mihomo::Core {
std::pair<bool, std::optional<QString>> KernelInstance::CheckAndSetCoreExecutableState(
    const QString &vCorePath)
{
#ifdef Q_OS_UNIX
    // For Linux/macOS users: if they cannot execute the core,
    // then we shall grant the permission to execute it.

    QFile coreFile(vCorePath);

    if (!coreFile.permissions().testFlag(QFileDevice::ExeUser)) {
        qDebug() << "Core file not executable. Trying to enable.";
        const auto result = coreFile.setPermissions(
            coreFile.permissions().setFlag(QFileDevice::ExeUser));
        if (!result) {
            qDebug() << "Failed to enable executable permission.";
            const auto message
                = tr("Core file is lacking executable permission for the current user.") + //
                  tr("Application tried to set, but failed because permission denied.");
            return {false, message};
        } else {
            qDebug() << "Core executable permission set.";
        }
    } else {
        qDebug() << "Core file is executable.";
    }
    return {true, std::nullopt};
#endif

    // For Windows and other users: just skip this check.
    qDebug() << "Skipped check and set core executable state.";
    return {true, tr("Check is skipped")};
}

bool KernelInstance::ValidateKernel(const QString &vCorePath, QString *message)
{
    QFile coreFile(vCorePath);

    if (!coreFile.exists()) {
        qDebug() << "core file cannot be found.";
        *message = tr("core executable not found.");
        return false;
    }

    // Use open() here to prevent `executing` a folder, which may have the
    // same name as the V2Ray core.
    if (!coreFile.open(QFile::ReadOnly)) {
        qDebug() << "core file cannot be opened, possibly be a folder?";
        *message = tr(
            "core file cannot be opened, please ensure there's a file instead of a folder.");
        return false;
    }

    coreFile.close();

    // Get Core ABI.
    const auto [abi, err] = abi::deduceKernelABI(vCorePath);
    if (err) {
        qInfo() << "Core ABI deduction failed: " + ACCESS_OPTIONAL_VALUE(err);
        *message = ACCESS_OPTIONAL_VALUE(err);
        return false;
    }
    qInfo() << "Core ABI: " + abi::abiToString(ACCESS_OPTIONAL_VALUE(abi));

    // Get Compiled ABI
    auto compiledABI = abi::COMPILED_ABI_TYPE;
    qInfo() << "Host ABI: " + abi::abiToString(compiledABI);

    // Check ABI Compatibility.
    switch (abi::checkCompatibility(compiledABI, ACCESS_OPTIONAL_VALUE(abi))) {
    case abi::ABI_NOPE: {
        qInfo() << "Host is incompatible with core";
        *message = tr("core is incompatible with your platform.\r\n"    //
                      "Expected core ABI is %1, but got actual %2.\r\n" //
                      "Maybe you have downloaded the wrong core?")
                       .arg(abi::abiToString(compiledABI),
                            abi::abiToString(ACCESS_OPTIONAL_VALUE(abi)));
        return false;
    }
    case abi::ABI_MAYBE: {
        qInfo() << "WARNING: Host maybe incompatible with core";
        break;
    }
    case abi::ABI_PERFECT: {
        qInfo() << "Host is compatible with core";
        break;
    }
    }

    // Check executable permissions.
    const auto [isExecutableOk, strExecutableErr] = CheckAndSetCoreExecutableState(vCorePath);
    if (!isExecutableOk) {
        *message = strExecutableErr.value_or("");
        return false;
    }

    // Check if V2Ray core returns a version number correctly.
    QProcess proc;
#ifdef Q_OS_WIN32
    // nativeArguments are required for Windows platform, without a
    // reason...
    proc.setProcessChannelMode(QProcess::MergedChannels);
    proc.setProgram(vCorePath);
    proc.setNativeArguments("-v");
    proc.start();
#else
    proc.start(vCorePath, {"-v"});
#endif
    proc.waitForStarted();
    proc.waitForFinished();
    auto exitCode = proc.exitCode();

    if (exitCode != 0) {
        qDebug() << "Core failed with an exit code: " + QSTRN(exitCode);
        *message = tr("core failed with an exit code: ") + QSTRN(exitCode);
        return false;
    }

    QString output = proc.readAllStandardOutput();
    qInfo() << "Core output: " + SplitLines(output).join(";");

    if (SplitLines(output).isEmpty()) {
        *message = tr("core returns empty string.");
        return false;
    }

    *message = SplitLines(output).first();
    return true;
}

bool KernelInstance::ValidateConfig(const QString &vCorePath, const QString &path)
{
    QString checkResult;
    if (ValidateKernel(vCorePath, &checkResult)) {
        qDebug() << "Core version: " + checkResult;
        // Append assets location env.
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        //env.insert("V2RAY_LOCATION_ASSET", assetsPath);
        //
        QProcess process;
        process.setProcessEnvironment(env);
        qDebug() << "Starting core with test options";
        process.start(vCorePath,
                      QStringList{"-t", "-f", path},
                      QIODevice::ReadWrite | QIODevice::Text);
        process.waitForFinished();

        if (process.exitCode() != 0) {
            QString output = QString(process.readAllStandardOutput());
            QMessageBox::warning(nullptr, tr("Configuration Error"), output);
            return false;
        } else {
            qDebug() << "Config file check passed.";
            return true;
        }
    } else {
        QMessageBox::warning(nullptr,
                             tr("Cannot start Core"),                                //
                             tr("core settings is incorrect.") + NEWLINE + NEWLINE + //
                                 tr("The error is: ") + NEWLINE + checkResult);
        return false;
    }
}

KernelInstance::KernelInstance(QObject *parent)
    : QObject(parent)
{
    vProcess = new QProcess();
    connect(vProcess, &QProcess::readyReadStandardOutput, this, [&]() {
        emit OnProcessOutputReadyRead(vProcess->readAllStandardOutput().trimmed());
    });
    connect(vProcess, &QProcess::stateChanged, [&](QProcess::ProcessState state) {
        qDebug() << "kernel process status changed: " + QVariant::fromValue(state).toString();

        // If Kernel crashed AFTER we start it.
        if (KernelStarted && state == QProcess::NotRunning) {
            qInfo() << "kernel crashed.";
            StopConnection();
            emit OnProcessErrored("kernel crashed.");
        }
    });
    KernelStarted = false;
}

KernelInstance &KernelInstance::getInstance()
{
    static KernelInstance instance;
    return instance;
}
std::optional<QString> KernelInstance::StartConnection(const QString &vCorePath,
                                                       const QString &configPath)
{
    if (KernelStarted) {
        qInfo() << "Status is invalid, expect STOPPED when calling StartConnection";
        return tr("Core Status is invalid");
    }

    if (ValidateConfig(vCorePath, configPath)) {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

        vProcess->setProcessEnvironment(env);
        vProcess->start(vCorePath, {"-f", configPath}, QIODevice::ReadWrite | QIODevice::Text);
        vProcess->waitForStarted();
        qDebug() << "core started.";
        KernelStarted = true;
        return std::nullopt;
    } else {
        KernelStarted = false;
        return "V2Ray核心启动失败";
    }
}

void KernelInstance::StopConnection()
{
    // Set this to false BEFORE close the Process, since we need this flag
    // to capture the real kernel CRASH
    KernelStarted = false;
    vProcess->kill();
    vProcess->close();
    // Block until V2Ray core exits
    // Should we use -1 instead of waiting for 30secs?
    vProcess->waitForFinished();
}

KernelInstance::~KernelInstance()
{
    if (KernelStarted) {
        StopConnection();
    }
    delete vProcess;
}

} // namespace Mihomo::Core
