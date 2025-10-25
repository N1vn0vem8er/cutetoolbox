#include "desktopcreatorwidget.h"
#include "config.h"
#include "src/widgets/ui_desktopcreatorwidget.h"
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <qdialog.h>
#include <qdir.h>
#include <qtconcurrentrun.h>

DesktopCreatorWidget::DesktopCreatorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::DesktopCreatorWidget)
{
    ui->setupUi(this);
    setName(tr("Desktop File Creator"));
    QSettings settings(Config::settingsName);
    ui->languagesListWidget->setVisible(false);
    ui->addLanguageButton->setVisible(false);
    ui->removeLanguageButton->setVisible(false);
    connect(ui->name, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->exec, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->tryexec, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->icon, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->type, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->version, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->categories, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->comment, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->mimeTypes, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->startupWMClass, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->terminal, &QCheckBox::clicked, this, &DesktopCreatorWidget::generate);
    connect(ui->noDisplay, &QCheckBox::clicked, this, &DesktopCreatorWidget::generate);
    connect(ui->startupNotify, &QCheckBox::clicked, this, &DesktopCreatorWidget::generate);
    connect(ui->keywords, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(ui->genericName, &QLineEdit::textChanged, this, &DesktopCreatorWidget::generate);
    connect(&watcher, &QFutureWatcher<QString>::started, this, [&]{ui->selectIcon->setEnabled(false);});
    connect(&watcher, &QFutureWatcher<QString>::finished, this, [&]{
        icons = watcher.result();
        for(const QString& icon : std::as_const(icons))
        {
            ui->selectIcon->addItem(QIcon::fromTheme(icon), icon);
        }
        ui->selectIcon->setEnabled(true);
    });
    connect(ui->selectIcon, &QComboBox::currentTextChanged, this, [&](const QString& text){ui->icon->setText(text);});
    connect(ui->openButton, &QPushButton::clicked, this, &DesktopCreatorWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{ui->output->selectAll(); ui->output->copy();});
    connect(ui->clearButton, &QPushButton::clicked, ui->output, &QPlainTextEdit::clear);
    connect(ui->saveButton, &QPushButton::clicked, this, &DesktopCreatorWidget::saveAs);
    int size = settings.beginReadArray("desktopCreator.recentFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentFiles.append(path);
    }
    settings.endArray();
    startSearchingForIcons();
}

DesktopCreatorWidget::~DesktopCreatorWidget()
{
    QSettings settings(Config::settingsName);
    settings.beginWriteArray("desktopCreator.recentFiles");
    for(int i = 0; i<recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFiles.at(i));
    }
    settings.endArray();
    delete ui;
}

bool DesktopCreatorWidget::canOpenFiles() const
{
    return true;
}

bool DesktopCreatorWidget::canSaveFiles() const
{
    return true;
}

bool DesktopCreatorWidget::canBasicEdit() const
{
    return true;
}

void DesktopCreatorWidget::save()
{
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->output->toPlainText().toUtf8());
            file.close();
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
    else
        saveAs();
}

void DesktopCreatorWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save"), QDir::homePath(), "*.desktop");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->output->toPlainText().toUtf8());
            file.close();
            openedFile = path;
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
        }
    }
}

void DesktopCreatorWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.desktop");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->output->setPlainText(file.readAll());
            file.close();
            readFile(path);
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

void DesktopCreatorWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

QString DesktopCreatorWidget::getOpenedFileName() const
{
    return openedFile;
}

QStringList DesktopCreatorWidget::getRecentFiles() const
{
    return recentFiles;
}

void DesktopCreatorWidget::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->output->setPlainText(file.readAll());
            file.close();
            readFile(path);
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

void DesktopCreatorWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
}

