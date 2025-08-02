#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::clicked, this, &MainWindow::showWidget);

    ui->splitter->setStretchFactor(1, 1);
    ui->listWidget->setFrameShape(QFrame::NoFrame);
    addMenuItem(tr("Formatters"), QIcon(), false);
    addMenuItem(tr("JSON"), QIcon());
    addMenuItem(tr("SQL"), QIcon());
    addMenuItem(tr("XML"), QIcon());
    addMenuItem(tr("HTML"), QIcon());
    addMenuItem(tr("Generators"), QIcon(), false);
    addMenuItem(tr("Hash"), QIcon());
    addMenuItem(tr("Lorem ipsum"), QIcon());
    addMenuItem(tr("UUID"), QIcon());
    addMenuItem(tr("Random text"), QIcon());
    addMenuItem(tr("Qr code"), QIcon());
    addMenuItem(tr("Text"), QIcon(), false);
    addMenuItem(tr("Regex"), QIcon());
    addMenuItem(tr("Text diff"), QIcon());
    addMenuItem(tr("Markdown"), QIcon());
    addMenuItem(tr("XML Scheme Validator"), QIcon());
    addMenuItem(tr("JSON Schema Validator"), QIcon());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showWidget(const QModelIndex& index)
{
    const QString text = index.data().toString();
    if(menuIndexMap.contains(text))
    {
        ui->stackedWidget->setCurrentIndex(menuIndexMap.value(text));
    }
}

void MainWindow::addMenuItem(const QString &text, const QIcon &icon, bool enabled)
{
    static int index = 0;
    QListWidgetItem* item = new QListWidgetItem(text, ui->listWidget);
    item->setIcon(icon);
    if(!enabled)
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    else
        menuIndexMap[text] = index++;
    ui->listWidget->addItem(item);
}
