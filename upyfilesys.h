#ifndef UPYFILESYS_H
#define UPYFILESYS_H

#include <QListWidget>
#include <QDebug>

class upyFileExplorer : public QListWidget{
Q_OBJECT
public:
    upyFileExplorer();
    void setFiles();
    void addItem(QString);
    void setCurrentPath(QString);

private:
    QString path;
};

#endif // UPYFILESYS_H
