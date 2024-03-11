#include "GeneralWidget.h"
#include "Components/TranslateHelper.h"
#include "qtmaterialfab.h"
#include "ui_GeneralWidget.h"
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QtMWidgets/TextLabel>
#include <QtMaterialCanvas.h>
#include <qtmaterialiconbutton.h>
#include <qtmaterialswitch.h>

namespace Clash::Meta::GUI {

class TableViewCellHelper
{
public:
    TableViewCellHelper(QtMWidgets::TableViewCell *cell)
        : layout_(new QHBoxLayout(cell->accessoryWidget()))
        , cell_(cell)
    {
        layout_->setContentsMargins(0, 0, 0, 0);
        layout_->setSpacing(0);
    }

public:
    QLabel *addLabel(const QString &text = "")
    {
        auto textLable = new QLabel(cell_);
        textLable->setText(text);
        layout_->addWidget(textLable);
        return textLable;
    }
    QLabel *addLabel(const Components::TranslateHelper::TextCreator &creator)
    {
        auto textLable = new QLabel(cell_);
        layout_->addWidget(textLable);
        Components::TranslateHelper::instance().addObject(textLable, creator);
        return textLable;
    }

    QAbstractButton *addButton(const QString &text = QString())
    {
        auto btn = new QtMaterialFlatButton(cell_);
        btn->setText(text);
        btn->setType(Material::ButtonRaised);
        layout_->addWidget(btn);
        return btn;
    }
    QAbstractButton *addSwitchButton()
    {
        auto btn = new QtMaterialSwitch(cell_);
        layout_->addWidget(btn);
        return btn;
    }
    QAbstractButton *addIconButton(const QString &normalTheme, const QString &checkedTheme = "")
    {
        auto btn = new QPushButton(cell_);
        btn->setIcon(QIcon::fromTheme(normalTheme));
        layout_->addWidget(btn);

        if (!checkedTheme.isEmpty()) {
            btn->setCheckable(true);
            QObject::connect(btn,
                             &QAbstractButton::toggled,
                             btn,
                             [btn, checkedTheme, normalTheme](bool checked) {
                                 btn->setIcon(checked ? QIcon::fromTheme(checkedTheme)
                                                      : QIcon::fromTheme(normalTheme));
                             });
        }
        return btn;
    }
    void addSpacerItem()
    {
        layout_->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Expanding));
    }

private:
    QHBoxLayout *layout_;
    QtMWidgets::TableViewCell *cell_;
};

GeneralWidget::GeneralWidget(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , ui(new Ui::GeneralWidget)
{
    ui->setupUi(this);
    auto section = new QtMWidgets::TableViewSection(this);
    section->setHighlightCellOnClick(true);

    {
        auto cell = new QtMWidgets::TableViewCell(this);

        TableViewCellHelper helper(cell);

        helper.addLabel(TR_CREATOR("Port"));

        helper.addSpacerItem();

        helper.addIconButton("cmd");
        helper.addIconButton("port-random-disabled", "port-random");

        auto btn = helper.addButton("7890");

        section->addCell(cell);
    }

    {
        auto cell = new QtMWidgets::TableViewCell(this);

        TableViewCellHelper helper(cell);
        helper.addLabel(TR_CREATOR("Allow Lan"));
        helper.addSpacerItem();
        helper.addSwitchButton();

        section->addCell(cell);
    }

    {
        auto cell = new QtMWidgets::TableViewCell(this);

        TableViewCellHelper helper(cell);
        helper.addLabel(TR_CREATOR("IPv6"));
        helper.addSpacerItem();
        helper.addSwitchButton();

        section->addCell(cell);
    }

    {
        auto cell = new QtMWidgets::TableViewCell(this);

        TableViewCellHelper helper(cell);
        helper.addLabel(TR_CREATOR("TUN Mode"));
        auto btn = helper.addIconButton("tips-info");
        btn->setToolTip("1111111111111");
        btn->setToolTipDuration(-1);
        btn->setEnabled(false);

        helper.addSpacerItem();
        helper.addSwitchButton();

        section->addCell(cell);
    }

    {
        auto cell = new QtMWidgets::TableViewCell(this);

        TableViewCellHelper helper(cell);
        helper.addLabel(TR_CREATOR("System Proxy"));
        helper.addSpacerItem();
        helper.addSwitchButton();

        section->addCell(cell);
    }

    {
        auto cell = new QtMWidgets::TableViewCell(this);

        TableViewCellHelper helper(cell);
        helper.addLabel(TR_CREATOR("Start with System"));
        helper.addSpacerItem();
        helper.addSwitchButton();

        section->addCell(cell);
    }
    ui->view->addSection(section);
    ui->view->setHighlightCellOnClick(true);
    ui->view->setIndicatorColor(Qt::red);
    ui->view->setBlurColor(Qt::red);
    ui->view->viewport()->setAttribute(Qt::WA_TranslucentBackground);
}

GeneralWidget::~GeneralWidget()
{
    delete ui;
}

} // namespace Clash::Meta::GUI
