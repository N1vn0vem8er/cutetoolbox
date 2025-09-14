#include "mainwindow.h"
#include "editor/codeeditor.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <qplaintextedit.h>
#include <qsettings.h>

#define VERSION "0.0.1"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("cutetoolbox");
    restoreGeometry(settings.value("Geometry").toByteArray());
    restoreState(settings.value("State").toByteArray());
    ui->actionSide_menu->setChecked(settings.value("sideMenu", true).toBool());
    ui->listWidget->setVisible(ui->actionSide_menu->isChecked());
    connect(ui->listWidget, &QListWidget::clicked, this, &MainWindow::showWidget);
    connect(ui->actionSide_menu, &QAction::triggered, this, [&]{ui->listWidget->setVisible(ui->actionSide_menu->isChecked());});
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [&]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_CuteToolBox, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
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
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &MainWindow::widgetChanged);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);

    currentToolLabel = new QLabel(ui->statusbar);
    currentToolLabel->setText("HTML Encoder");
    ui->statusbar->addPermanentWidget(currentToolLabel);

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
    addMenuItem(tr("Users"), QIcon());
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
    addMenuItem(tr("Graphics"), QIcon(), false);
    addMenuItem(tr("Color Picker"), QIcon());
    if(currentMenu)
        ui->menuTools->addMenu(currentMenu);
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

void MainWindow::widgetChanged()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        ui->actionSave->setEnabled(widget->canSaveFiles());
        ui->actionSave_As->setEnabled(widget->canSaveFiles());
        ui->actionCopy->setEnabled(widget->canBasicEdit());
        ui->actionCut->setEnabled(widget->canBasicEdit());
        ui->actionPaste->setEnabled(widget->canBasicEdit());
        ui->actionSelect_All->setEnabled(widget->canBasicEdit());
        ui->actionDelete_All->setEnabled(widget->canBasicEdit());
        ui->actionDelete->setEnabled(widget->canBasicEdit());
        ui->actionUndo->setEnabled(widget->canBasicEdit());
        ui->actionRedo->setEnabled(widget->canBasicEdit());
        ui->actionOpen->setEnabled(widget->canOpenFiles());
        ui->actionIncrease_font_size->setEnabled(widget->canChangeFont());
        ui->actionDecrease_font_size->setEnabled(widget->canChangeFont());
        ui->actionSet_font_size->setEnabled(widget->canChangeFont());
        ui->actionReset_font_size->setEnabled(widget->canChangeFont());
        ui->actionSet_font->setEnabled(widget->canChangeFont());
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
        currentMenu = new QMenu(text, ui->menuTools);
        currentMenu->setIcon(icon);
        ui->menuTools->addMenu(currentMenu);
    }
    else
    {
        int currentIndex = index;
        menuIndexMap[text] = index++;
        connect(action, &QAction::triggered, this, [this, currentIndex, text]{ui->stackedWidget->setCurrentIndex(currentIndex); currentToolLabel->setText(text);});
        if(currentMenu)
            currentMenu->addAction(action);
    }
    ui->listWidget->addItem(item);
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
            <h4>External libraries other then Qt used in this project:</h4>
            <p>LibXml2 Copyright (C) 1998-2012 Daniel Veillard.  All Rights Reserved. Copyright (C) The Libxml2 Contributors. MIT License</p>
            <p>Zlib copyright © 1995-2025 Jean-loup Gailly and Mark Adler.</p>
            <p>yaml-cpp Copyright (c) 2008-2015 Jesse Beder. MIT License</p>
            <p>Qr Code Genenerator Copyright © 2025 Project Nayuki. MIT License</p>
            <p>Marked Copyright (c) 2011-2018, Christopher Jeffrey MIT License</p>
            <p>markdown.css Copyright 2011 Kevin Burke Apache License, Version 2.0</p>
        </body>
    </html>
)").arg(VERSION, LICENSELINK));
}

void MainWindow::save()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canSaveFiles())
            widget->save();
    }
}

void MainWindow::saveAs()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canSaveFiles())
            widget->saveAs();
    }
}

void MainWindow::open()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canOpenFiles())
            widget->open();
    }
}

void MainWindow::copy()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
        {
            CodeEditor* focused = qobject_cast<CodeEditor*>(qApp->focusWidget());
            if(focused)
            {
                focused->copy();
            }
            else
            {
                QPlainTextEdit* focused = qobject_cast<QPlainTextEdit*>(qApp->focusWidget());
                if(focused)
                {
                    focused->copy();
                }
                else
                {
                    QLineEdit* focused = qobject_cast<QLineEdit*>(qApp->focusWidget());
                    if(focused)
                    {
                        focused->copy();
                    }
                }
            }
        }
    }
}

