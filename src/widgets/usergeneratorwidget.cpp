#include "usergeneratorwidget.h"
#include "src/widgets/ui_usergeneratorwidget.h"

UserGeneratorWidget::UserGeneratorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::UserGeneratorWidget)
{
    ui->setupUi(this);
}

UserGeneratorWidget::~UserGeneratorWidget()
{
    delete ui;
}

bool UserGeneratorWidget::canSaveFiles() const
{
    return true;
}

bool UserGeneratorWidget::canBasicEdit() const
{
    return true;
}

void UserGeneratorWidget::save()
{

}

void UserGeneratorWidget::saveAs()
{

}

void UserGeneratorWidget::generate()
{

}
