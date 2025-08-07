#include "htmlsyntaxhighlighter.h"

HTMLSyntaxHighlighter::HTMLSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QTextCharFormat format;
    format.setForeground(QColor::fromRgb(0, 100, 255));
    rule.pattern = QRegularExpression(QStringLiteral(R"(</?[a-zA-Z0-9]+.*?>)"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setForeground(QColor::fromRgb(125, 125, 80));
    rule.pattern = QRegularExpression(QStringLiteral(R"(\b[a-zA-Z0-9_:-]+(?=\s*=))"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setForeground(QColor::fromRgb(206, 145, 120));
    rule.pattern = QRegularExpression(QStringLiteral(R"("[^"]*"|'[^']*')"));
    rule.format = format;
    highlightingRules.append(rule);
    format.setForeground(QColor::fromRgb(150, 150, 150));
    rule.pattern = QRegularExpression(QStringLiteral(R"(<!DOCTYPE.*?>)"));
    rule.format = format;
    highlightingRules.append(rule);

}

void HTMLSyntaxHighlighter::highlightBlock(const QString &text)
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
