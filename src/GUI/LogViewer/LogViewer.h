#pragma once
#include <QWidget>

namespace Ui {
class LogViewer;
}

namespace Clash::Meta::GUI::Log {

class LogViewer : public QWidget
{
    Q_OBJECT
public:
    explicit LogViewer(QWidget *parent = nullptr);
    ~LogViewer();

private:
    Ui::LogViewer *ui;
};

} // namespace Clash::Meta::GUI::Log