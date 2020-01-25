#include "mainwindow.h"
#define CONFIG_PATH ""

#include <QDebug>
#include <QDirModel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    centerWidget = new QSplitter(Qt::Horizontal);
    editorTabWidget = new QTabWidget();
    mainEditor = new Editor();
    toolBar = new QToolBar();
    term = new uPyTerminal(new Highlighter(), this);
    mainSplitter = new QSplitter(Qt::Vertical);
    connectedStatus = new QLabel();
    portNameStatus = new QLabel();
    baudRateStatus = new QLabel();
    activeFiles = new QMap<QString, EditorFile*>();
    leftPanel = new QWidget();
    leftPanelLay = new QVBoxLayout();
    workSpacelbl = new QLabel("   Workspace");
    bottomPanelTabWidget = new QTabWidget();
    upyFileSys = new upyFileExplorer();
    fileSysLayout = new QHBoxLayout();
    fileSys = new QWidget();
    fileSysToolbar = new QWidget();
    fileSysToolbarLayout = new QVBoxLayout();
    newFolder = new QPushButton();
    refresh = new QPushButton();
    deleteFolder = new QPushButton();
    fileSysLayout2 = new QVBoxLayout();
    ports = new QComboBox();
    loadingGif = new QMovie(":/icons/icons/loading.gif");

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

    leftPanel->setLayout(leftPanelLay);
    leftPanelLay->addWidget(workSpacelbl);
    leftPanelLay->addWidget(dirViewer);
    leftPanelLay->setContentsMargins(0,0,0,0);
    leftPanel->setMaximumWidth(170);
    leftPanel->setObjectName("LeftPanel");
    workSpacelbl->setObjectName("WorkSpace");
    workSpacelbl->setMinimumWidth(170);
    workSpacelbl->setMinimumHeight(25);

    centerWidget->addWidget(leftPanel);
    centerWidget->addWidget(mainSplitter);
    mainSplitter->addWidget(editorTabWidget);
    mainSplitter->addWidget(bottomPanelTabWidget);

    bottomPanelTabWidget->addTab(term,"Terminal");
    bottomPanelTabWidget->addTab(fileSys, "Device File System");
    mainSplitter->setObjectName("mainSplitter");

    upyFileSys->setEnabled(false);

    bottomPanelTabWidget->setMaximumHeight(200);
    bottomPanelTabWidget->setObjectName("BottomPanel");

    editorTabWidget->setTabsClosable(true);

    dirViewer->setMaximumWidth(170);

    showMaximized();
    initToolBar();
    initFileSysPanel();

    QStatusBar *status = statusBar();
    status->setMaximumHeight(20);
    status->addPermanentWidget(connectedStatus);
    status->addPermanentWidget(portNameStatus);
    status->addPermanentWidget(baudRateStatus);
    connectedStatus->setText("Disconnected !!");

    connect(dirViewer,&DirectoryViewer::doubleClicked,this,&MainWindow::onDirViewDoubleClick);
    connect(editorTabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(connectBtnAction, &QAction::triggered, this, &MainWindow::onConnectPressed);
    connect(disconnectBtnAction, &QAction::triggered, this, &MainWindow::onDisconnectPressed);
    connect(sresetBtnAction, &QAction::triggered, this, &MainWindow::onSoftResetPressed);
    connect(downloadBtnAction, &QAction::triggered, this, &MainWindow::onDownloadPressed);
    connect(term, &uPyTerminal::connected, this, &MainWindow::onConnected);
    connect(term, &uPyTerminal::opFinished, this, &MainWindow::onTerminalOpFinished);
    connect(upyFileSys,&upyFileExplorer::itemDoubleClicked , this, &MainWindow::onEspFileSysClicked);
    connect(term, &uPyTerminal::upyPathsReady, this, &MainWindow::onuFileSysRead);
    connect(saveBtnAction, &QAction::triggered, this, &MainWindow::onSaveBtnClicked);
    connect(openBtnAction, &QAction::triggered, this, &MainWindow::onOpenFolderClicked);
    connect(newBtnAction, &QAction::triggered, this, &MainWindow::onNewFileClicked);
}


void MainWindow::onNewFileClicked(){
    QFileSystemModel dir;
    QString path = dir.filePath(dirViewer->currentIndex());
    if(path.contains(".")){
        path = path.mid(0,path.lastIndexOf("/"));
    }else if(!path.compare("")) {
        path = workspacePath;
    }
    bool ok;
    QString newFileName = QInputDialog::getText(this, tr("Create new File"),tr("File name :"), QLineEdit::Normal,"newFile", &ok);
    if(ok){
        QFile f (path+"/"+newFileName);
        f.open(QIODevice::WriteOnly);
        f.close();
    }
    addFileToEditior(path+"/"+newFileName);
}

void MainWindow::onOpenFolderClicked(){
    QString path = QFileDialog::getExistingDirectory(this,tr("Open Workspace"), workspacePath);
    if(path.compare(""))
        dirViewer->setRoot(workspacePath);
}

