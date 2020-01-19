#include "mainwindow.h"
#include <QFile>
#include <QDebug>

#include <QTreeWidgetItem>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    setCentralWidget(centerWidget);

    centerWidget->setContentsMargins(0,0,0,0);

    centerWidget->addWidget(dirViewer);
    centerWidget->addWidget(editorTabWidget);
    centerWidget->addWidget(term);

    editorTabWidget->addTab(mainEditor,"Ros.py");
    editorTabWidget->addTab(new Editor,"TcpServer.py");
    editorTabWidget->addTab(new Editor,"walkArround.py");

    editorTabWidget->setTabIcon(0, QPixmap(":/icons/icons/python.png"));
    editorTabWidget->setTabIcon(1, QPixmap(":/icons/icons/python.png"));
    editorTabWidget->setTabIcon(2, QPixmap(":/icons/icons/python.png"));

    term->setObjectName("Terminal");
    term->setMaximumWidth(350);

    h = new Highlighter(term->document());

    editorTabWidget->setTabsClosable(true);

    menuBar();

    dirViewer->setMaximumWidth(200);
    showMaximized();

    toolBar = new QToolBar();
    toolBar->setOrientation(Qt::Vertical);
    QPixmap newpix(":/icons/icons/new.png");
    QPixmap openpix(":/icons/icons/open.png");
    QPixmap connectpix(":/icons/icons/connect.png");
    QPixmap disconnectpix(":/icons/icons/disconnect.png");
    QPixmap resetpix(":/icons/icons/reset.png");
    QPixmap downloadpix(":/icons/icons/download.png");
    QPixmap filepix(":/icons/icons/file.png");
    toolBar->addAction(QIcon(newpix),"New");
    toolBar->addAction(QIcon(openpix),"Open");
    toolBar->addAction(QIcon(connectpix),"Connect");
    toolBar->addAction(QIcon(disconnectpix),"Disconnect");
    toolBar->addAction(QIcon(resetpix),"Soft Reset");
    toolBar->addAction(QIcon(downloadpix),"Download");
    toolBar->addAction(QIcon(filepix),"uPython File System");
    toolBar->setMovable(false);

    addToolBar(Qt::LeftToolBarArea, toolBar);
}

MainWindow::~MainWindow(){
}

