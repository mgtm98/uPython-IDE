#include "espTerminal.h"

#include <QDebug>

#define ESC '\u001B'
#define TAB QString("    ")

uPyTerminal::uPyTerminal(QSyntaxHighlighter *highlighter, QWidget *parent) : QPlainTextEdit(parent){
    port = new QSerialPort();
    highlighter->setDocument(document());
    setCursorWidth(7);
    connect(port, &QSerialPort::readyRead, this, &uPyTerminal::readData);
}

uPyTerminal::uPyTerminal(QWidget *parent):uPyTerminal(nullptr,parent){
}

void uPyTerminal::run(QString code){
    send(uPyTerminal::CTRLE);
    send(code);
    send(uPyTerminal::CTRLD);
}

QStringList uPyTerminal::getPorts(){
    QStringList ports;
    for(QSerialPortInfo i : QSerialPortInfo::availablePorts()){
        ports.append(i.portName());
    }
    return ports;
}

// bugs in it ..... refactor it in a cleaner code or use regex / coloring escape / empty line then up key then down key
void uPyTerminal::readData(){
    QString data = port->readAll();

    if(freez && data.lastIndexOf("$") >= 0){
        freez = false;
        emit opFinished();
        return;
    }else if(freez && data.lastIndexOf("]") >= 0){
        dir+=data;
        currentDir_analyze();
        freez = false;
        return;
    }else if(freez){
        dir+=data;
    }
    if(!freez){
        if(data.length() > 1){
            for (int a = 0; a < data.length() ; a++){
                QChar c = data[a];
                if(c == '\b'){
                    QTextCursor t = textCursor();
                    t.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,1);
                    setTextCursor(t);
                }else if(c == ESC){
                    isESC = true;
                }else if(c == '[' && isESC){
                    isESCack = true;
                }else if(isESC && isESCack){
                    if(c.isDigit()){
                        if(isNumber){
                            number += c;
                        }else{
                            number = c;
                            isNumber = true;
                        }
                    }else{
                        if(c == 'D'){
                            QTextCursor t = textCursor();
                            t.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,number.toInt());
                            setTextCursor(t);
                        }else if(c == 'K'){
                            for(int counter = 0; counter < number.toInt()+1; counter++ ){
                                textCursor().deleteChar();
                            }
                        }else{
//                            qDebug() << "escape code not supported " << c;
                        }
                        isESCack = false;
                        isESC = false;
                        isNumber = false;
                        number = "1";
                    }
                }else{
                    QTextDocument *d = document();
                    QString doc = d->toRawText();
                    doc.remove(textCursor().position(),doc.length()-textCursor().position());
                    d->setPlainText(doc);
                    if(c != '\r' )insertPlainText(QString(c));
                }
            }
        }else{
            if(!data.compare("\b")){
                QTextCursor t = textCursor();
                t.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,1);
                setTextCursor(t);
            }else if(!data.compare(QString(ESC))){
                isESC = true;
            }else if(lastKey != Qt::Key_Right){
                if(data != '\r' ) insertPlainText(data);
                textCursor().movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
            }
        }
    }
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void uPyTerminal::currentDir_analyze(){
//    qDebug() << dir;
    int index1 = dir.indexOf("[");
    if(index1 > 0){
        int index2 = dir.indexOf("]");
        QString paths = dir.mid(index1+1,index2-index1-1);
        paths.replace(" ","");
        paths.replace("'","");
        emit upyPathsReady(paths.split(","));
        dir = "";
    }else{
        emit upyPathsReady(QStringList());
    }
}

void uPyTerminal::open(QString portName, int baudRate){
    port->setPortName(portName);
    port->setBaudRate(baudRate);
    if(port->open(QIODevice::ReadWrite)){
        emit connected();
        qDebug() << "connected";
    }else{
        emit errorHappened("Couldn't connect to port " + portName);
        qDebug() << "couldn't connect";
    }
}

void uPyTerminal::close(){
    port->close();
}

void uPyTerminal::keyPressEvent(QKeyEvent *e){
    lastKey = e->key();
    switch (e->key()) {
        case Qt::Key_Backspace:
            port->write("\b");
        break;
        case Qt::Key_Left:
            port->write("\x1b[D");
        break;
        case Qt::Key_Right:
            QPlainTextEdit::keyPressEvent(e);
            port->write("\x1b[C");
        break;
        case Qt::Key_Up:
            port->write("\x1b[A");
        break;
        case Qt::Key_Down:
            port->write("\x1b[B");
        break;
        case Qt::Key_Enter:
            textCursor().movePosition(QTextCursor::End);
        default:
            port->write(e->text().toLocal8Bit());
//            qDebug() << e->text();
    }
}

void uPyTerminal::send(QString data){
    port->write(data.toLocal8Bit());
}

void uPyTerminal::send(QChar data){
    port->write(QString(data).toLocal8Bit());
}

void uPyTerminal::mousePressEvent(QMouseEvent *e){
    Q_UNUSED(e)
    setFocus();
}

void uPyTerminal::mouseDoubleClickEvent(QMouseEvent *e){
    Q_UNUSED(e)
}

void uPyTerminal::contextMenuEvent(QContextMenuEvent *e){
    Q_UNUSED(e)
}

void uPyTerminal::upload(QString code, QString fileName, QString path){
    freez = true;
    send(uPyTerminal::CTRLE);
    send("import os \r");
    send("os.chdir('"+path+"') \r");
    send("data = '''\n" + code + "\n'''\n");
    send("f = open(\"" + fileName + "\",'w')\n");
    send("f.write(data) \r");
    send("f.close() \r");
    send("del f, data \r");
    send("chr(36)");
    send(uPyTerminal::CTRLD);
}

void uPyTerminal::getDir(QString path){
//    qDebug() << path;
    freez = true;
    send(uPyTerminal::CTRLE);
    send("import os\n\r");
    send("try :\r");
    send(TAB + "os.chdir(\""+path+"\")\r");
    send(TAB + "os.listdir()\r");
    send("except : \r");
    send(TAB + "print(chr(93))\r");
    send(uPyTerminal::CTRLD);

}

void uPyTerminal::createFolder(QString path, QString folderName){
    freez = true;
    send(uPyTerminal::CTRLE);
    send("import os\n\r");
    send("os.mkdir(\""+path+"/"+folderName+"\")\r");
    send("chr(36)");
    send(uPyTerminal::CTRLD);
}

void uPyTerminal::deleteFolder(QString path, QString folderName){
    freez = true;
    send(uPyTerminal::CTRLE);
    send("import os\n\r");
    send("try :\r");
    send(TAB + "f = open(\""+path+"/"+folderName+"\",'r')\r");
    send(TAB + "f.close()\r");
    send(TAB + "os.remove(\""+path+"/"+folderName+"\")\r");
    send("except:");
    send(TAB + "os.rmdir(\""+path+"/"+folderName+"\")\r");
    send("chr(36)");
    send(uPyTerminal::CTRLD);
}

//void uPyTerminal::readFile(QString path, QString folderName){

//}
