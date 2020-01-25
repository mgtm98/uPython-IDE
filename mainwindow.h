#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSplitter>
#include <QToolBar>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QDir>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QInputDialog>
#include "directoryviewer.h"
#include "highlighter.h"
#include "pythonHighlighter.h"
#include "editor.h"
#include "espTerminal.h"
#include "upyfilesys.h"


class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDirViewDoubleClick(QModelIndex);
    void closeTab(int);
    void onConnectPressed();
    void onDisconnectPressed();
    void onSoftResetPressed();
    void onDownloadPressed();
    void onConnected();
    void onEspFileSysClicked(QListWidgetItem*);
    void onuFileSysRead(QStringList);
    void onFileSysRefreshClicked();
    void onFileSysNewFolderClicked();
    void onFileSyDeleteFolderClicked();
    void onNewFolderNamebtnClicked();
    void onTerminalOpFinished();
    void onSaveBtnClicked();
    void onOpenFolderClicked();
    void onNewFileClicked();

private:

    typedef struct EditorFile{
        QString filePath;
        Editor *editor;
    } EditorFile;

    QSplitter *centerWidget;
    QSplitter *mainSplitter;
    DirectoryViewer *dirViewer;
    QTabWidget *editorTabWidget;
    QTabWidget *bottomPanelTabWidget;
    Editor *mainEditor;
    QToolBar *toolBar;
    uPyTerminal *term;
    Highlighter *h;
    QMap <QString, EditorFile*> *activeFiles;
    QString workspacePath;

    QAction *newBtnAction;
    QAction *openBtnAction;
    QAction *saveBtnAction;
    QAction *connectBtnAction;
    QAction *disconnectBtnAction;
    QAction *sresetBtnAction;
    QAction *downloadBtnAction;

    QWidget *leftPanel;
    QVBoxLayout *leftPanelLay;
    QLabel *workSpacelbl;

    QLabel *connectedStatus;
    QLabel *portNameStatus;
    QLabel *baudRateStatus;

    upyFileExplorer *upyFileSys;
    QString upyDir;
    QString upyDirLastClick;
    QHBoxLayout *fileSysLayout;
    QWidget *fileSys;
    QWidget *fileSysToolbar;
    QVBoxLayout *fileSysToolbarLayout;
    QPushButton *newFolder  ;
    QPushButton *deleteFolder;
    QPushButton *refresh;
    bool fileSysrefresh = false;
    QVBoxLayout *fileSysLayout2;
    QHBoxLayout *fileSysNewFolderLayout;
    QLineEdit *newFolderName;
    QPushButton *newFolderNamebtn;

    void initToolBar();
    void initFileSysPanel();
    void addFileToEditior(QString);

};
#endif // MAINWINDOW_H
