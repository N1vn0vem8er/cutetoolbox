#include "gzipwidget.h"
#include "src/widgets/ui_gzipwidget.h"
#include <QFileDialog>
#include <zlib.h>

GZipWidget::GZipWidget(QWidget *parent)
    : QWidget(parent)
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
}

GZipWidget::~GZipWidget()
{
    delete ui;
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