void MainWindow::cut()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
        {
            CodeEditor* focused = qobject_cast<CodeEditor*>(qApp->focusWidget());
            if(focused)
            {
                focused->cut();
            }
            else
            {
                QPlainTextEdit* focused = qobject_cast<QPlainTextEdit*>(qApp->focusWidget());
                if(focused)
                {
                    focused->cut();
                }
                else
                {
                    QLineEdit* focused = qobject_cast<QLineEdit*>(qApp->focusWidget());
                    if(focused)
                    {
                        focused->cut();
                    }
                }
            }
        }
    }
}

void MainWindow::paste()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
        {
            CodeEditor* focused = qobject_cast<CodeEditor*>(qApp->focusWidget());
            if(focused)
            {
                focused->paste();
            }
            else
            {
                QPlainTextEdit* focused = qobject_cast<QPlainTextEdit*>(qApp->focusWidget());
                if(focused)
                {
                    focused->paste();
                }
                else
                {
                    QLineEdit* focused = qobject_cast<QLineEdit*>(qApp->focusWidget());
                    if(focused)
                    {
                        focused->paste();
                    }
                }
            }
        }
    }
}

void MainWindow::selectAll()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
        {
            CodeEditor* focused = qobject_cast<CodeEditor*>(qApp->focusWidget());
            if(focused)
            {
                focused->selectAll();
            }
            else
            {
                QPlainTextEdit* focused = qobject_cast<QPlainTextEdit*>(qApp->focusWidget());
                if(focused)
                {
                    focused->selectAll();
                }
                else
                {
                    QLineEdit* focused = qobject_cast<QLineEdit*>(qApp->focusWidget());
                    if(focused)
                    {
                        focused->selectAll();
                    }
                }
            }
        }
    }
}

void MainWindow::deleteSelected()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
        {
            CodeEditor* focused = qobject_cast<CodeEditor*>(qApp->focusWidget());
            if(focused)
            {
                focused->deleteSelected();
            }
            else
            {
                QPlainTextEdit* focused = qobject_cast<QPlainTextEdit*>(qApp->focusWidget());
                if(focused)
                {
                    focused->textCursor().removeSelectedText();
                }
                else
                {
                    QLineEdit* focused = qobject_cast<QLineEdit*>(qApp->focusWidget());
                    if(focused)
                    {
                        focused->del();
                    }
                }
            }
        }
    }
}

void MainWindow::deleteAll()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
        {
            CodeEditor* focused = qobject_cast<CodeEditor*>(qApp->focusWidget());
            if(focused)
            {
                focused->deleteAll();
            }
            else
            {
                QPlainTextEdit* focused = qobject_cast<QPlainTextEdit*>(qApp->focusWidget());
                if(focused)
                {
                    focused->clear();
                }
                else
                {
                    QLineEdit* focused = qobject_cast<QLineEdit*>(qApp->focusWidget());
                    if(focused)
                    {
                        focused->clear();
                    }
                }
            }
        }
    }
}

void MainWindow::undo()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
        {
            CodeEditor* focused = qobject_cast<CodeEditor*>(qApp->focusWidget());
            if(focused)
            {
                focused->undo();
            }
            else
            {
                QPlainTextEdit* focused = qobject_cast<QPlainTextEdit*>(qApp->focusWidget());
                if(focused)
                {
                    focused->undo();
                }
                else
                {
                    QLineEdit* focused = qobject_cast<QLineEdit*>(qApp->focusWidget());
                    if(focused)
                    {
                        focused->undo();
                    }
                }
            }
        }
    }
}

void MainWindow::redo()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canBasicEdit())
        {
            CodeEditor* focused = qobject_cast<CodeEditor*>(qApp->focusWidget());
            if(focused)
            {
                focused->redo();
            }
            else
            {
                QPlainTextEdit* focused = qobject_cast<QPlainTextEdit*>(qApp->focusWidget());
                if(focused)
                {
                    focused->redo();
                }
                else
                {
                    QLineEdit* focused = qobject_cast<QLineEdit*>(qApp->focusWidget());
                    if(focused)
                    {
                        focused->redo();
                    }
                }
            }
        }
    }
}

void MainWindow::increaseFontSize()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canChangeFont())
            widget->increaseFontSize();
    }
}

void MainWindow::decreaseFontSize()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canChangeFont())
            widget->decreaseFontSize();
    }
}

void MainWindow::setFontSize()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canChangeFont())
            widget->setFontSize();
    }
}

void MainWindow::resetFontSize()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canChangeFont())
            widget->resetFontSize();
    }
}

void MainWindow::setFont()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canChangeFont())
            widget->setFont();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("cutetoolbox");
    settings.setValue("State", saveState());
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("sideMenu", ui->actionSide_menu->isChecked());
    QMainWindow::closeEvent(event);
}
