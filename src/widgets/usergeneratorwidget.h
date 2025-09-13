#ifndef USERGENERATORWIDGET_H
#define USERGENERATORWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class UserGeneratorWidget;
}

class UserGeneratorWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit UserGeneratorWidget(QWidget *parent = nullptr);
    ~UserGeneratorWidget();
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    void save() override;
    void saveAs() override;

private:
    Ui::UserGeneratorWidget *ui;

private slots:
    void generate();

};

#endif // USERGENERATORWIDGET_H
