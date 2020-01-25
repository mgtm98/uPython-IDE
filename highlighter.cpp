#include "highlighter.h"
Highlighter::Highlighter(QTextDocument *parent):QSyntaxHighlighter(parent){
    keywordFormat.setForeground(QColor("#20A98B"));
    const QString keywordPatterns[] = {
           ">>>"
    };

    HighlightingRule rule;
    for (const QString &pattern : keywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
    }

}

void Highlighter::highlightBlock(const QString &text){
    for(const HighlightingRule &rule : qAsConst(highlightingRules)){
        QRegularExpressionMatchIterator iterator = rule.pattern.globalMatch(text);
        while(iterator.hasNext()){
            QRegularExpressionMatch match = iterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
