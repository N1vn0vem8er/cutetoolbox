#include "desktopcreatorwidget.h"
#include "src/widgets/ui_desktopcreatorwidget.h"
#include <QSettings>
#include <QStandardPaths>
#include <qdir.h>
#include <qtconcurrentrun.h>

DesktopCreatorWidget::DesktopCreatorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DesktopCreatorWidget)
{
    ui->setupUi(this);
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
    connect(&watcher, &QFutureWatcher<QString>::started, this, [&]{ui->selectIconButton->setEnabled(false);});
    connect(&watcher, &QFutureWatcher<QString>::finished, this, [&]{icons = watcher.result(); ui->selectIconButton->setEnabled(true);});
    startSearchingForIcons();
}

DesktopCreatorWidget::~DesktopCreatorWidget()
{
    delete ui;
}

void DesktopCreatorWidget::startSearchingForIcons()
{
    QFuture<QList<QString>> future = QtConcurrent::run([]{
        QList<QString> result;
        QStringList paths;
        paths.append("/usr/share/applications");
        paths.append(QDir::homePath() + "/.local/share/applications");
        paths << QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);
        paths << QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).first() + "/applications";
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
        return result;
    });
    watcher.setFuture(future);
}

void DesktopCreatorWidget::generate()
{
    QString buffer;
    buffer.append(QString("[Name]=%1\n").arg(ui->name->text()));
    buffer.append(QString("[Exec]=%1\n").arg(ui->exec->text()));
    buffer.append(QString("[TryExec]=%1\n").arg(ui->tryexec->text()));
    buffer.append(QString("[Icon]=%1\n").arg(ui->icon->text()));
    buffer.append(QString("[Type]=%1\n").arg(ui->type->text()));
    buffer.append(QString("[Version]=%1\n").arg(ui->version->text()));
    buffer.append(QString("[GenericName]=%1\n").arg(ui->genericName->text()));
    buffer.append(QString("[Categories]=%1\n").arg(ui->categories->text()));
    buffer.append(QString("[Comment]=%1\n").arg(ui->comment->text()));
    buffer.append(QString("[MimeType]=%1\n").arg(ui->mimeTypes->text()));
    buffer.append(QString("[Keywords]=%1\n").arg(ui->keywords->text()));
    buffer.append(QString("[StartupWMClass]=%1\n").arg(ui->startupWMClass->text()));
    buffer.append(QString("[Terminal]=%1\n").arg(ui->terminal->isChecked()));
    buffer.append(QString("[NoDisplay]=%1\n").arg(ui->noDisplay->isChecked()));
    buffer.append(QString("[StartupNotify]=%1\n").arg(ui->startupNotify->isChecked()));
    ui->output->setPlainText(buffer);
}

void DesktopCreatorWidget::openSelectIcon()
{

}
