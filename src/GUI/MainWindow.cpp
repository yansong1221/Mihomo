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

    ui->controlBar->tryStartKernel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::retranslateUi()
{
    ui->retranslateUi(this);
}

} // namespace Clash::Meta::GUI