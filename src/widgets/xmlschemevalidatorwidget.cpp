#include "xmlschemevalidatorwidget.h"
#include "src/widgets/ui_xmlschemevalidatorwidget.h"
#include <libxml/xmlschemas.h>

XMLSchemeValidatorWidget::XMLSchemeValidatorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XMLSchemeValidatorWidget)
{
    ui->setupUi(this);
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

void XMLSchemeValidatorWidget::handleErrorMsg(void *userData, xmlErrorPtr error)
{
    if(userData && error)
    {
        XMLSchemeValidatorWidget* widget = static_cast<XMLSchemeValidatorWidget*>(userData);
        widget->errors.append(error->message);
    }
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
