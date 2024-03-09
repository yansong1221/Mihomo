#include "MainWindow.h"
#include "Configurator/ClashConfigurator.h"
#include "Connections/ConnectionsModel.h"
#include "Core/APIClient.h"
#include "Core/KernelInteractions.hpp"
#include "General/GeneralWidget.h"
#include "WindowBar/windowbutton.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <QTime>
#include <QTimer>
#include <QTranslator>
#include <QWKWidgets/widgetwindowagent.h>
#include <QWindow>

namespace Clash::Meta::GUI {

MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    : ObjectEvent(parent)
    , ui(new ::Ui::MainWindow)
{
    ui->setupUi(this);

    ui->windowBar->setup(this);

    auto model = new GUI::Connections::ConnectionsModel(this);
    qDebug() << QIcon::themeSearchPaths();

    QString translationsDir = qApp->applicationDirPath() + "/translations";
    QList<QTranslator *> translators;
    auto names = QStringList() << "qtbase_"
                               << "qt_";
    for (const auto &v : names) {
        QTranslator *trans = new QTranslator();
        if (!trans->load(v + "zh_CN", translationsDir)) {
            trans->deleteLater();
            continue;
        }
        qApp->installTranslator(trans);
    }

    auto generalWidget = new GeneralWidget(this);
    ui->stackedWidget->addWidget(generalWidget);

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateStatus);
    timer->setInterval(std::chrono::seconds(1));
    timer->start();

    connect(&Core::KernelInstance::instance(),
            &Core::KernelInstance::OnProcessErrored,
            this,
            [this](const QString &errMessage) {
                updateStatus();
                QTimer::singleShot(1000, this, &MainWindow::tryStartKernel);
            });

    updateStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::retranslateUi()
{
    ui->retranslateUi(this);
}

void MainWindow::updateStatus()
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

void MainWindow::tryStartKernel()
{
    if (Core::KernelInstance::instance().KernelStarted)
        return;

    restartKernel();
}

void MainWindow::restartKernel()
{
    Core::KernelInstance::instance().StopConnection();
    Config::ClashConfigurator::instance().toYamlFile();
    Core::KernelInstance::instance().StartConnection(Config::ClashConfigurator::executableFilePath(),
                                                     Config::ClashConfigurator::fullConfigPath());
}

} // namespace Clash::Meta::GUI