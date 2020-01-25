#ifndef DIRECTORYVIEWER_H
#define DIRECTORYVIEWER_H

#include <QTreeView>
#include <QFileSystemModel>
#include <QHeaderView>

class DirectoryViewer : public QTreeView{
    Q_OBJECT
public:
    DirectoryViewer(QWidget *parent = nullptr);
    DirectoryViewer(QString path, QWidget *parent = nullptr);
    void setHeader(QString);
    QFileSystemModel* getFileSystemModel();
    void setRoot(QString);

private:
    QFileSystemModel *fmodel;
};

#endif // DIRECTORYVIEWER_H
