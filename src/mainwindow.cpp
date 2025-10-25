#include "mainwindow.h"
#include "base64coderdecoderwidget.h"
#include "chmodcalculatorwidget.h"
#include "colorpalettegeneratorwidget.h"
#include "colorpicker.h"
#include "config.h"
#include "contrastcheckerwidget.h"
#include "cppformatterwidget.h"
#include "csharpformatterwidget.h"
#include "desktopcreatorwidget.h"
#include "editor/codeeditor.h"
#include "gzipwidget.h"
#include "hashwidget.h"
#include "htmlformatterwidget.h"
#include "imageformatconverterwidget.h"
#include "javaformatterwidget.h"
#include "javascriptformatterwidget.h"
#include "jsonformatterwidget.h"
#include "jsonyamlconverter.h"
#include "loremipsumgenerator.h"
#include "markdownwidget.h"
#include "numberbaseswidget.h"
#include "objectivecformatterwidget.h"
#include "protoformatterwidget.h"
#include "qdatetimeconverterwidget.h"
#include "qrcodegenerator.h"
#include "randomnumbergeneratorwidget.h"
#include "randomtextgeneratorwidget.h"
#include "regexwidget.h"
#include "sqlformatterwidget.h"
#include "tablegenformatterwidget.h"
#include "textdiffwidget.h"
#include "textprotoformatterwidget.h"
#include "timestampwidget.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <qplaintextedit.h>
#include <qsettings.h>
#include "htmlcoderdecoderwidget.h"
#include "urlcoderdecoderwidget.h"
#include "usergeneratorwidget.h"
#include "uuidgeneratorwidget.h"
#include "verilogformatterwidget.h"
#include "videoconverterwidget.h"
#include "xmlformatterwidget.h"
#include "xmlschemevalidatorwidget.h"

#define VERSION "0.1.0"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openedFileLabel = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(openedFileLabel);
    QSettings settings(Config::settingsName);
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
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::close);
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
    connect(ui->actionFull_screen, &QAction::triggered, this, [&]{if(isFullScreen()) showNormal(); else showFullScreen();});
    connect(ui->actionFind, &QAction::triggered, this, [&]{ui->searchWidget->setVisible(!ui->searchWidget->isVisible());});

    ui->searchWidget->setVisible(false);

    ui->splitter->setStretchFactor(1, 1);
    ui->listWidget->setFrameShape(QFrame::NoFrame);
    addMenuItem(tr("Coders and decoders"), QIcon());
    addMenuItem(tr("HTML Encoder"), QIcon(), new HTMLCoderDecoderWidget(ui->stackedWidget));
    addMenuItem(tr("Base64"), QIcon(), new Base64CoderDecoderWidget(ui->stackedWidget));
    addMenuItem(tr("Url"), QIcon(), new UrlCoderDecoderWidget(ui->stackedWidget));
    addMenuItem(tr("GZip"), QIcon(), new GZipWidget(ui->stackedWidget));
    addMenuItem(tr("Formatters"), QIcon());
    addMenuItem(tr("JSON"), QIcon(), new JsonFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("SQL"), QIcon(), new SQLFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("XML"), QIcon(), new XMLFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("HTML"), QIcon(), new HTMLFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("C++"), QIcon(), new CppFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("CSharp"), QIcon(), new CSharpFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("Java"), QIcon(), new JavaFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("JavaScript"), QIcon(), new JavaScriptFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("Objective C"), QIcon(), new ObjectiveCFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("Proto"), QIcon(), new ProtoFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("TableGen"), QIcon(), new TableGenFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("TextProto"), QIcon(), new TextProtoFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("Verilog"), QIcon(), new VerilogFormatterWidget(ui->stackedWidget));
    addMenuItem(tr("Generators"), QIcon());
    addMenuItem(tr("Hash"), QIcon(), new HashWidget(ui->stackedWidget));
    addMenuItem(tr("Lorem ipsum"), QIcon(), new LoremIpsumGenerator(ui->stackedWidget));
    addMenuItem(tr("UUID"), QIcon(), new UUIDGeneratorWidget(ui->stackedWidget));
    addMenuItem(tr("Random Text"), QIcon(), new RandomTextGeneratorWidget(ui->stackedWidget));
    addMenuItem(tr("Random Number"), QIcon(), new RandomNumberGeneratorWidget(ui->stackedWidget));
    addMenuItem(tr("Chmod Calculator"), QIcon(), new ChmodCalculatorWidget(ui->stackedWidget));
    addMenuItem(tr("Qr code"), QIcon(), new QrCodeGenerator(ui->stackedWidget));
    addMenuItem(tr("Desktop Creator"), QIcon(), new DesktopCreatorWidget(ui->stackedWidget));
    addMenuItem(tr("Users"), QIcon(), new UserGeneratorWidget(ui->stackedWidget));
    addMenuItem(tr("Text"), QIcon());
    addMenuItem(tr("Regex"), QIcon(), new RegexWidget(ui->stackedWidget));
    addMenuItem(tr("Text diff"), QIcon(), new TextDiffWidget(ui->stackedWidget));
    addMenuItem(tr("Markdown"), QIcon(), new MarkdownWidget(ui->stackedWidget));
    addMenuItem(tr("XML Scheme Validator"), QIcon(), new XMLSchemeValidatorWidget(ui->stackedWidget));
    addMenuItem(tr("Converters"), QIcon());
    addMenuItem(tr("JSON - YAML"), QIcon(), new JsonYamlConverter(ui->stackedWidget));
    addMenuItem(tr("Timestamp"), QIcon(), new TimestampWidget(ui->stackedWidget));
    addMenuItem(tr("Number Bases"), QIcon(), new NumberBasesWidget(ui->stackedWidget));
    addMenuItem(tr("QDateTime"), QIcon(), new QDateTimeConverterWidget(ui->stackedWidget));
    addMenuItem(tr("Graphics"), QIcon());
    addMenuItem(tr("Color Picker"), QIcon(), new ColorPicker(ui->stackedWidget));
    addMenuItem(tr("Image Format Converter"), QIcon(), new ImageFormatConverterWidget(ui->stackedWidget));
    addMenuItem(tr("Video Format Converter"), QIcon(), new VideoConverterWidget(ui->stackedWidget));
    addMenuItem(tr("Contrast Checker"), QIcon(), new ContrastCheckerWidget(ui->stackedWidget));
    addMenuItem(tr("Color Palette"), QIcon(), new ColorPaletteGeneratorWidget(ui->stackedWidget));
    if(currentMenu)
        ui->menuTools->addMenu(currentMenu);

    ui->stackedWidget->setCurrentIndex(settings.value("lastTool", 0).toInt());
    ui->toolNameLabel->setText(static_cast<CustomWidget*>(ui->stackedWidget->currentWidget())->getName());

    completer = new QCompleter(toolNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->searchLine->setCompleter(completer);
    connect(completer, qOverload<const QString&>(&QCompleter::activated), this, &MainWindow::find);
    connect(ui->searchLine, &QLineEdit::returnPressed, this, [&]{find(ui->searchLine->text());});
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
        showByName(text);
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
        ui->actionClose->setEnabled(widget->canOpenFiles() || widget->canSaveFiles());
        openedFileLabel->setText(widget->getOpenedFileName());
        ui->menuRecent->setEnabled(widget->canOpenFiles());
        if(widget->canOpenFiles())
            updateRecent();
    }
}