void DesktopCreatorWidget::startSearchingForIcons()
{
    QFuture<QList<QString>> future = QtConcurrent::run([]{
        QList<QString> result;
        QStringList paths;
        paths.append("/usr/share/applications");
        paths.append(QDir::homePath() + "/.local/share/applications");
        paths << QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);
        paths << QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).constFirst() + "/applications";
        paths.removeDuplicates();
        for(const QString& path : std::as_const(paths))
        {
            const QList<QFileInfo> files{QDir(path).entryInfoList({"*.desktop"}, QDir::NoDotAndDotDot | QDir::Files)};
            for(const QFileInfo& file : files)
            {
                QSettings settings(file.absoluteFilePath(), QSettings::IniFormat);
                settings.beginGroup("Desktop Entry");
                QString icon = settings.value("Icon").toString();
                settings.endGroup();
                if(!icon.isEmpty())
                {
                    result.append(icon);
                }
            }
        }
        result.removeDuplicates();
        return result;
    });
    watcher.setFuture(future);
}

void DesktopCreatorWidget::readFile(const QString &path)
{
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("Desktop Entry");
    ui->name->setText(settings.value("Name").toString());
    ui->exec->setText(settings.value("Exec").toString());
    ui->tryexec->setText(settings.value("TryExec").toString());
    ui->icon->setText(settings.value("Icon").toString());
    ui->type->setText(settings.value("Type").toString());
    ui->version->setText(settings.value("Version").toString());
    ui->genericName->setText(settings.value("GenericName").toString());
    ui->categories->setText(settings.value("Categories").toString());
    ui->comment->setText(settings.value("Comment").toString());
    ui->mimeTypes->setText(settings.value("MimeType").toStringList().join(";"));
    ui->keywords->setText(settings.value("Keywords").toStringList().join(";"));
    ui->startupWMClass->setText(settings.value("StartupWMClass").toString());
    ui->terminal->setChecked(settings.value("Terminal", false).toBool());
    ui->noDisplay->setChecked(settings.value("NoDisplay", false).toBool());
    ui->startupNotify->setChecked(settings.value("StartupNotify", false).toBool());
    settings.endGroup();
}

void DesktopCreatorWidget::generate()
{
    QString buffer;
    buffer.append("[Desktop Entry]\n");
    if(!ui->name->text().isEmpty())
        buffer.append(QString("Name=%1\n").arg(ui->name->text()));
    if(!ui->exec->text().isEmpty())
        buffer.append(QString("Exec=%1\n").arg(ui->exec->text()));
    if(!ui->tryexec->text().isEmpty())
        buffer.append(QString("TryExec=%1\n").arg(ui->tryexec->text()));
    if(!ui->icon->text().isEmpty())
        buffer.append(QString("Icon=%1\n").arg(ui->icon->text()));
    if(!ui->type->text().isEmpty())
        buffer.append(QString("Type=%1\n").arg(ui->type->text()));
    if(!ui->version->text().isEmpty())
        buffer.append(QString("Version=%1\n").arg(ui->version->text()));
    if(!ui->genericName->text().isEmpty())
        buffer.append(QString("GenericName=%1\n").arg(ui->genericName->text()));
    if(!ui->categories->text().isEmpty())
        buffer.append(QString("Categories=%1\n").arg(ui->categories->text()));
    if(!ui->comment->text().isEmpty())
        buffer.append(QString("Comment=%1\n").arg(ui->comment->text()));
    if(!ui->mimeTypes->text().isEmpty())
        buffer.append(QString("MimeType=%1\n").arg(ui->mimeTypes->text()));
    if(!ui->keywords->text().isEmpty())
        buffer.append(QString("Keywords=%1\n").arg(ui->keywords->text()));
    if(!ui->startupWMClass->text().isEmpty())
        buffer.append(QString("StartupWMClass=%1\n").arg(ui->startupWMClass->text()));
    buffer.append(QString("Terminal=%1\n").arg(ui->terminal->isChecked() ? "true" : "false"));
    buffer.append(QString("NoDisplay=%1\n").arg(ui->noDisplay->isChecked() ? "true" : "false"));
    buffer.append(QString("StartupNotify=%1\n").arg(ui->startupNotify->isChecked() ? "true" : "false"));
    ui->output->setPlainText(buffer);
}
