#include "xmlschemevalidatorwidget.h"
#include "src/widgets/ui_xmlschemevalidatorwidget.h"
#include <libxml/xmlschemas.h>
#include <QFileDialog>
#include <QInputDialog>
#include <qdialog.h>
#include <qdir.h>
#include <QFontDialog>

XMLSchemeValidatorWidget::XMLSchemeValidatorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::XMLSchemeValidatorWidget)
{
    ui->setupUi(this);
    setName(tr("XML Scheme Validator"));
    connect(ui->openXMLButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly))
            {
                ui->xml->setPlainText(file.readAll());
                file.close();
                openedXmlFile = path;
                emit opened(openedXmlFile + " " + openedXsdFile);
            }
        }
    });
    connect(ui->openXSDButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly))
            {
                ui->schema->setPlainText(file.readAll());
                file.close();
                openedXsdFile = path;
                emit opened(openedXmlFile + " " + openedXsdFile);
            }
        }
    });
    connect(ui->copyXMLButton, &QPushButton::clicked, ui->xml, &CodeEditor::copyAll);
    connect(ui->copyXSDButton, &QPushButton::clicked, ui->schema, &CodeEditor::copyAll);
    connect(ui->pasteXMLButton, &QPushButton::clicked, ui->xml, &CodeEditor::paste);
    connect(ui->pasteXSDButton, &QPushButton::clicked, ui->schema, &CodeEditor::paste);
    connect(ui->clearXMLButton, &QPushButton::clicked, ui->xml, &CodeEditor::clear);
    connect(ui->clearXSDButton, &QPushButton::clicked, ui->schema, &CodeEditor::clear);
    infoLabel = new QLabel(ui->infoLabelContainer);
    QVBoxLayout* layout = new QVBoxLayout(ui->infoLabelContainer);
    layout->addWidget(infoLabel);
    ui->infoLabelContainer->setLayout(layout);
    connect(ui->xml, &CodeEditor::textChanged, this, &XMLSchemeValidatorWidget::validate);
    ui->scrollArea->setVisible(false);
}

XMLSchemeValidatorWidget::~XMLSchemeValidatorWidget()
{
    delete ui;
}

bool XMLSchemeValidatorWidget::canOpenFiles() const
{
    return true;
}

bool XMLSchemeValidatorWidget::canSaveFiles() const
{
    return true;
}

bool XMLSchemeValidatorWidget::canBasicEdit() const
{
    return true;
}

bool XMLSchemeValidatorWidget::canChangeFont() const
{
    return true;
}

void XMLSchemeValidatorWidget::save()
{
    if(!openedXmlFile.isEmpty() || !openedXsdFile.isEmpty())
    {
        TextEdits option = TextEdits::none;
        if(ui->xml->hasFocus() && !openedXmlFile.isEmpty())
            option = xml;
        else if(ui->schema->hasFocus() && !openedXsdFile.isEmpty())
            option = xsd;
        else if(!openedXmlFile.isEmpty() && !openedXsdFile.isEmpty())
            option = getSelectedOption();
        else if(!openedXmlFile.isEmpty())
            option = xml;
        else if(!openedXsdFile.isEmpty())
            option = xsd;
        if(option != TextEdits::none)
        {
            if(option == TextEdits::xml)
            {
                QFile file(openedXmlFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->xml->toPlainText().toUtf8());
                    file.close();
                    emit saved(tr("Saved: %1").arg(openedXmlFile));
                }
            }
            else if(option == TextEdits::xsd)
            {
                QFile file(openedXsdFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->schema->toPlainText().toUtf8());
                    file.close();
                    emit saved(tr("Saved: %1").arg(openedXsdFile));
                }
            }
        }
    }
    else
        saveAs();
}

void XMLSchemeValidatorWidget::saveAs()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly))
            {
                if(option == TextEdits::xml)
                {
                    file.write(ui->xml->toPlainText().toUtf8());
                    openedXmlFile = path;
                    emit saved(tr("Saved: %1").arg(openedXmlFile));
                }
                else if(option == TextEdits::xsd)
                {
                    file.write(ui->schema->toPlainText().toUtf8());
                    openedXsdFile = path;
                    emit saved(tr("Saved: %1").arg(openedXsdFile));
                }
                file.close();
                emit opened(openedXmlFile + " " + openedXsdFile);
            }
        }
    }
}

void XMLSchemeValidatorWidget::open()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly))
            {
                if(option == TextEdits::xml)
                {
                    ui->xml->setPlainText(file.readAll());
                    openedXmlFile = path;
                }
                else if(option == TextEdits::xsd)
                {
                    ui->schema->setPlainText(file.readAll());
                    openedXsdFile = path;
                }
                emit opened(openedXmlFile + " " + openedXsdFile);
            }
        }
    }
}

