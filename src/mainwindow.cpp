#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::clicked, this, &MainWindow::showWidget);
    connect(ui->actionSide_menu, &QAction::triggered, this, [&]{ui->listWidget->setVisible(ui->actionSide_menu->isChecked());});
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [&]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_CuteToolBox, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(ui->actionSelect_All, &QAction::triggered, this, &MainWindow::selectAll);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::deleteSelected);
    connect(ui->actionDelete_All, &QAction::triggered, this, &MainWindow::deleteAll);
    connect(ui->actionIncrease_font_size, &QAction::triggered, this, &MainWindow::increaseFontSize);
    connect(ui->actionDecrease_font_size, &QAction::triggered, this, &MainWindow::decreaseFontSize);
    connect(ui->actionSet_font_size, &QAction::triggered, this, &MainWindow::setFontSize);
    connect(ui->actionReset_font_size, &QAction::triggered, this, &MainWindow::resetFontSize);
    connect(ui->actionSet_font, &QAction::triggered, this, &MainWindow::setFont);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);

    currentToolLabel = new QLabel(ui->statusbar);
    currentToolLabel->setText("HTML Encoder");
    ui->statusbar->addPermanentWidget(currentToolLabel);

    ui->actionSide_menu->setChecked(true);
    ui->splitter->setStretchFactor(1, 1);
    ui->listWidget->setFrameShape(QFrame::NoFrame);
    addMenuItem(tr("Coders and decoders"), QIcon(), false);
    addMenuItem(tr("HTML Encoder"), QIcon());
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
        currentToolLabel->setText(text);
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
        connect(action, &QAction::triggered, this, [this, currentIndex, text]{ui->stackedWidget->setCurrentIndex(currentIndex); currentToolLabel->setText(text);});
    }
    ui->listWidget->addItem(item);
    ui->menuTools->addAction(action);
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About Cute ToolBox"), tr(R"(
    <html>
        <body>
            <h3>Cute ToolBox</h3>
            <p>Cute ToolBox is a collection of tools for developers.</p>
            <p>Version: %1</p>
            <p>License: <a href="%2">GPL 3</a>
            <h4>External liblaries other then Qt used in this project:</h4>
            <p>Lib Xml</p>
            <p>Zlib</p>
            <p>yaml-cpp</p>
            <p>Qr Code Gen</p>
            <p>Marked Copyright (c) 2011-2018, Christopher Jeffrey (https://github.com/chjj/) MIT License</p>
            <p>markdown.css Copyright 2011 Kevin Burke Apache License, Version 2.0</p>
        </body>
    </html>
)"));
}

void MainWindow::saveAs()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canSaveFiles())
            widget->saveAs();
    }
}

void MainWindow::open()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canOpenFiles())
            widget->open();
    }
}

void MainWindow::copy()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
            widget->copy();
    }
}

void MainWindow::cut()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
            widget->cut();
    }
}

void MainWindow::paste()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
            widget->paste();
    }
}

void MainWindow::selectAll()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
            widget->selectAll();
    }
}

void MainWindow::deleteSelected()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
            widget->deleteText();
    }
}

void MainWindow::deleteAll()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
            widget->deleteAllText();
    }
}

void MainWindow::undo()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
            widget->undo();
    }
}

void MainWindow::redo()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
            widget->redo();
    }
}

void MainWindow::increaseFontSize()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canChangeFont())
            widget->increaseFontSize();
    }
}

void MainWindow::decreaseFontSize()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canChangeFont())
            widget->decreaseFontSize();
    }
}

void MainWindow::setFontSize()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canChangeFont())
            widget->setFontSize();
    }
}

void MainWindow::resetFontSize()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canChangeFont())
            widget->resetFontSize();
    }
}

void MainWindow::setFont()
{
    CustomWidget* widget = dynamic_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canChangeFont())
            widget->setFont();
    }
}
