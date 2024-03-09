#pragma once
#include <QAbstractButton>
#include <QFrame>

namespace Ui {
class WindowBar;
}

namespace Clash::Meta::GUI {
class WindowBar : public QFrame
{
    Q_OBJECT
public:
    explicit WindowBar(QWidget *parent = nullptr);
    ~WindowBar();

public:
    void setup(QWidget *widget);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void reloadMaxButtonIcon();
    void reloadMinButtonIcon();
    void reloadCloseButtonIcon();
    void reloadStaysOnTopButtonIcon();

    void reloadIcon();

private:
    Ui::WindowBar *ui;
    QWidget *hostWidget_ = nullptr;
};
} // namespace Clash::Meta::GUI