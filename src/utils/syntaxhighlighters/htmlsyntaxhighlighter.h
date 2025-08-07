#ifndef HTMLSYNTAXHIGHLIGHTER_H
#define HTMLSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <qregularexpression.h>

class HTMLSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit HTMLSyntaxHighlighter(QTextDocument *parent = nullptr);

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;

protected:
    void highlightBlock(const QString &text) override;
};

#endif // HTMLSYNTAXHIGHLIGHTER_H
