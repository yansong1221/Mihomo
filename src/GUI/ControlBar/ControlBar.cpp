#include "ControlBar.h"
#include "Configurator/ClashConfigurator.h"
#include "Core/KernelInteractions.hpp"
#include "ui_ControlBar.h"
#include <QTime>
#include <QTimer>

namespace Clash::Meta::GUI {
ControlBar::ControlBar(QWidget *parent /*= nullptr*/)
    : QFrame(parent)
    , ui(new Ui::ControlBar)
{
    ui->setupUi(this);

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ControlBar::updateStatus);
    timer->setInterval(std::chrono::seconds(1));
    timer->start();

    connect(&Core::KernelInstance::instance(),
            &Core::KernelInstance::OnProcessErrored,
            this,
            [this](const QString &errMessage) {
                updateStatus();
                QTimer::singleShot(1000, this, &ControlBar::tryStartKernel);
            });

    updateStatus();
}

ControlBar::~ControlBar()
{
    delete ui;
}

void ControlBar::updateStatus()
{
    if (!Core::KernelInstance::instance().KernelStarted) {
        ui->kernelRunTime->clear();
        ui->kernelStatus->setIcon(QIcon::fromTheme("kernel-stopped"));
        ui->kernelStatus->setText(tr("Stopped"));
        return;
    }
    auto duration = Core::KernelInstance::instance().ElapsedTimer().durationElapsed();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    auto time = QTime::fromMSecsSinceStartOfDay(ms);
    ui->kernelRunTime->setText(time.toString("hh:mm:ss"));
    ui->kernelStatus->setIcon(QIcon::fromTheme("kernel-running"));
    ui->kernelStatus->setText(tr("Running"));
}
void ControlBar::tryStartKernel()
{
    if (Core::KernelInstance::instance().KernelStarted)
        return;

    restartKernel();
}

void ControlBar::restartKernel()
{
    Core::KernelInstance::instance().StopConnection();
    Config::ClashConfigurator::instance().toYamlFile();
    Core::KernelInstance::instance().StartConnection(Config::ClashConfigurator::executableFilePath(),
                                                     Config::ClashConfigurator::fullConfigPath());
}

} // namespace Clash::Meta::GUI
