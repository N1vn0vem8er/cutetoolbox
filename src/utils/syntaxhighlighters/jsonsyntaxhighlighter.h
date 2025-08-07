#ifndef JSONSYNTAXHIGHLIGHTER_H
#define JSONSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <qregularexpression.h>

class JSONSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit JSONSyntaxHighlighter(QTextDocument *parent = nullptr);

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

#endif // JSONSYNTAXHIGHLIGHTER_H