void MainWindow::addMenuItem(const QString &text, const QIcon &icon, CustomWidget *widget)
{
    static int index = 0;
    QListWidgetItem* item = new QListWidgetItem(text, ui->listWidget);
    QAction* action = new QAction(text, ui->menuTools);
    action->setIcon(icon);
    item->setIcon(icon);
    if(!widget)
    {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        currentMenu = new QMenu(text, ui->menuTools);
        currentMenu->setIcon(icon);
        ui->menuTools->addMenu(currentMenu);
    }
    else
    {
        ui->stackedWidget->addWidget(widget);
        int currentIndex = index;
        menuIndexMap[text] = index++;
        connect(action, &QAction::triggered, this, [this, currentIndex, text]{
            ui->stackedWidget->setCurrentIndex(currentIndex);
            ui->toolNameLabel->setText(static_cast<CustomWidget*>(ui->stackedWidget->widget(currentIndex))->getName());
        });
        connect(widget, &CustomWidget::opened, this, &MainWindow::openedFile);
        connect(widget, &CustomWidget::saved, this, &MainWindow::savedFile);
        if(widget->canOpenFiles())
            connect(widget, &CustomWidget::updateRecent, this, &MainWindow::updateRecent);
        toolNames.append(QString("%1 (%2)").arg(text, currentMenu->title()));
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

void MainWindow::close()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        if(widget->canOpenFiles() || widget->canSaveFiles())
            widget->close();
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

void MainWindow::savedFile(const QString &path)
{
    ui->statusbar->showMessage(path, 3000);
}

void MainWindow::openedFile(const QString &path)
{
    openedFileLabel->setText(path);
}

void MainWindow::find(QString text)
{
    text.remove(QRegularExpression(R"(\s\(.*\))"));
    if(menuIndexMap.contains(text))
    {
        showByName(text);
        widgetChanged();
    }
}

void MainWindow::updateRecent()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
    {
        const QStringList recent = widget->getRecentFiles();
        ui->menuRecent->clear();
        for(const auto& i : recent)
        {
            QAction* action = new QAction(ui->menuRecent);
            action->setText(i);
            connect(action, &QAction::triggered, this, [this, i]{openFromRecent(i);});
            ui->menuRecent->addAction(action);
        }
        if(ui->menuRecent->isEmpty())
        {
            QAction* action = new QAction(ui->menuRecent);
            action->setText(tr("no recent"));
            action->setEnabled(false);
            ui->menuRecent->addAction(action);
        }
        else
        {
            ui->menuRecent->addSeparator();
            QAction* action = new QAction(ui->menuRecent);
            action->setText(tr("Clear Recent"));
            connect(action, &QAction::triggered, this, [this]{clearRecent();});
            ui->menuRecent->addAction(action);
        }
    }
}

void MainWindow::openFromRecent(const QString &path)
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
        widget->openFromRecent(path);
}

void MainWindow::clearRecent()
{
    CustomWidget* widget = qobject_cast<CustomWidget*>(ui->stackedWidget->currentWidget());
    if(widget)
        widget->clearRecent();
}

void MainWindow::showByName(const QString &name)
{
    ui->stackedWidget->setCurrentIndex(menuIndexMap.value(name));
    ui->toolNameLabel->setText(static_cast<CustomWidget*>(ui->stackedWidget->currentWidget())->getName());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings(Config::settingsName);
    settings.setValue("State", saveState());
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("sideMenu", ui->actionSide_menu->isChecked());
    settings.setValue("lastTool", ui->stackedWidget->currentIndex());
    QMainWindow::closeEvent(event);
}
