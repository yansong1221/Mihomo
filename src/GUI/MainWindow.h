#pragma once
#include "Components/ObjectEvent.h"
#include <QWidget>

namespace Ui {
class MainWindow;
}

namespace Clash::Meta::GUI {
class TabButtonModel;
class MainWindow : public Components::ObjectEvent<QWidget>
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void retranslateUi() override;

private:
    ::Ui::MainWindow *ui;
    TabButtonModel *tabButtonModel_;
};
} // namespace Clash::Meta::GUI