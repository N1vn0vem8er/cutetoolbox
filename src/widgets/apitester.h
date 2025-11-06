#ifndef APITESTER_H
#define APITESTER_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class ApiTester;
}

class ApiTester : public CustomWidget
{
    Q_OBJECT

public:
    explicit ApiTester(QWidget *parent = nullptr);
    ~ApiTester();

private:
    Ui::ApiTester *ui;
};

#endif // APITESTER_H
