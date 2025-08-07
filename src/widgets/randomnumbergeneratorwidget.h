#ifndef RANDOMNUMBERGENERATORWIDGET_H
#define RANDOMNUMBERGENERATORWIDGET_H

#include <QWidget>

namespace Ui {
class RandomNumberGeneratorWidget;
}

class RandomNumberGeneratorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RandomNumberGeneratorWidget(QWidget *parent = nullptr);
    ~RandomNumberGeneratorWidget();

private:
    Ui::RandomNumberGeneratorWidget *ui;

private slots:
    void generate();
};

#endif // RANDOMNUMBERGENERATORWIDGET_H
