#include "desktopcreatorwidget.h"
#include "src/widgets/ui_desktopcreatorwidget.h"

DesktopCreatorWidget::DesktopCreatorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DesktopCreatorWidget)
{
    ui->setupUi(this);
}

DesktopCreatorWidget::~DesktopCreatorWidget()
{
    delete ui;
}
