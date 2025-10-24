#include "previewpage.h"
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>

PreviewPage::PreviewPage(QObject *parent) : QWebEnginePage(parent) {}

void PreviewPage::setAbsolutePath(const QString &newAbsolutePath)
{
    absolutePath = newAbsolutePath;
}

bool PreviewPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame)
{
    const QString path = QFileInfo(absolutePath).absoluteDir().absolutePath() + QDir::separator() + url.toString().remove("qrc:/");
    if(QFile::exists(path))
    {
        setAbsolutePath(path);
        emit openedFile(path);
        return false;
    }
    if(url.scheme() == QString("qrc"))
        return true;
    QDesktopServices::openUrl(url);
    return false;
}
