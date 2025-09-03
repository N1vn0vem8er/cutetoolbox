#include "gzipwidget.h"
#include "src/widgets/ui_gzipwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <qinputdialog.h>
#include <zlib.h>

GZipWidget::GZipWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::GZipWidget)
{
    ui->setupUi(this);
    connect(ui->input, &QPlainTextEdit::textChanged, this, &GZipWidget::compress);
    connect(ui->output, &QPlainTextEdit::textChanged, this, &GZipWidget::decompress);
    connect(ui->clearTextButton, &QPushButton::clicked, ui->input, &QPlainTextEdit::clear);
    connect(ui->clearGZipButton, &QPushButton::clicked, ui->output, &QPlainTextEdit::clear);
    connect(ui->pasteTextButton, &QPushButton::clicked, ui->input, &QPlainTextEdit::paste);
    connect(ui->pasteGZipButton, &QPushButton::clicked, ui->output, &QPlainTextEdit::paste);
    connect(ui->copyOutputButton, &QPushButton::clicked, ui->output, [&]{ui->output->selectAll(); ui->output->copy();});
    connect(ui->copyTextButton, &QPushButton::clicked, ui->input, [&]{ui->input->selectAll(); ui->input->copy();});
    connect(ui->openTextButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->input->setPlainText(file.readAll());
                file.close();
            }
        }
    });
    connect(ui->openGZipButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->output->setPlainText(file.readAll().toBase64());
                file.close();
            }
        }
    });
    ui->input->setReplaceTabWithSpacesEnabled(false);
    ui->input->setAutoClosingEnabled(false);
    ui->output->setAutoClosingEnabled(false);
    ui->output->setReplaceTabWithSpacesEnabled(false);
}

GZipWidget::~GZipWidget()
{
    delete ui;
}

bool GZipWidget::canOpenFiles() const
{
    return true;
}

bool GZipWidget::canSaveFiles() const
{
    return true;
}

bool GZipWidget::canBasicEdit() const
{
    return true;
}

bool GZipWidget::canChangeFont() const
{
    return true;
}

void GZipWidget::save()
{

}

void GZipWidget::saveAs()
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
                file.write(option == TextEdits::input ? ui->input->toPlainText().toUtf8() : ui->output->toPlainText().toUtf8());
                file.close();
            }
        }
    }
}

void GZipWidget::open()
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
                if(option == TextEdits::input)
                    ui->input->setPlainText(file.readAll());
                else if(option == TextEdits::output)
                    ui->output->setPlainText(file.readAll());
                file.close();
            }
        }
    }
}

void GZipWidget::increaseFontSize()
{
    if(ui->input->hasFocus())
        ui->input->increaseFontSize();
    else if(ui->output->hasFocus())
        ui->output->increaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::input)
                ui->input->increaseFontSize();
            else if(option == TextEdits::output)
                ui->output->increaseFontSize();
        }
    }
}

void GZipWidget::decreaseFontSize()
{
    if(ui->input->hasFocus())
        ui->input->decreaseFontSize();
    else if(ui->output->hasFocus())
        ui->output->decreaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::input)
                ui->input->decreaseFontSize();
            else if(option == TextEdits::output)
                ui->output->decreaseFontSize();
        }
    }
}

void GZipWidget::setFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200);
        if(option == TextEdits::input)
            ui->input->setFontSize(size);
        else if(option == TextEdits::output)
            ui->output->setFontSize(size);
    }
}

void GZipWidget::resetFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        if(option == TextEdits::input)
            ui->input->setFontSize(10);
        else if(option == TextEdits::output)
            ui->output->setFontSize(10);
    }
}

void GZipWidget::setFont()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, this);
        if(ok)
        {
            if(option == TextEdits::input)
                ui->input->setFont(font);
            else if(option == TextEdits::output)
                ui->output->setFont(font);
        }
    }
}

GZipWidget::TextEdits GZipWidget::getSelectedOption()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton textButton(tr("Text"), &dialog);
    QPushButton outputButton(tr("GZip"), &dialog);
    connect(&textButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::input;});
    connect(&outputButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::output;});
    connect(&textButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&outputButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&textButton);
    layout.addWidget(&outputButton);
    dialog.setLayout(&layout);
    if(dialog.exec() == QDialog::Accepted)
    {
        return option;
    }
    return option;
}

void GZipWidget::compress()
{    if(!compressing)
    {
        compressing = true;
        const QByteArray data = ui->input->toPlainText().toUtf8();
        z_stream zs;
        zs.zalloc = Z_NULL;
        zs.zfree = Z_NULL;
        zs.opaque = Z_NULL;

        if(deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
        {
            return;
        }

        zs.next_in = (Bytef*)data.constData();
        zs.avail_in = data.size();
        QByteArray compressedData;
        const int CHUNK_SIZE = 16384;
        char out[CHUNK_SIZE];

        do {
            zs.next_out = (Bytef*)out;
            zs.avail_out = CHUNK_SIZE;
            deflate(&zs, Z_FINISH);
            int have = CHUNK_SIZE - zs.avail_out;
            compressedData.append(out, have);
        } while (zs.avail_out == 0);
        deflateEnd(&zs);
        ui->output->setPlainText(QString::fromLatin1(compressedData.toBase64()));
    }
    else
    {
        compressing = false;
    }
}

void GZipWidget::decompress()
{
    if(!compressing)
    {
        compressing = true;
        const QByteArray data = QByteArray::fromBase64(ui->output->toPlainText().toUtf8());
        z_stream zs;
        zs.zalloc = Z_NULL;
        zs.zfree = Z_NULL;
        zs.opaque = Z_NULL;
        zs.next_in = (Bytef*)data.constData();
        zs.avail_in = data.size();

        if(inflateInit2(&zs, 15 + 16) != Z_OK)
        {
            return;
        }
        QByteArray decompressedData;
        const int CHUNK_SIZE = 16384;
        char out[CHUNK_SIZE];

        do {
            zs.next_out = (Bytef*)out;
            zs.avail_out = CHUNK_SIZE;
            int ret = inflate(&zs, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                inflateEnd(&zs);
                return;
            }
            int have = CHUNK_SIZE - zs.avail_out;
            decompressedData.append(out, have);
        } while (zs.avail_out == 0);

        inflateEnd(&zs);
        ui->input->setPlainText(decompressedData);
    }
    else
    {
        compressing = false;
    }
}
