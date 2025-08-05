#ifndef PREVIEWPAGE_H
#define PREVIEWPAGE_H

#include <QWebEnginePage>

class PreviewPage : public QWebEnginePage
{
    Q_OBJECT
public:
    PreviewPage(QObject* parent = nullptr);
};

#endif // PREVIEWPAGE_H
