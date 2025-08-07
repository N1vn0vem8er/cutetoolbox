#include "jsonsyntaxhighlighter.h"

JSONSyntaxHighlighter::JSONSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter{parent}
{
    HighlightingRule rule;
    QTextCharFormat format;
    format.setForeground(QColor::fromRgb(0, 255, 255));
    rule.pattern = QRegularExpression(QStringLiteral(R"(\d)"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setForeground(QColor::fromRgb(206, 145, 120));
    rule.pattern = QRegularExpression(QStringLiteral(R"(".*"|'.*')"));
    rule.format = format;
    highlightingRules.append(rule);
}

void JSONSyntaxHighlighter::highlightBlock(const QString &text)
{
    for(const HighlightingRule& rule : std::as_const(highlightingRules))
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while(matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
