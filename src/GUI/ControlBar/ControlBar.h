#pragma once
#include <QFrame>
namespace Ui {
class ControlBar;
}
namespace Clash::Meta::GUI {

class ControlBar : public QFrame
{
    Q_OBJECT
public:
    explicit ControlBar(QWidget *parent = nullptr);
    ~ControlBar();

public:
    void tryStartKernel();
    void restartKernel();

private:
    void updateStatus();

private:
    Ui::ControlBar *ui;
};
} // namespace Clash::Meta::GUI