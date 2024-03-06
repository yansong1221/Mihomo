#pragma once
#include <QWidget>
namespace Mihomo {
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
} // namespace mihomo