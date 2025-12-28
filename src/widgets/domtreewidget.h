#ifndef DOMTREEWIDGET_H
#define DOMTREEWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class DomTreeWidget;
}

class DomTreeWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit DomTreeWidget(QWidget *parent = nullptr);
    ~DomTreeWidget();

private:
    Ui::DomTreeWidget *ui;
};

#endif // DOMTREEWIDGET_H
