#pragma once
#include <QWidget>

namespace Ui {
class GeneralWidget;
}

namespace Clash::Meta::GUI {
class GeneralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralWidget(QWidget *parent = nullptr);
    ~GeneralWidget();

private:
    Ui::GeneralWidget *ui;
};
} // namespace Clash::Meta::GUI