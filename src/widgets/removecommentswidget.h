#ifndef REMOVECOMMENTSWIDGET_H
#define REMOVECOMMENTSWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class RemoveCommentsWidget;
}

class RemoveCommentsWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit RemoveCommentsWidget(QWidget *parent = nullptr);
    ~RemoveCommentsWidget();

private:
    Ui::RemoveCommentsWidget *ui;
};

#endif // REMOVECOMMENTSWIDGET_H
