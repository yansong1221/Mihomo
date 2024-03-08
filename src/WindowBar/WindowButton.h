#pragma once
#include <QPushButton>
namespace Clash::Meta::GUI {

class WindowButton : public QPushButton
{
    Q_OBJECT
public:
    explicit WindowButton(QWidget *parent = nullptr);
    ~WindowButton();

    void setHoverColor(QColor color);
signals:
    void doubleClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QColor hoverColor_ = QColor(55, 55, 55);
    bool isHovered_ = false;
};

class WatchStateButton : public WindowButton
{
    Q_OBJECT
public:
    using WindowButton::WindowButton;

public:
    void setWatchWidget(QWidget *widget);
    QWidget *watchWidget() const;

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    virtual void onWatchWidgetEvent(QWidget *widget, QEvent *event) = 0;
    virtual void updateIcon() = 0;

private:
    QWidget *watchWidget_ = nullptr;
};

class MaximizeWindowButton : public WatchStateButton
{
    Q_OBJECT
public:
    using WatchStateButton::WatchStateButton;

    void setMaximizeIcon(const QIcon &icon);
    void setNormaledIcon(const QIcon &icon);

protected:
    virtual void onWatchWidgetEvent(QWidget *widget, QEvent *event) override;
    virtual void updateIcon() override;

private:
    QIcon maximizeIcon_ = QIcon(":/dark/images/maximize.png");
    QIcon normaledIcon_ = QIcon(":/dark/images/normaled.png");
};

class AlwaysTopWindowButton : public WatchStateButton
{
    Q_OBJECT
public:
    using WatchStateButton::WatchStateButton;

    void setCheckedIcon(const QIcon &icon);
    void setUnCheckedIcon(const QIcon &icon);

protected:
    virtual void onWatchWidgetEvent(QWidget *widget, QEvent *event) override;
    virtual void updateIcon() override;

private:
    QIcon checkedIcon_ = QIcon(":/dark/images/maximize.png");
    QIcon uncheckedIcon_ = QIcon(":/dark/images/normaled.png");
};

} // namespace Mihomo::GUI