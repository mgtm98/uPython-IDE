#include "editor.h"

Editor::Editor(QWidget *parent):QPlainTextEdit(parent){
    numArea = new LineNumberArea(this);

    setStyleSheet("line-height : 100px");

    numArea->setStyleSheet("background-color : #000000");
    this->setBackgroundVisible(true);
//    highlight = new Highlighter(this->document());
    pyHighlightrt = new PythonHighlighter(this->document());
    instTable = new QMap<int,int>;
    connect(this, &Editor::blockCountChanged, this, &Editor::updateNumAreaWidth);
    connect(this, &Editor::updateRequest, this, &Editor::updateNumArea);
    connect(this, &Editor::cursorPositionChanged, this, &Editor::highlightCurrentLine);
    setViewportMargins(lineNumberWidth(),0,0,0);


//    QTextDocument* doc = this->document();
//    QTextBlock currentBlock = doc->firstBlock();

//    while (currentBlock.isValid()) {
//        qDebug() << "here";
//        QTextCursor cursor(currentBlock);
//        QTextBlockFormat blockFormat = currentBlock.blockFormat();
//        blockFormat.setLineHeight(200, QTextBlockFormat::ProportionalHeight);
//        cursor.setBlockFormat(blockFormat);
//        currentBlock = currentBlock.next();
//    }
}

int Editor::lineNumberWidth(){
    int digits = 1;
    int max = qMax(1, blockCount());
    while(max >= 10){
        max /= 10;
        digits ++;
    }
    int space = 3 + digits * fontMetrics().horizontalAdvance("9");
    return space + 40;
}

void Editor::updateNumAreaWidth(int number){
    setViewportMargins(lineNumberWidth(),0,0,0);
}

void Editor::updateNumArea(const QRect &rect, int dy){
    if (dy)
        numArea->scroll(0, dy);
    else
        numArea->update(0, rect.y(), numArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateNumAreaWidth(0);
}

void Editor::resizeEvent(QResizeEvent *e){
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    numArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberWidth(), cr.height()));
}

void Editor::lineNumberPaintEvent(QPaintEvent *event){

        QPainter painter(numArea);
        QTextBlock block = firstVisibleBlock();
        int blockNumber = block.blockNumber();
        qreal top = blockBoundingGeometry(block).translated(contentOffset()).top();
        qreal bottom = top + blockBoundingRect(block).height();

        while (block.isValid() && top <= event->rect().bottom()) {
            if (block.isVisible() && bottom >= event->rect().top()) {
                QString number = QString::number(blockNumber + 1);
                painter.drawText(-20, top, numArea->width(), fontMetrics().height(),
                                 Qt::AlignRight, number);
            }

            block = block.next();
            top = bottom;
            bottom = top + blockBoundingRect(block).height();
            ++blockNumber;
        }
}

void Editor::highlightCurrentLine(){
        QList<QTextEdit::ExtraSelection> extraSelections;
        if (!isReadOnly()) {
            QTextEdit::ExtraSelection selection;
            QColor lineColor = QColor(218, 220, 224);
//            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            extraSelections.append(selection);
        }
        setExtraSelections(extraSelections);
}

void Editor::highlightLine(int line){
    for(int a = 0; a < this->document()->blockCount(); a++){
        QTextCursor cursor(this->document()->findBlockByLineNumber(a));
        QTextBlockFormat blockFormat = cursor.blockFormat();
//        blockFormat.setBackground(a == line ? color : QColor(Qt::white));
        cursor.setBlockFormat(blockFormat);
    }
}

void Editor::highlightMultiLine(QMap<int,QColor> lines){
    for(int a = 0; a < this->document()->blockCount(); a++){
        QTextCursor cursor(this->document()->findBlockByLineNumber(a));
        QTextBlockFormat blockFormat = cursor.blockFormat();
//        blockFormat.setBackground(lines.contains(a) ? lines.value(a) : QColor(Qt::white));
        cursor.setBlockFormat(blockFormat);
    }
}

void Editor::setHighlighColor(QColor c){
    color = c;
}

void Editor::mapLineToInst(int count){
    delete  instTable;
    instTable = new QMap<int,int>;
    int i = 0;
    qDebug() << "line count changed";
    for(int a = 0; a < count; a++){
        if(this->document()->findBlockByLineNumber(a).text().contains(QRegExp("[A-Za-z0-9]+"))){
            instTable->insert(i,a);
            i++;
            maxInstructionNumbers = i;
        }
    }
}

void Editor::reset(){
    for(int a = 0; a < this->document()->blockCount(); a++){
        QTextCursor cursor(this->document()->findBlockByLineNumber(a));
        QTextBlockFormat blockFormat = cursor.blockFormat();
//        blockFormat.setBackground(QColor(Qt::white));
        cursor.setBlockFormat(blockFormat);
    }
}

int Editor::getInstNumber(int inst){
    if(inst < maxInstructionNumbers){return instTable->value(inst);}
    else {return maxInstructionNumbers;}
}

Editor::~Editor(){
    qDebug() << "Removed an editor";
}
