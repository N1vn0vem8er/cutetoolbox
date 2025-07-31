#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::doubleClicked, this, &MainWindow::showWidget);
    for(const auto& i : std::as_const(listOption))
    {
        QListWidgetItem* item = new QListWidgetItem(i.first, ui->listWidget);
        item->setIcon(QIcon(i.second));
        ui->listWidget->addItem(item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showWidget(const QModelIndex& index)
{
    if(ui->stackedWidget->count() > index.row())
    {
        ui->stackedWidget->setCurrentIndex(index.row());
    }
}
