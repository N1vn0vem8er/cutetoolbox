#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::clicked, this, &MainWindow::showWidget);
    connect(ui->actionSide_menu, &QAction::triggered, this, [&]{ui->listWidget->setVisible(ui->actionSide_menu->isChecked());});

    ui->actionSide_menu->setChecked(true);
    ui->splitter->setStretchFactor(1, 1);
    ui->listWidget->setFrameShape(QFrame::NoFrame);
    addMenuItem(tr("Coders and decoders"), QIcon(), false);
    addMenuItem(tr("HTML Coder"), QIcon());
    addMenuItem(tr("Base64"), QIcon());
    addMenuItem(tr("Url"), QIcon());
    addMenuItem(tr("GZip"), QIcon());
    addMenuItem(tr("Formatters"), QIcon(), false);
    addMenuItem(tr("JSON"), QIcon());
    addMenuItem(tr("SQL"), QIcon());
    addMenuItem(tr("XML"), QIcon());
    addMenuItem(tr("HTML"), QIcon());
    addMenuItem(tr("Generators"), QIcon(), false);
    addMenuItem(tr("Hash"), QIcon());
    addMenuItem(tr("Lorem ipsum"), QIcon());
    addMenuItem(tr("UUID"), QIcon());
    addMenuItem(tr("Random Text"), QIcon());
    addMenuItem(tr("Random Number"), QIcon());
    addMenuItem(tr("Chmod Calculator"), QIcon());
    addMenuItem(tr("Qr code"), QIcon());
    addMenuItem(tr("Desktop Creator"), QIcon());
    addMenuItem(tr("Text"), QIcon(), false);
    addMenuItem(tr("Regex"), QIcon());
    addMenuItem(tr("Text diff"), QIcon());
    addMenuItem(tr("Markdown"), QIcon());
    addMenuItem(tr("XML Scheme Validator"), QIcon());
    addMenuItem(tr("Converters"), QIcon(), false);
    addMenuItem(tr("JSON - YAML"), QIcon());
    addMenuItem(tr("Timestamp"), QIcon());
    addMenuItem(tr("Number Bases"), QIcon());
    addMenuItem(tr("QDateTime"), QIcon());
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
    QAction* action = new QAction(text, ui->menuTools);
    action->setIcon(icon);
    item->setIcon(icon);
    if(!enabled)
    {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        action->setEnabled(false);
    }
    else
    {
        int currentIndex = index;
        menuIndexMap[text] = index++;
        connect(action, &QAction::triggered, this, [this, currentIndex]{ui->stackedWidget->setCurrentIndex(currentIndex);});
    }
    ui->listWidget->addItem(item);
    ui->menuTools->addAction(action);
}
