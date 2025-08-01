#ifndef TEXTDIFFWIDGET_H
#define TEXTDIFFWIDGET_H

#include <QWidget>

namespace Ui {
class TextDiffWidget;
}

class TextDiffWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TextDiffWidget(QWidget *parent = nullptr);
    ~TextDiffWidget();

private:
    Ui::TextDiffWidget *ui;
};

#endif // TEXTDIFFWIDGET_H
