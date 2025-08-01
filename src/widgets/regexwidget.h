#ifndef REGEXWIDGET_H
#define REGEXWIDGET_H

#include <QWidget>

namespace Ui {
class RegexWidget;
}

class RegexWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegexWidget(QWidget *parent = nullptr);
    ~RegexWidget();

private:
    Ui::RegexWidget *ui;
};

#endif // REGEXWIDGET_H
