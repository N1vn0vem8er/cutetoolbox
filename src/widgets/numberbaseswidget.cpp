#include "numberbaseswidget.h"
#include "src/widgets/ui_numberbaseswidget.h"

NumberBasesWidget::NumberBasesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NumberBasesWidget)
{
    ui->setupUi(this);
    connect(ui->binary, &QLineEdit::textEdited, this, &NumberBasesWidget::fromBin);
    connect(ui->decimal, &QLineEdit::textEdited, this, &NumberBasesWidget::fromDec);
    connect(ui->octal, &QLineEdit::textEdited, this, &NumberBasesWidget::fromOct);
    connect(ui->hexadecimal, &QLineEdit::textEdited, this, &NumberBasesWidget::fromHex);
    connect(ui->ascii, &QLineEdit::textEdited, this, &NumberBasesWidget::fromASCII);
    connect(ui->utf8, &QLineEdit::textEdited, this, &NumberBasesWidget::fromUTF8);
    connect(ui->copyBinaryButton, &QPushButton::clicked, this, [&]{ui->binary->selectAll(); ui->binary->copy();});
    connect(ui->pasteBinaryButton, &QPushButton::clicked, ui->binary, &QLineEdit::paste);
    connect(ui->clearBinaryButton, &QPushButton::clicked, ui->binary, &QLineEdit::clear);
    connect(ui->copyDecimalButton, &QPushButton::clicked, this, [&]{ui->decimal->selectAll(); ui->decimal->copy();});
    connect(ui->pasteDecimalButton, &QPushButton::clicked, ui->decimal, &QLineEdit::paste);
    connect(ui->clearDecimalButton, &QPushButton::clicked, ui->decimal, &QLineEdit::clear);
    connect(ui->copyOctalButton, &QPushButton::clicked, this, [&]{ui->octal->selectAll(); ui->octal->copy();});
    connect(ui->pasteOctalButton, &QPushButton::clicked, ui->octal, &QLineEdit::paste);
    connect(ui->clearOctalButton, &QPushButton::clicked, ui->octal, &QLineEdit::clear);
    connect(ui->copyHexButton, &QPushButton::clicked, this, [&]{ui->hexadecimal->selectAll(); ui->hexadecimal->copy();});
    connect(ui->pasteHexButton, &QPushButton::clicked, ui->hexadecimal, &QLineEdit::paste);
    connect(ui->clearHexButton, &QPushButton::clicked, ui->hexadecimal, &QLineEdit::clear);
    connect(ui->copyASCIIButton, &QPushButton::clicked, this, [&]{ui->ascii->selectAll(); ui->ascii->copy();});
    connect(ui->pasteASCIIButton, &QPushButton::clicked, ui->ascii, &QLineEdit::paste);
    connect(ui->clearASCIIButton, &QPushButton::clicked, ui->ascii, &QLineEdit::clear);
    connect(ui->copyUTF8Button, &QPushButton::clicked, this, [&]{ui->utf8->selectAll(); ui->utf8->copy();});
    connect(ui->pasteUTF8Button, &QPushButton::clicked, ui->utf8, &QLineEdit::paste);
    connect(ui->clearUTF8Button, &QPushButton::clicked, ui->utf8, &QLineEdit::clear);
}

NumberBasesWidget::~NumberBasesWidget()
{
    delete ui;
}

void NumberBasesWidget::fromBin()
{
    const int val = ui->binary->text().toInt(nullptr, 2);
    ui->decimal->setText(QString::number(val));
    ui->octal->setText(QString::number(val, 8));
    ui->hexadecimal->setText(QString::number(val, 16));
    ui->ascii->setText(QChar::fromLatin1(val));
    ui->utf8->setText(QChar::fromUcs2(val));
}

void NumberBasesWidget::fromDec()
{
    const int val = ui->decimal->text().toInt(nullptr, 10);
    ui->binary->setText(QString::number(val, 2));
    ui->octal->setText(QString::number(val, 8));
    ui->hexadecimal->setText(QString::number(val, 16));
    ui->ascii->setText(QChar::fromLatin1(val));
    ui->utf8->setText(QChar::fromUcs2(val));
}

void NumberBasesWidget::fromOct()
{
    const int val = ui->octal->text().toInt(nullptr, 8);
    ui->decimal->setText(QString::number(val));
    ui->binary->setText(QString::number(val, 2));
    ui->hexadecimal->setText(QString::number(val, 16));
    ui->ascii->setText(QChar::fromLatin1(val));
    ui->utf8->setText(QChar::fromUcs2(val));
}

void NumberBasesWidget::fromHex()
{
    const int val = ui->hexadecimal->text().toInt(nullptr, 16);
    ui->decimal->setText(QString::number(val));
    ui->octal->setText(QString::number(val, 8));
    ui->binary->setText(QString::number(val, 2));
    ui->ascii->setText(QChar::fromLatin1(val));
    ui->utf8->setText(QChar::fromUcs2(val));
}

void NumberBasesWidget::fromASCII()
{
    if(ui->ascii->text().length() > 0)
    {
        const int val = ui->ascii->text().at(0).cell();
        ui->decimal->setText(QString::number(val));
        ui->octal->setText(QString::number(val, 8));
        ui->hexadecimal->setText(QString::number(val, 16));
        ui->binary->setText(QString::number(val, 2));
        ui->utf8->setText(QChar::fromUcs2(val));
    }
}

void NumberBasesWidget::fromUTF8()
{
    if(ui->utf8->text().length() > 0)
    {
        const int val = ui->utf8->text().at(0).cell();
        ui->decimal->setText(QString::number(val));
        ui->octal->setText(QString::number(val, 8));
        ui->hexadecimal->setText(QString::number(val, 16));
        ui->ascii->setText(QChar::fromLatin1(val));
        ui->binary->setText(QString::number(val, 2));
    }
}
