#ifndef RANDOMTEXTGENERATORWIDGET_H
#define RANDOMTEXTGENERATORWIDGET_H

#include <QWidget>

namespace Ui {
class RandomTextGeneratorWidget;
}

class RandomTextGeneratorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RandomTextGeneratorWidget(QWidget *parent = nullptr);
    ~RandomTextGeneratorWidget();

private:
    Ui::RandomTextGeneratorWidget *ui;
};

#endif // RANDOMTEXTGENERATORWIDGET_H
