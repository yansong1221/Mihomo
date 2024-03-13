#pragma once
#include <QQuickWindow>
namespace Clash::Meta::GUI {
class Window : public QQuickWindow
{
    Q_OBJECT
public:
    using QQuickWindow::QQuickWindow;

    Q_PROPERTY(bool stayTop READ stayTop WRITE setStayTop NOTIFY stayTopChanged);

    bool stayTop() const;
    void setStayTop(bool state);
signals:
    void stayTopChanged(bool state);

private:
    bool stayTop_ = false;
};
} // namespace Clash::Meta::GUI