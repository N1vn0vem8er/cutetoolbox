#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>

class Document : public QObject
{
    Q_OBJECT
public:
    explicit Document(QObject *parent = nullptr);
    void setText(const QString& text);

private:
    QString text;

signals:
    void textChanged(const QString& text);
};

#endif // DOCUMENT_H
