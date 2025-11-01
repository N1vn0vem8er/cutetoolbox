#ifndef CSVDATATABLEINFO_H
#define CSVDATATABLEINFO_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class CSVDataTableInfo;
}

class CSVDataTableInfo : public CustomWidget
{
    Q_OBJECT

public:
    explicit CSVDataTableInfo(QWidget *parent = nullptr);
    ~CSVDataTableInfo();

private:
    Ui::CSVDataTableInfo *ui;
};

#endif // CSVDATATABLEINFO_H
