#include "WindowBar.h"
#include "ui_WindowBar.h"
#include <QApplication>
#include <QHoverEvent>
#include <QStyle>
#include <QTimer>
#include <QWKWidgets/widgetwindowagent.h>
#include <QWindow>

namespace Clash::Meta::GUI {

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

WindowBar::WindowBar(QWidget *parent /*= nullptr*/)
    : QFrame(parent)
    , ui(new Ui::WindowBar)
{
    ui->setupUi(this);
    reloadIcon();
}

WindowBar::~WindowBar()
{
    delete ui;
}

void WindowBar::setup(QWidget *widget)
{
    if (hostWidget_)
        return;

    hostWidget_ = widget;
    hostWidget_->installEventFilter(this);

    auto windowAgent = new QWK::WidgetWindowAgent(this);
    windowAgent->setup(widget);
    windowAgent->setTitleBar(this);

#ifndef Q_OS_MAC
    windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, ui->iconButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, ui->minButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, ui->maxButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Close, ui->closeButton);
#endif
    windowAgent->setHitTestVisible(ui->toolBar);

#ifdef Q_OS_MAC
    windowAgent->setSystemButtonAreaCallback([](const QSize &size) {
        static constexpr const int width = 75;
        return QRect(QPoint(size.width() - width, 0), QSize(width, size.height())); //
    });
#endif
    connect(ui->minButton, &QAbstractButton::clicked, hostWidget_, &QWidget::showMinimized);
    connect(ui->closeButton, &QAbstractButton::clicked, hostWidget_, &QWidget::close);
    connect(ui->maxButton, &QAbstractButton::clicked, hostWidget_, [this]() {
        if (hostWidget_->isMaximized()) {
            hostWidget_->showNormal();
        } else {
            hostWidget_->showMaximized();
        }
        emulateLeaveEvent(ui->maxButton);
    });

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

    connect(ui->staysOnTopButton, &QAction::triggered, this, [this](bool checked) {
        if (!checked) {
            hostWidget_->windowHandle()->setFlags(hostWidget_->windowFlags()
                                                  & ~Qt::WindowStaysOnTopHint);

        } else {
            hostWidget_->windowHandle()->setFlags(hostWidget_->windowFlags()
                                                  | Qt::WindowStaysOnTopHint);
        }
        reloadStaysOnTopButtonIcon();
    });
    ui->iconButton->setIcon(hostWidget_->windowIcon());
    connect(hostWidget_, &QWidget::windowIconChanged, ui->iconButton, &QAbstractButton::setIcon);
}

bool WindowBar::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == hostWidget_) {
        switch (event->type()) {
        case QEvent::WindowStateChange:
            reloadMaxButtonIcon();
            break;
        default:
            break;
        }
    }
    return QObject::eventFilter(watched, event);
}

void WindowBar::reloadMaxButtonIcon()
{
    if (!hostWidget_ || !hostWidget_->isMaximized()) {
        ui->maxButton->setIcon(
            QIcon::fromTheme("windowbar-max-button",
                             QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton)));
    } else {
        ui->maxButton->setIcon(
            QIcon::fromTheme("windowbar-normal-button",
                             QApplication::style()->standardIcon(QStyle::SP_TitleBarNormalButton)));
    }
}

void WindowBar::reloadMinButtonIcon()
{
    ui->minButton->setIcon(
        QIcon::fromTheme("windowbar-min-button",
                         QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton)));
}
void WindowBar::reloadCloseButtonIcon()
{
    ui->closeButton->setIcon(
        QIcon::fromTheme("windowbar-close-button",
                         QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton)));
}
void WindowBar::reloadIcon()
{
    reloadMaxButtonIcon();
    reloadMinButtonIcon();
    reloadCloseButtonIcon();
    reloadStaysOnTopButtonIcon();
}

void WindowBar::reloadStaysOnTopButtonIcon()
{
    if (ui->staysOnTopButton->isChecked())
        ui->staysOnTopButton->setIcon(QIcon::fromTheme("stays-on-top-checked"));
    else
        ui->staysOnTopButton->setIcon(QIcon::fromTheme("stays-on-top"));
}

} // namespace Clash::Meta::GUI