void MainWindow::onSaveBtnClicked(){
    if(editorTabWidget->currentIndex() > -1){
        QString activeFile = editorTabWidget->tabText(editorTabWidget->currentIndex());
        QString path = activeFiles->value(activeFile)->filePath;
        Editor *e = activeFiles->value(activeFile)->editor;
        QFile f(path);
        if(f.open(QIODevice::WriteOnly)){
            f.write(e->toPlainText().toUtf8());
            f.close();
            statusBar()->showMessage("File Saved",2000);
        }
    }
}

void MainWindow::initFileSysPanel(){
    fileSys->setLayout(fileSysLayout);
    fileSysLayout->addLayout(fileSysLayout2);
    fileSysLayout2->addWidget(upyFileSys);
    fileSysLayout->addWidget(fileSysToolbar);
    fileSys->setObjectName("fileSys");
    fileSysToolbar->setMaximumWidth(100);
    fileSysToolbar->setObjectName("fileSysToolbar");
    fileSysToolbar->setLayout(fileSysToolbarLayout);
    fileSysToolbarLayout->setSpacing(0);
    fileSysToolbarLayout->addWidget(refresh);
    fileSysToolbarLayout->addWidget(newFolder);
    fileSysToolbarLayout->addWidget(deleteFolder);
    fileSysToolbarLayout->addWidget(new QWidget);
    refresh->setObjectName("fileSysToolRefresh");
    newFolder->setObjectName("fileSysToolNewFolder");
    deleteFolder->setObjectName("fileSysToolDeleteFolder");
    refresh->setIcon(QPixmap(":/icons/icons/refresh.png"));
    deleteFolder->setIcon(QPixmap(":/icons/icons/bin.png"));
    newFolder->setIcon(QPixmap(":/icons/icons/newFolder.png"));
    refresh->setIconSize(QSize(35,35));
    deleteFolder->setIconSize(QSize(35,35));
    newFolder->setIconSize(QSize(35,35));
    connect(refresh, &QPushButton::clicked, this, &MainWindow::onFileSysRefreshClicked);
    connect(newFolder, &QPushButton::clicked, this, &MainWindow::onFileSysNewFolderClicked);
    connect(deleteFolder, &QPushButton::clicked, this, &MainWindow::onFileSyDeleteFolderClicked);
}

void MainWindow::onFileSyDeleteFolderClicked(){
    if(upyFileSys->selectedItems().length()){
        term->deleteFolder(upyDir, upyFileSys->selectedItems()[0]->text());
    }
}

void MainWindow::onFileSysNewFolderClicked(){
    newFolderName = new QLineEdit();
    fileSysNewFolderLayout = new QHBoxLayout();
    newFolderNamebtn = new QPushButton("Insert");
    newFolderNamebtn->setObjectName("fileSysSaveBtn");
    fileSysLayout2->addLayout(fileSysNewFolderLayout);
    fileSysNewFolderLayout->addWidget(newFolderName);
    fileSysNewFolderLayout->addWidget(newFolderNamebtn);
    connect(newFolderNamebtn, &QPushButton::clicked, this, &MainWindow::onNewFolderNamebtnClicked);
}

void MainWindow::onTerminalOpFinished(){
    onFileSysRefreshClicked();
}

void MainWindow::onNewFolderNamebtnClicked(){
    term->createFolder(upyDir, newFolderName->text());
    delete newFolderNamebtn;
    delete newFolderName;
    delete fileSysNewFolderLayout;
}

void MainWindow::onFileSysRefreshClicked(){
    fileSysrefresh = true;
    term->getDir(upyDir.mid(0,upyDir.length()-1));
    qDebug() << upyDir.mid(0,upyDir.length()-1);
    showLaodingGif();
}

void MainWindow::onuFileSysRead(QStringList dataList){
    if(dataList.length() > 0){
        if(!fileSysrefresh){
            upyDir += upyDirLastClick + "/";
        }else{
            if(!upyDir.endsWith("/")) upyDir += "/";
        }
        upyFileSys->clear();
        if(upyDir.compare("//"))upyFileSys->addItem("..");
        for (QString s : dataList){
            if(s.length() > 0)upyFileSys->addItem(s);
        }
    }
    fileSysrefresh = false;
    upyFileSys->setCurrentPath(upyDir);
    delete loadingStatus;
    loadingGif->stop();
}

