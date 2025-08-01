#ifndef HASHWIDGET_H
#define HASHWIDGET_H

#include <QWidget>

namespace Ui {
class HashWidget;
}

class HashWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HashWidget(QWidget *parent = nullptr);
    ~HashWidget();

private:
    Ui::HashWidget *ui;
};

#endif // HASHWIDGET_H
