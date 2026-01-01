#include "removecommentswidget.h"
#include "config.h"
#include "src/widgets/ui_removecommentswidget.h"
#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <qinputdialog.h>
#include <clang-c/Index.h>
#include <QProcess>
#include <qsettings.h>

RemoveCommentsWidget::RemoveCommentsWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::RemoveCommentsWidget)
{
    ui->setupUi(this);
    setName(tr("Remove Comments"));
    ui->languageComboBox->insertItem(static_cast<int>(Languages::Cpp), tr("C++"));
    ui->languageComboBox->insertItem(static_cast<int>(Languages::C), tr("C"));
    ui->languageComboBox->insertItem(static_cast<int>(Languages::ObjectiveC), tr("Objective C"));
    if(std::system("python3 --version > /dev/null 2>&1") == 0)
    {
        ui->languageComboBox->insertItem(static_cast<int>(Languages::Python), tr("Python"));
    }
    QSettings settings(Config::settingsName);
    ui->languageComboBox->setCurrentIndex(settings.value("removecomments.selectedLanguage", 0).toInt());
    connect(ui->removeButton, &QPushButton::clicked, this, &RemoveCommentsWidget::removeComments);
    connect(ui->openButton, &QPushButton::clicked, this, &RemoveCommentsWidget::open);
    connect(ui->saveAsButton, &QPushButton::clicked, this, &RemoveCommentsWidget::saveAs);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->claerButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
    connect(ui->removeEmptyButton, &QPushButton::clicked, this, &RemoveCommentsWidget::removeNewLines);
    int size = settings.beginReadArray("removecomments.recentFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentFiles.append(path);
    }
    settings.endArray();
}

RemoveCommentsWidget::~RemoveCommentsWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("removecomments.selectedLanguage", ui->languageComboBox->currentIndex());
    settings.beginWriteArray("removecomments.recentFiles");
    for(int i = 0; i<recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFiles.at(i));
    }
    settings.endArray();
    delete ui;
}

bool RemoveCommentsWidget::canOpenFiles() const
{
    return true;
}

bool RemoveCommentsWidget::canSaveFiles() const
{
    return true;
}

bool RemoveCommentsWidget::canBasicEdit() const
{
    return true;
}

bool RemoveCommentsWidget::canChangeFont() const
{
    return true;
}

void RemoveCommentsWidget::save()
{
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
    else
        saveAs();
}

void RemoveCommentsWidget::saveAs()
{
    QString suffix;
    switch(static_cast<Languages>(ui->languageComboBox->currentIndex()))
    {
    case Languages::None:
        break;
    case Languages::Cpp:
        suffix = "*.cpp *.h *.hpp";
        break;
    case Languages::C:
        suffix = "*.c *.h";
        break;
    case Languages::ObjectiveC:
        suffix = "*.m *.mm";
        break;
    case Languages::Python:
        suffix = "*.py";
        break;
    }
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), suffix);
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
            openedFile = path;
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
        }
    }
}

void RemoveCommentsWidget::open()
{
    QString suffix;
    switch(static_cast<Languages>(ui->languageComboBox->currentIndex()))
    {
    case Languages::None:
        break;
    case Languages::Cpp:
        suffix = "*.cpp *.h *.hpp";
        break;
    case Languages::C:
        suffix = "*.c *.h";
        break;
    case Languages::ObjectiveC:
        suffix = "*.m *.mm";
        break;
    case Languages::Python:
        suffix = "*.py";
        break;
    }
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), suffix);
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->codeEditor->setPlainText(file.readAll());
            file.close();
            openedFile = path;
            if(recentFiles.length() >= 10)
                recentFiles.removeFirst();
            if(!recentFiles.contains(openedFile))
                recentFiles.append(openedFile);
            emit updateRecent();
            emit opened(openedFile);
        }
    }
}

void RemoveCommentsWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

void RemoveCommentsWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void RemoveCommentsWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void RemoveCommentsWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void RemoveCommentsWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void RemoveCommentsWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->codeEditor->setFont(font);
    }
}

QString RemoveCommentsWidget::getOpenedFileName() const
{
    return openedFile;
}

QStringList RemoveCommentsWidget::getRecentFiles() const
{
    return recentFiles;
}

void RemoveCommentsWidget::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->codeEditor->setPlainText(file.readAll());
            file.close();
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

void RemoveCommentsWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
}

