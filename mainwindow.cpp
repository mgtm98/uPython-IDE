#include "mainwindow.h"
#include <QFile>
#include <QDebug>

#include <QTreeWidgetItem>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    setCentralWidget(centerWidget);

    centerWidget->setContentsMargins(0,0,0,0);

    centerWidget->addWidget(dirViewer);
    centerWidget->addWidget(editorTabWidget);


    editorTabWidget->addTab(mainEditor,"Python");
    editorTabWidget->addTab(new Editor,"Python");
    editorTabWidget->addTab(new Editor,"Python");

    editorTabWidget->setTabsClosable(true);

    menuBar();

    dirViewer->setMaximumWidth(200);
    showMaximized();

    toolBar = new QToolBar();
    toolBar->setOrientation(Qt::Vertical);
        QPixmap openpix(":/icons/open.png");
        QPixmap newpix(":/icons/new.png");
        QPixmap savepix(":/icons/connect.png");
        QPixmap compilepix(":/icons/download.png");
        toolBar->addAction(QIcon(newpix),"New");
        toolBar->addAction(QIcon(openpix),"Open");
        toolBar->addAction(QIcon(savepix),"Save");
        toolBar->addAction(QIcon(compilepix),"Compile");

    toolBar->setMovable(false);

    addToolBar(Qt::LeftToolBarArea, toolBar);
}

MainWindow::~MainWindow(){
}