void MainWindow::onEspFileSysClicked(QListWidgetItem *item){
    if(!item->text().compare("..")){
        upyDirLastClick = "";
        QString s = upyDir.mid(0,upyDir.length()-1);
        upyDir = s.mid(0,s.lastIndexOf("/"));
        term->getDir(upyDir);
    }else if(item->text().contains(".")){
//        term->readFile(upyDir.mid(0,upyDir.length()-1),item->text());
//        TODO read files from upy
    }else{
        upyDirLastClick = item->text();
        term->getDir(upyDir+upyDirLastClick);
    }
    deleteLoadingGif();
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
    newBtnAction = toolBar->addAction(QIcon(newpix),"New");
    openBtnAction = toolBar->addAction(QIcon(openpix),"Open");
    saveBtnAction = toolBar->addAction(QIcon(savepix),"Save");
//    toolBar->addWidget(ports);
    connectBtnAction = toolBar->addAction(QIcon(connectpix),"Connect");
    disconnectBtnAction = toolBar->addAction(QIcon(disconnectpix),"Disconnect");
    sresetBtnAction = toolBar->addAction(QIcon(resetpix),"Soft Reset");
    downloadBtnAction = toolBar->addAction(QIcon(downloadpix),"Download");

    toolBar->setMovable(false);
    addToolBar(Qt::LeftToolBarArea, toolBar);

//    ports->addItems(uPyTerminal::getPorts());
//    ports->setObjectName("portsCombobox");
////    ports->setMaximumWidth(40);

}

void MainWindow::onDirViewDoubleClick(QModelIndex modelIndex){
    QString path = dirViewer->getFileSystemModel()->filePath(modelIndex);
    addFileToEditior(path);
}

void MainWindow::addFileToEditior(QString path){
    QString name = path.mid(path.lastIndexOf("/")+1, path.size());
    QString type = path.mid(path.lastIndexOf(".")+1, path.size()-path.lastIndexOf("."));
    if(!path.compare(type)) type = "";
    if(type.size()) name = name.mid(0,name.size()-type.size()-1);
    QString tabName;
    if(type.size() == 0) tabName = name;
    else tabName = name + "." + type;

    QDir dir(path);
    if(!dir.exists()){
        QFile f(path);
        f.open(QIODevice::Text | QIODevice::ReadOnly);
        QString content = f.readAll();
        f.close();

        Editor *e;

        if(!type.compare("py")){
            e = new Editor(new PythonHighlighter(), this);
            int tabIndex = editorTabWidget->addTab(e,tabName);
            editorTabWidget->setTabIcon(tabIndex, QPixmap(":/icons/icons/python.png"));
        }else{
            e = new Editor();
            int tabIndex = editorTabWidget->addTab(e,tabName);
            editorTabWidget->setTabIcon(tabIndex, QPixmap(":/icons/icons/txt.png"));
        }

        e->setPlainText(content);

        EditorFile *eFile = new EditorFile;
        eFile->editor = e;
        eFile->filePath = path;
        activeFiles->insert(tabName,eFile);
    }
}

void MainWindow::closeTab(int index){
    QString editorName = editorTabWidget->tabText(index);
    editorTabWidget->removeTab(index);
    delete activeFiles->value(editorName);
    activeFiles->remove(editorName);
}

void MainWindow::onConnectPressed(){
    bool ok;
    if(!connected){
        QString portName = QInputDialog::getItem(this, tr("Select Serial Port"),
                                              tr("uPython Serial Port : "), uPyTerminal::getPorts(), 0, false, &ok);
        if(ok){
            term->open("/dev/"+portName,115200);
        }
    }
}

void MainWindow::onDisconnectPressed(){
    term->close();
    connectBtnAction->setIcon(QPixmap((":/icons/icons/connect.png")));
    term->setPlainText("Disconnected from repl ... \n ");
    term->moveCursor(QTextCursor::End);
    term->setCursorWidth(0);
    upyFileSys->setEnabled(false);
    connected = false;
}

void MainWindow::onConnected(){
    connectBtnAction->setIcon(QPixmap((":/icons/icons/connected.png")));
    term->setPlainText("Connected to micropython repl .... \n>>> ");
    term->moveCursor(QTextCursor::End);
    connectedStatus->setText("Connected");
    portNameStatus->setText("port : /dev/ttyUSB0");
    baudRateStatus->setText("baudRate : 115200");
    term->setCursorWidth(7);
    upyFileSys->setEnabled(true);
    connected = true;
}

void MainWindow::onSoftResetPressed(){
    term->send(QString('\u0004'));
}

void MainWindow::onDownloadPressed(){
    int index = editorTabWidget->currentIndex();
    if(index > -1){
        QString activeEditor = editorTabWidget->tabText(index);
        QString code = activeFiles->value(activeEditor)->editor->document()->toPlainText();
        term->upload(code,activeEditor,upyDir.mid(0,upyDir.length()-1));
    }
    showLaodingGif();
    // TODO downloading / reset esp32 alot / fix bug in reading from serial
}

void MainWindow::showLaodingGif(){
//    if(loadingGif->state() != QMovie::Running){
//        loadingStatus = new QLabel();
//        loadingStatus->setMovie(loadingGif);
//        loadingGif->start();
//        statusBar()->addWidget(loadingStatus);
//    }
}

void MainWindow::deleteLoadingGif(){
//    if(loadingStatus != nullptr){
//        delete loadingStatus;
//        loadingGif->stop();
//    }
}

MainWindow::~MainWindow(){
}

