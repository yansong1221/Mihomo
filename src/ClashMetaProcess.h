#pragma once
#include "Core/KernelInteractions.hpp"
#include <QProcess>

namespace Clash::Meta {
class ClashMetaProcess : public QObject
{
    Q_OBJECT
protected:
    explicit ClashMetaProcess(QObject *parent = nullptr);
    ~ClashMetaProcess();

public:
    static ClashMetaProcess &instance();

    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged);

    bool isRunning() const;
signals:
    void runningChanged(bool running);
public slots:
    void restart();

private:
    Core::KernelInstance *kernel_;
};
} // namespace Clash::Meta