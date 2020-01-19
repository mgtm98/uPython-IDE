#include "mainwindow.h"
#define CONFIG_PATH ""

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){

    centerWidget = new QSplitter(Qt::Horizontal);
    editorTabWidget = new QTabWidget();
    mainEditor = new Editor();
    toolBar = new QToolBar();
    term = new QPlainTextEdit();
    activeFiles = new QMap<QString, EditorFile*>();
    h = new Highlighter(term->document());

    QFile configFile(QString(CONFIG_PATH)+"config.txt");
    configFile.open(QIODevice::ReadWrite);
    workspacePath = configFile.readLine();
    configFile.close();
    if(workspacePath.endsWith("\n")){
        workspacePath.chop(1);
    }
    dirViewer = new DirectoryViewer(workspacePath,this);

    setCentralWidget(centerWidget);
    centerWidget->setContentsMargins(0,0,0,0);

    centerWidget->addWidget(dirViewer);
    centerWidget->addWidget(editorTabWidget);
    centerWidget->addWidget(term);

    term->setObjectName("Terminal");
    term->setMaximumWidth(350);

    editorTabWidget->setTabsClosable(true);

    dirViewer->setMaximumWidth(170);
    showMaximized();

    initToolBar();

    connect(dirViewer,&DirectoryViewer::doubleClicked,this,&MainWindow::onDirViewDoubleClick);
    connect(editorTabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);

    // TODO save File
}

void MainWindow::initToolBar(){
    toolBar->setOrientation(Qt::Vertical);
    QPixmap newpix(":/icons/icons/new.png");
    QPixmap openpix(":/icons/icons/open.png");
    QPixmap savepix(":/icons/icons/save.png");
    QPixmap connectpix(":/icons/icons/connect.png");
    QPixmap disconnectpix(":/icons/icons/disconnect.png");
    QPixmap resetpix(":/icons/icons/reset.png");
    QPixmap downloadpix(":/icons/icons/download.png");
    QPixmap filepix(":/icons/icons/file.png");
    toolBar->addAction(QIcon(newpix),"New");
    toolBar->addAction(QIcon(openpix),"Open");
    toolBar->addAction(QIcon(savepix),"Save");
    toolBar->addAction(QIcon(connectpix),"Connect");
    toolBar->addAction(QIcon(disconnectpix),"Disconnect");
    toolBar->addAction(QIcon(resetpix),"Soft Reset");
    toolBar->addAction(QIcon(downloadpix),"Download");
    toolBar->addAction(QIcon(filepix),"uPython File System");
    toolBar->setMovable(false);
    addToolBar(Qt::LeftToolBarArea, toolBar);
}

void MainWindow::onDirViewDoubleClick(QModelIndex modelIndex){

    QString path = dirViewer->getFileSystemModel()->filePath(modelIndex);
    QString name = path.mid(path.lastIndexOf("/")+1, path.size());
    QString type = path.mid(path.lastIndexOf(".")+1, path.size()-path.lastIndexOf("."));
    if(!path.compare(type)) type = "";
    if(type.size()) name = name.mid(0,name.size()-type.size()-1);
    QString tabName;
    if(type.size() == 0) tabName = name;
    else tabName = name + "." + type;
    qDebug() << path;
    qDebug() << name;
    qDebug() << type;

    QDir dir(path);
    if(!dir.exists()){
        QFile f(path);
        f.open(QIODevice::Text | QIODevice::ReadOnly);
        QString content = f.readAll();
        f.close();

        Editor *e = new Editor();
        e->setPlainText(content);
        int tabIndex = editorTabWidget->addTab(e,tabName);

        if(!type.compare("py")) editorTabWidget->setTabIcon(tabIndex, QPixmap(":/icons/icons/python.png"));
        else editorTabWidget->setTabIcon(tabIndex, QPixmap(":/icons/icons/txt.png"));

        EditorFile *eFile = new EditorFile;
        eFile->editor = e;
        eFile->filePath = path;
        activeFiles->insert(name,eFile);
    }
}

void MainWindow::closeTab(int index){
    QString editorName = editorTabWidget->tabText(index);
    editorTabWidget->removeTab(index);
    delete activeFiles->value(editorName);
    activeFiles->remove(editorName);
}

MainWindow::~MainWindow(){
}

