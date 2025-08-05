#include "document.h"

Document::Document(QObject *parent)
    : QObject{parent}
{}

void Document::setText(const QString &text)
{
    if(this->text == text)
        return;
    this->text = text;
    emit textChanged(this->text);
}
