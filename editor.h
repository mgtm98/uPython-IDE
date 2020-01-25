#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QtDebug>
#include <QSyntaxHighlighter>

class LineNumberArea;

class Editor : public QPlainTextEdit{
    Q_OBJECT

public:
    struct lineHiighLight{
        int line;
        QColor color;
    };
    Editor(QWidget *parent = nullptr);
    Editor(QSyntaxHighlighter*,QWidget*);
    void lineNumberPaintEvent (QPaintEvent *e);
    int lineNumberWidth();
    void highlightLine(int);
    void highlightMultiLine(QMap<int,QColor>);
    int getInstNumber(int);
    void mapLineToInst(int);
    void reset();
    void setHighlighColor(QColor);
    ~Editor();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateNumAreaWidth(int);
    void highlightCurrentLine();
    void updateNumArea(const QRect &, int);


private:
    LineNumberArea *numArea;
    QSyntaxHighlighter *pyHighlightrt;
    QMap<int,int> *instTable;
    QColor color;
    int maxInstructionNumbers;
    void initFont();
};

class LineNumberArea : public QWidget{
    Q_OBJECT
public:
    LineNumberArea(Editor *editor) : QWidget(editor) {

        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberPaintEvent(event);
    }

private:
    Editor *codeEditor;
};


#endif // EDITOR_H
