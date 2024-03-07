#pragma once
#include "Components/ObjectEvent.h"
#include <QWidget>

namespace Ui {
class MainWindow;
}

namespace Mihomo {
class MainWindow : public Components::ObjectEvent<QWidget>
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void retranslateUi() override;
    void changeEvent(QEvent *event) override;

private:
    ::Ui::MainWindow *ui;
};
} // namespace Mihomo