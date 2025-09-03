#include "customwidget.h"

CustomWidget::CustomWidget(QWidget *parent)
    : QWidget{parent}
{}

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
