#include "customwidget.h"

CustomWidget::CustomWidget(QWidget *parent)
    : QWidget{parent}
{}

CustomWidget::CustomWidget(const QString &name, QWidget *parent)
{
    this->name = name;
}

bool CustomWidget::canOpenFiles() const
{
    return false;
}

bool CustomWidget::canSaveFiles() const
{
    return false;
}

bool CustomWidget::canBasicEdit() const
{
    return false;
}

bool CustomWidget::canChangeFont() const
{
    return false;
}

void CustomWidget::save()
{

}

void CustomWidget::saveAs()
{

}

void CustomWidget::open()
{

}

void CustomWidget::close()
{

}

void CustomWidget::increaseFontSize()
{

}

void CustomWidget::decreaseFontSize()
{

}

void CustomWidget::setFontSize()
{

}

void CustomWidget::resetFontSize()
{

}

void CustomWidget::setFont()
{

}

QString CustomWidget::getName() const
{
    return name;
}

void CustomWidget::setName(const QString &name)
{
    this->name = name;
}

QString CustomWidget::getOpenedFileName() const
{
    return "";
}
