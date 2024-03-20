#pragma once
#include <QObject>
#include <qcoro/qcorotask.h>

namespace Clash::Meta::Config {
class ClashRuntimeConfigure : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged);

    explicit ClashRuntimeConfigure(QObject *parent = nullptr);
    ~ClashRuntimeConfigure();

    static ClashRuntimeConfigure &instance();

public:
    QString mode() const { return mode_; }
    void setMode(const QString &mode) { __setMode(mode); }

private:
    QCoro::Task<> __setMode(const QString &mode);
    QCoro::Task<> __refreshConfigure();
signals:
    void modeChanged(const QString& mode);

private:
    QString mode_;
};

} // namespace Clash::Meta::Config