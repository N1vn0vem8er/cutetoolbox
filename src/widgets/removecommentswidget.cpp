#include "removecommentswidget.h"
#include "src/widgets/ui_removecommentswidget.h"
#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <qinputdialog.h>
#include <clang-c/Index.h>

RemoveCommentsWidget::RemoveCommentsWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::RemoveCommentsWidget)
{
    ui->setupUi(this);
    connect(ui->removeButton, &QPushButton::clicked, this, &RemoveCommentsWidget::removeComments);
    ui->languageComboBox->insertItem(static_cast<int>(Languages::Cpp), tr("C++"));
    ui->languageComboBox->insertItem(static_cast<int>(Languages::C), tr("C"));
    ui->languageComboBox->insertItem(static_cast<int>(Languages::ObjectiveC), tr("Objective C"));
}

RemoveCommentsWidget::~RemoveCommentsWidget()
{
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
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
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
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
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

void RemoveCommentsWidget::removeComments()
{
    const QByteArray utf8 = ui->codeEditor->toPlainText().toUtf8();
    const char* buffer = utf8.constData();
    const size_t bufferSize = static_cast<size_t>(utf8.size());
    CXIndex index = clang_createIndex(0, 0);
    CXUnsavedFile unsaved {
        .Filename = "input.cpp",
        .Contents = buffer,
        .Length = static_cast<unsigned long>(bufferSize)
    };
    QList<const char*> args;
    switch(static_cast<Languages>(ui->languageComboBox->currentIndex()))
    {
    case Languages::Cpp:
        unsaved.Filename = "input.cpp";
        args = {"-std=c++20", "-x", "c++"};
        break;
    case Languages::C:
        unsaved.Filename = "input.c";
        args = {"-std=c17", "-x", "c"};
        break;
    case Languages::ObjectiveC:
        unsaved.Filename = "input.m";
        args = {"-x", "objective-c"};
        break;
    case Languages::None:
        return;
        break;
    }

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
        CXSourceLocation end   = clang_getRangeEnd(r);
        unsigned startOffset = 0, endOffset = 0;
        clang_getFileLocation(start, nullptr, nullptr, nullptr, &startOffset);
        clang_getFileLocation(end,   nullptr, nullptr, nullptr, &endOffset);
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
