#pragma once
#include <QPushButton>
namespace Clash::Meta::GUI {

class WindowButton : public QPushButton
{
    Q_OBJECT
public:
    explicit WindowButton(QWidget *parent = nullptr);
    ~WindowButton();

signals:
    void doubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

} // namespace Clash::Meta::GUI