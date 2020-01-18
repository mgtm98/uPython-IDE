#ifndef DIRECTORYVIEWER_H
#define DIRECTORYVIEWER_H

#include <QTreeView>
#include <QFileSystemModel>
#include <QHeaderView>

class DirectoryViewer : public QTreeView{
    Q_OBJECT
public:
    DirectoryViewer(QWidget *parent = nullptr);
    void setHeader(QString);
    QFileSystemModel* getFileSystemModel();

private:
    QFileSystemModel *fmodel;
};

#endif // DIRECTORYVIEWER_H
