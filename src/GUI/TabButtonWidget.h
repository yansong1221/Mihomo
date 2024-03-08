#pragma once
#include <QListView>

namespace Clash::Meta::GUI {
class TabButtonWidget : public QListView
{
    Q_OBJECT
public:
    explicit TabButtonWidget(QWidget *parent = nullptr);
    ~TabButtonWidget();

protected:
    void paintEvent(QPaintEvent *e) override;
};
} // namespace Clash::Meta::GUI