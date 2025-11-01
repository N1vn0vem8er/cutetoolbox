#include "csvdatatableinfo.h"
#include "src/widgets/ui_csvdatatableinfo.h"

CSVDataTableInfo::CSVDataTableInfo(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::CSVDataTableInfo)
{
    ui->setupUi(this);
    setName(tr("Data Table Info"));
}

CSVDataTableInfo::~CSVDataTableInfo()
{
    delete ui;
}
