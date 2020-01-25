#include "upyfilesys.h"

upyFileExplorer::upyFileExplorer(){
    setSpacing(0);
    addItem("/");
}

void upyFileExplorer::addItem(QString txt){
    QListWidgetItem *item = new QListWidgetItem();
//    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setText(txt);
    if(!txt.contains(".")){
        item->setIcon(QPixmap(":/icons/icons/folder.png"));
    }else{
        if(!txt.mid(txt.indexOf(".")+1).compare("py"))
            item->setIcon(QPixmap(":/icons/icons/python.png"));
        else if(!txt.mid(txt.indexOf(".")+1).compare("."))
            item->setIcon(QPixmap(":/icons/icons/back.png"));
        else
            item->setIcon(QPixmap(":/icons/icons/txt.png"));
    }
    QListWidget::addItem(item);
}

void upyFileExplorer::setCurrentPath(QString txt){
    path = txt;
}
