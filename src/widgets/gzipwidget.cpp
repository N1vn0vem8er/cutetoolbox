#include "gzipwidget.h"
#include "config.h"
#include "src/widgets/ui_gzipwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <qinputdialog.h>
#include <qsettings.h>
#include <qtconcurrentrun.h>
#include <zlib.h>

GZipWidget::GZipWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::GZipWidget)
{
    ui->setupUi(this);
    setName(tr("GZip Compresser and Decompresser"));
    QSettings settings(Config::settingsName);
    connect(ui->input, &QPlainTextEdit::textChanged, this, &GZipWidget::compress);
    connect(ui->output, &QPlainTextEdit::textChanged, this, &GZipWidget::decompress);
    connect(ui->clearTextButton, &QPushButton::clicked, ui->input, &QPlainTextEdit::clear);
    connect(ui->clearGZipButton, &QPushButton::clicked, ui->output, &QPlainTextEdit::clear);
    connect(ui->pasteTextButton, &QPushButton::clicked, ui->input, &QPlainTextEdit::paste);
    connect(ui->pasteGZipButton, &QPushButton::clicked, ui->output, &QPlainTextEdit::paste);
    connect(ui->copyOutputButton, &QPushButton::clicked, ui->output, [&]{ui->output->selectAll(); ui->output->copy();});
    connect(ui->copyTextButton, &QPushButton::clicked, ui->input, [&]{ui->input->selectAll(); ui->input->copy();});
    connect(ui->openTextButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), !openedInputFile.isEmpty() ? QFileInfo(openedInputFile).dir().absolutePath() : QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->input->setPlainText(file.readAll());
                openedInputFile = path;
                file.close();
                if(recentInputFiles.length() >= 10)
                    recentInputFiles.removeFirst();
                if(!recentInputFiles.contains(openedInputFile))
                    recentInputFiles.append(openedInputFile);
                emit opened(openedInputFile + " " + openedOutputFile);
                emit updateRecent();
            }
        }
    });
    connect(ui->openGZipButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), !openedOutputFile.isEmpty() ? QFileInfo(openedOutputFile).dir().absolutePath() : QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->output->setPlainText(file.readAll().toBase64());
                openedOutputFile = path;
                file.close();
                if(recentOutputFiles.length() >= 10)
                    recentOutputFiles.removeFirst();
                if(!recentOutputFiles.contains(openedOutputFile))
                    recentOutputFiles.append(openedOutputFile);
                emit opened(openedInputFile + " " + openedOutputFile);
                emit updateRecent();
            }
        }
    });
    connect(&compressingWatcher, &QFutureWatcher<QByteArray>::finished, this, [&]{
        ui->output->setPlainText(QString::fromLatin1(compressingWatcher.result().toBase64()));
        compressing = false;
    });
    connect(&decompressingWatcher, &QFutureWatcher<QByteArray>::finished, this, [&]{
        ui->input->setPlainText(decompressingWatcher.result());
        compressing = false;
    });
    ui->input->setReplaceTabWithSpacesEnabled(false);
    ui->input->setAutoClosingEnabled(false);
    ui->output->setAutoClosingEnabled(false);
    ui->output->setReplaceTabWithSpacesEnabled(false);
    int size = settings.beginReadArray("gZipWidget.recentInputFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentInputFiles.append(path);
    }
    settings.endArray();
    size = settings.beginReadArray("gZipWidget.recentOutputFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentOutputFiles.append(path);
    }
    settings.endArray();
}

GZipWidget::~GZipWidget()
{
    QSettings settings(Config::settingsName);
    settings.beginWriteArray("gZipWidget.recentInputFiles");
    for(int i = 0; i<recentInputFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentInputFiles.at(i));
    }
    settings.endArray();
    settings.beginWriteArray("gZipWidget.recentOutputFiles");
    for(int i = 0; i<recentOutputFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentOutputFiles.at(i));
    }
    settings.endArray();
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
    if(!openedOutputFile.isEmpty() || !openedInputFile.isEmpty())
    {
        TextEdits option = TextEdits::none;
        if(ui->input->hasFocus() && !openedInputFile.isEmpty())
            option = TextEdits::input;
        else if(ui->output->hasFocus() && !openedOutputFile.isEmpty())
            option = TextEdits::output;
        else if(!openedInputFile.isEmpty() && !openedOutputFile.isEmpty())
            option = getSelectedOption();
        else if(!openedInputFile.isEmpty())
            option = TextEdits::input;
        else if(openedOutputFile.isEmpty())
            option = TextEdits::output;
        if(option != TextEdits::none)
        {
            if(option == TextEdits::input)
            {
                QFile file(openedInputFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->input->toPlainText().toUtf8());
                    file.close();
                    emit saved(tr("Saved: %1").arg(openedInputFile));
                }
            }
            else if(option == TextEdits::output)
            {
                QFile file(openedOutputFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(QByteArray::fromBase64(ui->output->toPlainText().toUtf8()));
                    file.close();
                    emit saved(tr("Saved: %1").arg(openedOutputFile));
                }
            }
        }
    }
    else
        saveAs();
}