void RemoveCommentsWidget::removeCommentsClang(const QString &fileName, const QList<const char *> &args)
{
    const QByteArray utf8 = ui->codeEditor->toPlainText().toUtf8();
    const char* buffer = utf8.constData();
    const size_t bufferSize = static_cast<size_t>(utf8.size());
    CXIndex index = clang_createIndex(0, 0);
    CXUnsavedFile unsaved {
        .Filename = fileName.toUtf8(),
        .Contents = buffer,
        .Length = static_cast<unsigned long>(bufferSize)
    };
    CXTranslationUnit tu{};
    if(clang_parseTranslationUnit2(index, unsaved.Filename, args.data(), 1, &unsaved, 1, CXTranslationUnit_None, &tu) != CXError_Success)
    {
        clang_disposeIndex(index);
        return;
    }
    CXCursor cursor = clang_getTranslationUnitCursor(tu);
    CXSourceRange fullRange = clang_getCursorExtent(cursor);
    CXToken* tokens = nullptr;
    unsigned tokenCount = 0;
    clang_tokenize(tu, fullRange, &tokens, &tokenCount);
    struct Range
    {
        size_t begin;
        size_t end;
    };
    QList<Range> commentRanges;
    commentRanges.reserve(tokenCount);
    for(unsigned i = 0; i < tokenCount; ++i)
    {
        if(clang_getTokenKind(tokens[i]) != CXToken_Comment)
            continue;
        CXSourceRange r = clang_getTokenExtent(tu, tokens[i]);
        CXSourceLocation start = clang_getRangeStart(r);
        CXSourceLocation end = clang_getRangeEnd(r);
        unsigned startOffset = 0, endOffset = 0;
        clang_getFileLocation(start, nullptr, nullptr, nullptr, &startOffset);
        clang_getFileLocation(end, nullptr, nullptr, nullptr, &endOffset);
        commentRanges.push_back({ startOffset, endOffset });
    }
    clang_disposeTokens(tu, tokens, tokenCount);
    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);
    if(commentRanges.isEmpty())
        return;
    std::sort(commentRanges.begin(), commentRanges.end(), [](const Range& a, const Range& b) {return a.begin < b.begin;});
    QString result;
    result.reserve(ui->codeEditor->toPlainText().size());
    size_t cursorPos = 0;
    for(const auto& r : commentRanges)
    {
        if(r.begin > cursorPos)
        {
            result += QString::fromUtf8(buffer + cursorPos,static_cast<int>(r.begin - cursorPos));
        }
        cursorPos = r.end;
    }
    if(cursorPos < bufferSize)
    {
        result += QString::fromUtf8(buffer + cursorPos,static_cast<int>(bufferSize - cursorPos));
    }
    ui->codeEditor->setPlainText(result);
}

void RemoveCommentsWidget::removeCommentsPython()
{
    const QString command = QString(R"(
import sys
import tokenize
import io

def strip_comments_preserve(source):
    lines = source.splitlines(keepends=True)
    source_bytes = source.encode('utf-8')
    bytes_io = io.BytesIO(source_bytes)
    tokens = tokenize.tokenize(bytes_io.readline)
    for tok in tokens:
        if tok.type == tokenize.COMMENT:
            start_line, start_col = tok.start
            end_line, end_col = tok.end
            idx = start_line - 1
            line = lines[idx]
            lines[idx] = line[:start_col] + line[end_col:]
    return "".join(lines)
source_code = sys.stdin.read()
print(strip_comments_preserve(source_code), end='')
    )");
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::finished, this, [process, this](int exitCode, QProcess::ExitStatus status){
        if(status == QProcess::NormalExit && exitCode == 0)
        {
            QString output = QString::fromUtf8(process->readAllStandardOutput());
            ui->codeEditor->setPlainText(output);
        }
        else
        {
            QByteArray error = process->readAllStandardError();
            qDebug() << error;
        }
        process->deleteLater();
    });
    process->start("python3", {"-c", command});
    if(process->waitForStarted())
    {
        QByteArray codeToClean = ui->codeEditor->toPlainText().toUtf8();
        process->write(codeToClean);
        process->closeWriteChannel();
    }
}

void RemoveCommentsWidget::removeComments()
{
    switch(static_cast<Languages>(ui->languageComboBox->currentIndex()))
    {
    case Languages::Cpp:
        removeCommentsClang("input.cpp", {"-std=c++20", "-x", "c++"});
        break;
    case Languages::C:
        removeCommentsClang("input.c", {"-std=c17", "-x", "c"});
        break;
    case Languages::ObjectiveC:
        removeCommentsClang("input.m", {"-x", "objective-c"});
        break;
    case Languages::Python:
        removeCommentsPython();
        break;
    case Languages::None:
        return;
        break;
    }
}

void RemoveCommentsWidget::removeNewLines()
{
    const static QRegularExpression regex("(\\r?\\n){2,}");
    ui->codeEditor->setPlainText(ui->codeEditor->toPlainText().replace(regex, "\n"));
}
