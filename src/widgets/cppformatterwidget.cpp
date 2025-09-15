#include "cppformatterwidget.h"
#include "src/widget/ui_cppformatterwidget.h"

CppFormatterWidget::CppFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::CppFormatterWidget)
{
    ui->setupUi(this);

}

CppFormatterWidget::~CppFormatterWidget()
{
    delete ui;
}

bool CppFormatterWidget::canOpenFiles() const
{
    return true;
}

bool CppFormatterWidget::canSaveFiles() const
{
    return true;
}

bool CppFormatterWidget::canBasicEdit() const
{
    return true;
}

bool CppFormatterWidget::canChangeFont() const
{
    return true;
}

void CppFormatterWidget::save()
{

}

void CppFormatterWidget::saveAs()
{

}

void CppFormatterWidget::open()
{

}

void CppFormatterWidget::increaseFontSize()
{

}

void CppFormatterWidget::decreaseFontSize()
{

}

void CppFormatterWidget::setFontSize()
{

}

void CppFormatterWidget::resetFontSize()
{

}

void CppFormatterWidget::setFont()
{

}

void CppFormatterWidget::format()
{

}
