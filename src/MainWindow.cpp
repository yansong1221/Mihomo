#include "MainWindow.h"
#include "Connections/ConnectionsModel.h"
#include "Core/APIClient.h"
#include "WindowBar/windowbutton.h"
#include "ui_MainWindow.h"
#include <QTimer>
#include <QWKWidgets/widgetwindowagent.h>

namespace Mihomo {

static inline void emulateLeaveEvent(QWidget *widget)
{
    Q_ASSERT(widget);
    if (!widget) {
        return;
    }
    QTimer::singleShot(0, widget, [widget]() {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        const QScreen *screen = widget->screen();
#else
        const QScreen *screen = widget->windowHandle()->screen();
#endif
        const QPoint globalPos = QCursor::pos(screen);
        if (!QRect(widget->mapToGlobal(QPoint{0, 0}), widget->size()).contains(globalPos)) {
            QCoreApplication::postEvent(widget, new QEvent(QEvent::Leave));
            if (widget->testAttribute(Qt::WA_Hover)) {
                const QPoint localPos = widget->mapFromGlobal(globalPos);
                const QPoint scenePos = widget->window()->mapFromGlobal(globalPos);
                static constexpr const auto oldPos = QPoint{};
                const Qt::KeyboardModifiers modifiers = QGuiApplication::keyboardModifiers();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))
                const auto event = new QHoverEvent(QEvent::HoverLeave,
                                                   scenePos,
                                                   globalPos,
                                                   oldPos,
                                                   modifiers);
                Q_UNUSED(localPos);
#elif (QT_VERSION >= QT_VERSION_CHECK(6, 3, 0))
                const auto event =  new QHoverEvent(QEvent::HoverLeave, localPos, globalPos, oldPos, modifiers);
                Q_UNUSED(scenePos);
#else
                const auto event =  new QHoverEvent(QEvent::HoverLeave, localPos, oldPos, modifiers);
                Q_UNUSED(scenePos);
#endif
                QCoreApplication::postEvent(widget, event);
            }
        }
    });
}

MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    : ObjectEvent(parent)
    , ui(new ::Ui::MainWindow)
{
    ui->setupUi(this);

    ui->closeButton->setHoverColor(QColor(232, 17, 35));
    ui->maxButton->setWatchWidget(this);
    ui->alwaysTopButton->setWatchWidget(this);
    ui->alwaysTopButton->setVisible(false);

    auto windowAgent = new QWK::WidgetWindowAgent(this);
    windowAgent->setup(this);

    windowAgent->setTitleBar(ui->windowBar);
#ifndef Q_OS_MAC
    windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, ui->iconButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, ui->minButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, ui->maxButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Close, ui->closeButton);
#endif
    windowAgent->setHitTestVisible(ui->alwaysTopButton);

#ifdef Q_OS_MAC
    windowAgent->setSystemButtonAreaCallback([](const QSize &size) {
        static constexpr const int width = 75;
        return QRect(QPoint(size.width() - width, 0), QSize(width, size.height())); //
    });
#endif

    // 3. Adds simulated mouse events to the title bar buttons
#ifdef Q_OS_WINDOWS
    // Emulate Window system menu button behaviors
    connect(ui->iconButton, &QAbstractButton::clicked, windowAgent, [this, windowAgent] {
        ui->iconButton->setProperty("double-click-close", false);

        // Pick a suitable time threshold
        QTimer::singleShot(75, windowAgent, [this, windowAgent]() {
            if (ui->iconButton->property("double-click-close").toBool())
                return;
            windowAgent->showSystemMenu(
                ui->iconButton->mapToGlobal(QPoint{0, ui->iconButton->height()}));
        });
    });
    connect(ui->minButton, &QAbstractButton::clicked, this, &QWidget::showMinimized);
    connect(ui->maxButton, &QAbstractButton::clicked, this, [this]() {
        if (this->isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
    });
    connect(ui->alwaysTopButton, &QAbstractButton::clicked, this, [this]() {
        if (this->windowFlags().testFlag(Qt::WindowStaysOnTopHint)) {
            this->setWindowFlag(Qt::WindowStaysOnTopHint, false);
        } else {
            this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
            this->show();
        }
    });
    connect(ui->closeButton, &QAbstractButton::clicked, this, &QWidget::close);
#endif

    auto model = new GUI::Connections::ConnectionsModel(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::retranslateUi()
{
    ui->retranslateUi(this);
}
} // namespace Mihomo