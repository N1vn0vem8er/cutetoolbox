#include "resizablestackedwidget.h"

ResizableStackedWidget::ResizableStackedWidget(QWidget *parent) : QStackedWidget(parent)
{

}

QSize ResizableStackedWidget::sizeHint() const
{
    return currentWidget()->sizeHint();
}

QSize ResizableStackedWidget::minimumSizeHint() const
{
    return currentWidget()->minimumSizeHint();
}
