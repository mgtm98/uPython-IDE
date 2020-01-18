#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSplitter>
#include <QToolBar>

#include <QPlainTextEdit>


#include "directoryviewer.h"
#include "editor.h"

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QSplitter *centerWidget = new QSplitter(Qt::Horizontal);
    DirectoryViewer *dirViewer = new DirectoryViewer();
    QTabWidget *editorTabWidget = new QTabWidget();
    Editor *mainEditor = new Editor();
    QToolBar *toolBar;

};
#endif // MAINWINDOW_H
