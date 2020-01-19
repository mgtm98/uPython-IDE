#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSplitter>
#include <QToolBar>
#include <QPlainTextEdit>
#include <QFile>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QMap>
#include <QDir>
#include "directoryviewer.h"
#include "highlighter.h"
#include "editor.h"

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDirViewDoubleClick(QModelIndex);
    void closeTab(int);

private:

    typedef struct EditorFile{
        QString filePath;
        Editor *editor;
    } EditorFile;

    QSplitter *centerWidget;
    DirectoryViewer *dirViewer;
    QTabWidget *editorTabWidget;
    Editor *mainEditor;
    QToolBar *toolBar;
    QPlainTextEdit *term;
    Highlighter *h;
    QMap <QString, EditorFile*> *activeFiles;
    QString workspacePath;

    void initToolBar();


};
#endif // MAINWINDOW_H
