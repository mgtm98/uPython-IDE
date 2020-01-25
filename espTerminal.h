#ifndef TERMINAL_H
#define TERMINAL_H

#include <QSerialPort>
#include <QPlainTextEdit>
#include <QWidget>
#include <QSyntaxHighlighter>
#include <QScrollBar>
#include <QSerialPortInfo>

class uPyTerminal : public QPlainTextEdit{
    Q_OBJECT
public:
    uPyTerminal(QWidget *parent = nullptr);
    uPyTerminal(QSyntaxHighlighter*, QWidget *parent = nullptr);
    void open(QString portName, int baudRate);
    void close();
    void send(QString);
    void send(QChar);
    void upload(QString,QString,QString);
    void getDir(QString path = "/");
//    void readFile(QString,QString);
    void createFolder(QString,QString);
    void deleteFolder(QString,QString);

    static QStringList getPorts();

    static const char CTRLD = '\u0004';
    static const char CTRLB = '\u0002';
    static const char CTRLE = '\u0005';

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private:
    QSerialPort *port;
    int lastKey;
    bool freez = false;
    bool isESC = false;
    bool isESCack = false;
    bool isNumber = false;
    QString number = "1";
    QString dir = "";
    void currentDir_analyze();

private slots:
    void readData();

signals:
    void connected();
    void errorHappened(QString);
    void upyPathsReady(QStringList);
    void opFinished();
};

#endif // TERMINAL_H
