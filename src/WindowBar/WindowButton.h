#pragma once
#include <QPushButton>

class WindowButton : public QPushButton
{
    Q_OBJECT
public:
    explicit WindowButton(QWidget *parent = nullptr);
    ~WindowButton();
signals:
    void doubleClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QColor hoverColor_;
    bool isHovered_ = false;
};