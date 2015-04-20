#include "TagDialog.h"
#include "ui_TagDialog.h"
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QDebug>
#include <iostream>

TagDialog::TagDialog(QWidget *parent):
    QDialog(parent),ui(new Ui::TagDialog)
{
    ui->setupUi(this);
/*    int nWidth=200;
    int nHeight=100;
    if(parent!=NULL)
    {
	setGeometry(parent->x()+parent->width()/2-nWidth/2,
	parent->y()+parent->height()/2-nHeight/2,
    nWidth,nHeight);
    }
    else
    {
    resize(nWidth,nHeight);
    }*/
    //ReceiveDataFromMainWindow();
}
TagDialog::~TagDialog()
{
    delete ui;
}

void TagDialog::RecSignalFromMainWindowTag(std::string fMsgTag,std::string sMsgTag)
{
    ui->listWidget->addItem(fMsgTag.c_str());
    ui->listWidget->addItem(sMsgTag.c_str());
}