void GZipWidget::saveAs()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), !openedInputFile.isEmpty() && option == TextEdits::input ? QFileInfo(openedInputFile).dir().absolutePath() : !openedOutputFile.isEmpty() && option == TextEdits::output ? QFileInfo(openedOutputFile).dir().absolutePath() : QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly))
            {
                if(option == TextEdits::input)
                {
                    file.write(ui->input->toPlainText().toUtf8());
                    openedInputFile = path;
                    emit saved(tr("Saved: %1").arg(openedInputFile));
                }
                else if(option == TextEdits::output)
                {
                    file.write(QByteArray::fromBase64(ui->output->toPlainText().toUtf8()));
                    openedOutputFile = path;
                    emit saved(tr("Saved: %1").arg(openedOutputFile));
                }
                file.close();
                emit opened(openedInputFile + " " + openedOutputFile);
            }
        }
    }
}

void GZipWidget::open()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const QString path = QFileDialog::getOpenFileName(this, tr("Open"), !openedInputFile.isEmpty() && option == TextEdits::input ? QFileInfo(openedInputFile).dir().absolutePath() : !openedOutputFile.isEmpty() && option == TextEdits::output ? QFileInfo(openedOutputFile).dir().absolutePath() : QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly))
            {
                if(option == TextEdits::input)
                {
                    ui->input->setPlainText(file.readAll());
                    openedInputFile = path;
                    if(recentInputFiles.length() >= 10)
                        recentInputFiles.removeFirst();
                    if(!recentInputFiles.contains(openedInputFile))
                        recentInputFiles.append(openedInputFile);
                }
                else if(option == TextEdits::output)
                {
                    ui->output->setPlainText(file.readAll().toBase64());
                    openedOutputFile = path;
                    if(recentOutputFiles.length() >= 10)
                        recentOutputFiles.removeFirst();
                    if(!recentOutputFiles.contains(openedOutputFile))
                        recentOutputFiles.append(openedOutputFile);
                }
                file.close();
                emit opened(openedInputFile + " " + openedOutputFile);
                emit updateRecent();
            }
        }
    }
}

void GZipWidget::close()
{
    if(openedInputFile.isEmpty())
        openedOutputFile.clear();
    else if(openedOutputFile.isEmpty())
        openedInputFile.clear();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::input)
                openedInputFile.clear();
            else if(option == TextEdits::output)
                openedOutputFile.clear();
        }
    }
    emit opened(openedInputFile + " " + openedOutputFile);
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

QString GZipWidget::getOpenedFileName() const
{
    return openedInputFile + " " + openedOutputFile;
}

QStringList GZipWidget::getRecentFiles() const
{
    return recentInputFiles + recentOutputFiles;
}

void GZipWidget::openFromRecent(const QString &path)
{
    if(recentInputFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->input->setPlainText(file.readAll());
            file.close();
            openedInputFile = path;
            emit opened(openedInputFile + " " + openedOutputFile);
        }
    }
    else if(recentOutputFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->output->setPlainText(file.readAll());
            file.close();
            openedOutputFile = path;
            emit opened(openedInputFile + " " + openedOutputFile);
        }
    }
}

void GZipWidget::clearRecent()
{
    recentInputFiles.clear();
    recentOutputFiles.clear();
    emit updateRecent();
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
{
    if(!compressing)
    {
        compressing = true;
        const QByteArray data = ui->input->toPlainText().toUtf8();
        QFuture<QByteArray> future = QtConcurrent::run([data]{
            z_stream zs;
            zs.zalloc = Z_NULL;
            zs.zfree = Z_NULL;
            zs.opaque = Z_NULL;
            QByteArray compressedData;
            if(deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
                return compressedData;

            zs.next_in = (Bytef*)data.constData();
            zs.avail_in = data.size();
            const int CHUNK_SIZE = 16384;
            char out[CHUNK_SIZE];
            do{
                zs.next_out = (Bytef*)out;
                zs.avail_out = CHUNK_SIZE;
                deflate(&zs, Z_FINISH);
                int have = CHUNK_SIZE - zs.avail_out;
                compressedData.append(out, have);
            }
            while(zs.avail_out == 0);
            deflateEnd(&zs);
            return compressedData;
        });
        compressingWatcher.setFuture(future);
    }
}

void GZipWidget::decompress()
{
    if(!compressing)
    {
        compressing = true;
        const QByteArray data = QByteArray::fromBase64(ui->output->toPlainText().toUtf8());
        QFuture<QByteArray> future = QtConcurrent::run([data]{
            z_stream zs;
            zs.zalloc = Z_NULL;
            zs.zfree = Z_NULL;
            zs.opaque = Z_NULL;
            zs.next_in = (Bytef*)data.constData();
            zs.avail_in = data.size();
            QByteArray decompressedData;
            if(inflateInit2(&zs, 15 + 16) != Z_OK)
                return decompressedData;

            const int CHUNK_SIZE = 16384;
            char out[CHUNK_SIZE];
            do{
                zs.next_out = (Bytef*)out;
                zs.avail_out = CHUNK_SIZE;
                int ret = inflate(&zs, Z_NO_FLUSH);
                if (ret != Z_OK && ret != Z_STREAM_END) {
                    inflateEnd(&zs);
                    return decompressedData;
                }
                int have = CHUNK_SIZE - zs.avail_out;
                decompressedData.append(out, have);
            }
            while(zs.avail_out == 0);
            inflateEnd(&zs);
            return decompressedData;
        });
        decompressingWatcher.setFuture(future);
    }
}
