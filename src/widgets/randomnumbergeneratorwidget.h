#ifndef RANDOMNUMBERGENERATORWIDGET_H
#define RANDOMNUMBERGENERATORWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class RandomNumberGeneratorWidget;
}

class RandomNumberGeneratorWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit RandomNumberGeneratorWidget(QWidget *parent = nullptr);
    ~RandomNumberGeneratorWidget();
    bool canBasicEdit() const override;
    void deleteText() override;
    void deleteAllText() override;

private:
    Ui::RandomNumberGeneratorWidget *ui;

private slots:
    void generate();
};

#endif // RANDOMNUMBERGENERATORWIDGET_H