void XMLSchemeValidatorWidget::increaseFontSize()
{
    if(ui->xml->hasFocus())
        ui->xml->increaseFontSize();
    else if(ui->schema->hasFocus())
        ui->schema->increaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::xml)
                ui->xml->increaseFontSize();
            else if(option == TextEdits::xsd)
                ui->schema->increaseFontSize();
        }
    }
}

void XMLSchemeValidatorWidget::decreaseFontSize()
{
    if(ui->xml->hasFocus())
        ui->xml->decreaseFontSize();
    else if(ui->schema->hasFocus())
        ui->schema->decreaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::xml)
                ui->xml->decreaseFontSize();
            else if(option == TextEdits::xsd)
                ui->schema->decreaseFontSize();
        }
    }
}

void XMLSchemeValidatorWidget::setFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
        if(ok)
        {
            if(option == TextEdits::xml)
                ui->xml->setFontSize(size);
            else if(option == TextEdits::xsd)
                ui->schema->setFontSize(size);
        }
    }
}

void XMLSchemeValidatorWidget::resetFontSize()
{
    if(ui->xml->hasFocus())
        ui->xml->setFontSize(10);
    else if(ui->schema->hasFocus())
        ui->schema->setFontSize(10);
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::xml)
                ui->xml->setFontSize(10);
            else if(option == TextEdits::xsd)
                ui->schema->setFontSize(10);
        }
    }
}

void XMLSchemeValidatorWidget::setFont()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, this);
        if(ok)
        {
            if(option == TextEdits::xml)
                ui->xml->setFont(font);
            else if(option == TextEdits::xsd)
                ui->schema->setFont(font);
        }
    }
}

QString XMLSchemeValidatorWidget::getOpenedFileName() const
{
    return openedXmlFile + " " + openedXsdFile;
}

void XMLSchemeValidatorWidget::handleErrorMsg(void *userData, xmlErrorPtr error)
{
    if(userData && error)
    {
        XMLSchemeValidatorWidget* widget = static_cast<XMLSchemeValidatorWidget*>(userData);
        widget->errors.append(error->message);
    }
}

XMLSchemeValidatorWidget::TextEdits XMLSchemeValidatorWidget::getSelectedOption()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton xmlButton(tr("Xml"), &dialog);
    QPushButton xsdButton(tr("Xsd"), &dialog);
    connect(&xmlButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::xml;});
    connect(&xsdButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::xsd;});
    connect(&xmlButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&xsdButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&xmlButton);
    layout.addWidget(&xsdButton);
    dialog.setLayout(&layout);
    if(dialog.exec() == QDialog::Accepted)
    {
        return option;
    }
    return option;
}

void XMLSchemeValidatorWidget::validate()
{
    xmlDocPtr doc = nullptr;
    xmlSchemaPtr schema = nullptr;
    xmlSchemaParserCtxtPtr parserCtxt = nullptr;
    xmlSchemaValidCtxtPtr validCtxt = nullptr;
    QByteArray schemaByteArray = ui->schema->toPlainText().toUtf8();
    errors.clear();
    xmlSetStructuredErrorFunc(this, handleErrorMsg);

    parserCtxt = xmlSchemaNewMemParserCtxt(schemaByteArray, schemaByteArray.size());
    if(parserCtxt)
    {
        schema = xmlSchemaParse(parserCtxt);
        if(schema)
        {
            QByteArray docByteArray = ui->xml->toPlainText().toUtf8();
            doc = xmlReadMemory(docByteArray, docByteArray.size(), nullptr, nullptr, 0);
            if(doc)
            {
                validCtxt = xmlSchemaNewValidCtxt(schema);
                if(validCtxt)
                {
                    if(xmlSchemaValidateDoc(validCtxt, doc) == 0)
                    {
                        ui->xml->setPalette(QApplication::palette(ui->xml));
                    }
                }
            }
        }
    }
    QString errorMsgs;
    for(const QString& i : std::as_const(errors))
    {
        errorMsgs.append(i);
    }
    infoLabel->setText(errorMsgs);
    ui->scrollArea->setVisible(!infoLabel->text().isEmpty());

    if(validCtxt) xmlSchemaFreeValidCtxt(validCtxt);
    if(schema) xmlSchemaFree(schema);
    if(parserCtxt) xmlSchemaFreeParserCtxt(parserCtxt);
    if(doc) xmlFreeDoc(doc);
    xmlCleanupParser();
}
